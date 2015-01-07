#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/*mode说明
 * -2 只用于mode_1,用于第一次ADCReInit
 * -1 初始化生成正弦表
 * 0 用于自同步
 * 9 用于并网Pset，Qset
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//声明本函数需要的外部变量
//储存性变量
extern Uint16 adcresults[6];
//计算性变量
extern float32 e_pwm;
//控制性变量
extern int16 mode,mode_1; //模式控制
extern Uint16 flag_PWMEnable; //控制脉冲波
Uint16 flag_DataSend=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本函数测试变量
int16 input_back,input_forward,input_backL,input_backH,input_forwardL,input_forwardH;
Uint16 number;
extern float32 w,vg,e,ig,Q,Te,i[2],Mfif; //采集量
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//主函数
void main(void)
{
 	InitSysCtrl();
	DINT;
    InitPieCtrl();
    IER=0x0000;
    IFR=0x0000;
    InitPieVectTable();
//    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//    InitFlash();
    DELAY_US(50000L); //延时50ms，等待其他模块完成初始化

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //配置ePWM前关闭TBCLK
    EDIS;
    EALLOW;
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM中断
    PieVectTable.SCITXINTA=&sciaTx_isr; //SCI发送中断
    PieVectTable.SCIRXINTA=&sciaRx_isr; //SCI接收中断
    PieVectTable.TINT0=&cpu_timer0_isr; //Timer0中断
	EDIS;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM使用INT3
    IER|=M_INT9; //SCIA
	IER|=M_INT1; //Timer0
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //PWM
    PieCtrlRegs.PIEIER9.bit.INTx1=1; //SCIA_RX_INT
    PieCtrlRegs.PIEIER9.bit.INTx2=1; //SCIA_TX_INT
    PieCtrlRegs.PIEIER1.bit.INTx7=1; //Timer0

    //各种模块初始化
    ADCInit(); //ADC初始化
    InitEPwm1();//EPwm1初始化
    InitEPwm2();//EPwm2初始化
    InitEPwm1Gpio();//EPwm1 GPIO初始化
    InitEPwm2Gpio();//EPwm2 GPIO初始化
//    InitFIR(); //FIR滤波器初始化
    Gpio_setup1(); //GPIO初始化设置
    Gpio_setup2(); //GPIO初始化设置
    InitSCI(); //SCI初始化
    TimerInit(); //定时器初始化
    CalculateInit(); //参数初始化
    //初始化完成

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //重启TBCLK
    EDIS;
    EINT;
    ERTM;
    number=0;


    while(1)
    {
    	if(flag_DataSend==1)
    	{
    		TransData(i[k]);
//    		sci_send(255);
//    		sci_send(input_backL);
//   		    sci_send(input_backH);
//    		sci_send(input_forwardL);
//    		sci_send(input_forwardH);
    		flag_DataSend=0;
    	}


//    	if(GpioDataRegs.GPCDAT.bit.GPIO67==0)
//    	{
//    		DELAY_US(50000L);
//    		if(GpioDataRegs.GPCDAT.bit.GPIO67==0)
//    			while(GpioDataRegs.GPCDAT.bit.GPIO67==0);
//    		mode=9;
//    	}


/*    	if(GpioDataRegs.GPCDAT.bit.GPIO68==0)
    	{
    		DELAY_US(50000L);
    		if(GpioDataRegs.GPCDAT.bit.GPIO68==0)
    			while(GpioDataRegs.GPCDAT.bit.GPIO68==0);
    		Amp2=Amp2-0.005;
    	}
*/

     	if(flag_PWMEnable==0)
    	{
    		EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    		EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    	}
    	else
    	{
    		 EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    		 EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //禁止强制输出
    	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM1中断，负责运算
interrupt void epwm1_isr(void)
{
	if(mode!=mode_1)
		ADCReInit(mode); //ADC初始化
	ReadADC(adcresults,mode);
	i_cal();
	vg_cal();
	switch(mode)
	{
	case -1:
		GridImitate();
		break;
	default:
		P_cal();
		Q_cal();
		EPwm1Regs.CMPA.half.CMPA =(e_pwm/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD2; //如两个都为(e/100+0.5)*EPwm_TIMER_TBPRD则 A通道给1，4，B通道给2，3，双极型调制
		EPwm2Regs.CMPA.half.CMPA=(-e_pwm/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD2; //如两个为(+-e/100+0.5)*EPwm_TIMER_TBPRD则 A通道给1，2，B通道给3，4，但极性调制
//   	flag_DataSend=TransControl();
		flag_DataSend=1;
		break;
	}
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer0中断，负责开关位置判断，开关动作必须在三秒内到位，并按照特定顺序进行
interrupt void cpu_timer0_isr(void)
{
	if(GpioDataRegs.GPCDAT.bit.GPIO69==1)
		SelfProtect();

	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}



