#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//声明本函数需要的外部变量
extern float32 vg,i,vg_rms; //vg,ig为采集量，采样频率10khz，Vg为vg有效值，
extern Uint16 mode; //三个开关
extern float32 Dp,J,I_PI,K;//控制参数，
extern float32 Mfif,Mfif_compen,Mfif_cal,Q_1;
extern float32 Tm,T_sum,Te,Te_1,dT,dT_IOutput;
extern float32 P,Te,w;
extern float32 e;
extern float32 P_set,Q_set,Q;
extern Uint16 data[8];

extern Uint16 flag_PWMEnable=0; //各种控制标志位
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//测试变量
float32 Pmean,Qmean;
Uint16  flag_DataSend=0;
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
    DELAY_US(50000L); //延时50ms，等待其他模块完成初始化

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //配置ePWM前关闭TBCLK
    EDIS;
    EALLOW;
//    PieVectTable.TINT0=&cpu_timer0_isr; //Timer0中断
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM中断
    PieVectTable.SCITXINTA=&SCITxA_isr; //SCIA发送中断
	EDIS;
//    IER|=M_INT1;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM使用INT3
    IER|=M_INT9;
//    PieCtrlRegs.PIEIER1.bit.INTx7=1; //Timer0
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //PWM
    PieCtrlRegs.PIEIER9.bit.INTx1=1; //SCI
    PieCtrlRegs.PIEIER9.bit.INTx2=1; //SCI

    //各种模块初始化
    ADCInit(); //ADC初始化
    InitEPwm1();//EPwm1初始化
    InitEPwm2();//EPwm2初始化
    InitEPwm1Gpio();//EPwm1 GPIO初始化
    InitEPwm2Gpio();//EPwm2 GPIO初始化
    SCIAInit();
//    TimerInit();
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
    	if(flag_DataSend==1)
    	{
    		TransData(w/(2*pie));
    		flag_DataSend=0;
    	}
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
	vg_sample(); //可能会有问题 vg_sample,P_cal以及Q_cal的函数位置需要设计
	switch(mode)
	{
	case 0: //自同步模式
		P_set=0;
		Q_set=0;
		Pset_cal();
		Qset_cal();
		break;
	case 1: //Pd Qset模式
		P_set=500;
		vg_rms=Vg_RMS(e);
		Q_set=0;
		Pd_cal();
		Qd_cal();
		break;
	}
	i_sample();  //可能会有问题
	EPwm1Regs.CMPA.half.CMPA=(e/760+0.5)*EPwm_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=(-e/760+0.5)*EPwm_TIMER_TBPRD;
	flag_DataSend=1;
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer0中断，发送数据
/*interrupt void cpu_timer0_isr(void)
{
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}
*/



