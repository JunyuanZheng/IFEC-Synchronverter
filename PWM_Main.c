#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/*mode说明
 * -2只用于mode_1,用于第一次ADCReInit
 * -1用于检测电流过零点
 * 0 用于自同步
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//声明本函数需要的外部变量
//ADC结果储存
extern Uint16 adcresults[6];
//外部变量
extern int16 mode,mode_1; //三个开关
extern float32 e,vg; //采集量
extern float32 P_set,P,Q,Q_set,w; //PQ环参数
extern Uint16 flag_PWMEnable; //控制脉冲波
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本函数测试变量
Uint16 flag_DataSend=0;
int16 input_back,input_forward,input_backL,input_backH,input_forwardL,input_forwardH;
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
//  MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//  InitFlash();
    DELAY_US(50000L); //延时50ms，等待其他模块完成初始化

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //配置ePWM前关闭TBCLK
    EDIS;
    EALLOW;
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM中断
    PieVectTable.SCITXINTA=&sciaTx_isr;
    PieVectTable.SCIRXINTA=&sciaRx_isr;
    PieVectTable.TINT0=&cpu_timer0_isr;
//  PieVectTable.XINT13=&cpu_timer1_isr;
	EDIS;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM使用INT3
    IER|=M_INT9; //SCIA
	IER|=M_INT1; //Timer0
//	IER|=M_INT13; //Timer1
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
    InitFIR(); //FIR滤波器初始化
    Gpio_setup();
    InitSCI(); //SCI初始化
    TimerInit(); //定时器初始化
    CalculateInit(); //参数初始化
    //初始化完成

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //重启TBCLK
    EDIS;
    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平

    EINT;
    ERTM;
    while(1)
    {
/*    	if(flag_DataSend==1)
    	{
    		TransData(vg);
    		sci_send(input_backL);
    		sci_send(input_backH);
    		sci_send(input_forwardL);
    		sci_send(input_forwardH);
    		flag_DataSend=0;
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
	switch(mode)
	{
	default:
		ReadADC(adcresults,mode);
		sample();
		P_cal();
		Q_cal();
		EPwm1Regs.CMPA.half.CMPA=(e/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD; //如两个都为(e/100+0.5)*EPwm_TIMER_TBPRD则 A通道给1，4，B通道给2，3，双极型调制
		EPwm2Regs.CMPA.half.CMPA=(-e/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD; //如两个为(+-e/100+0.5)*EPwm_TIMER_TBPRD则 A通道给1，2，B通道给3，4，但极性调制
//	    flag_DataSend=TransControl();
//		flag_DataSend=1;
	}
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer0中断，负责计算vg零点
/*interrupt void cpu_timer0_isr(void)
{
	static float32 vgSup_1=0,vgSup=0;

//	if(mode!=mode_1)
//		ADCReInit(mode); //为单独采样vg进行ADC初始化
	ReadADC(adcresults,mode); //读取adc结果
	vgSup=(6.0*(adcresults[0]-10)/4095.0-3.0)/156.6667*0.4*136*1000; //结果转换

	if((vgSup_1<-2)&&(vgSup>2)) //判断是否过零，过零后模式切换，定时器和定时器中断关闭
	{
		mode=0;
	    CpuTimer0Regs.TCR.all=0x0008;
	}
	else
	{
		vgSup_1=vgSup;
		mode=-1;
		mode_1=-1; //追踪电网相位的mode为-1
		PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
	}
}
*/
interrupt void cpu_timer0_isr(void)
{
	if(GpioDataRegs.GPCDAT.bit.GPIO67==1)
		flag_PWMEnable=1;
	else
		flag_PWMEnable=0;

	if(GpioDataRegs.GPCDAT.bit.GPIO68==1)
	{
		GpioDataRegs.GPASET.bit.GPIO4=1;
		GpioDataRegs.GPASET.bit.GPIO5=1;
	}
	else
	{
		GpioDataRegs.GPACLEAR.bit.GPIO4=1;
		GpioDataRegs.GPACLEAR.bit.GPIO5=1;
	}
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer1中断，确定开关状态
//interrupt void cpu_timer1_isr(void)
//{
//	EDIS;
//}
//}



