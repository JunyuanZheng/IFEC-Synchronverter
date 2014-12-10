#include "DSP28x_Project.h"
#include "PWM_Header.h"
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//声明定义本函数变量
Uint16 flag_PWMEnable=0; //各种控制标志位
Uint16 mode_1=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//测试变量
float32 Pmean,Qmean;
float32 sample0[500];
int32 sample1[500];
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
	PieVectTable.EPWM1_INT=&epwm1_timer_isr; //ePWM中断
	EDIS;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM使用INT3
	PieCtrlRegs.PIEIER3.bit.INTx1=1;
    //各种模块初始化
    InitEPwm1();//EPwm1初始化
    InitEPwm2();//EPwm2初始化
    InitEPwm1Gpio();//EPwm1 GPIO初始化
    InitEPwm2Gpio();//EPwm2 GPIO初始化
    //初始化完成
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //重启TBCLK
    EDIS;
    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //强制输出低电平
    ADCInit(); //ADC初始化
    CalculateInit(); //参数初始化
    EINT;
    ERTM;
    while(1)
    {
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
interrupt void epwm1_timer_isr(void)
{
	static Uint16 count=0;
	vg_sample(); //可能会有问题 vg_sample,P_cal以及Q_cal的函数位置需要设计
	switch(mode)
	{
	case 0: //自同步模式
		Dp=0.56;
		J=3e-5;
		I_PI=20;
		K=1092.77;
		P_set=0;
		Q_set=0;
		Pset_cal();
		Qset_cal();
		mode_1=0;
		break;
	case 1: //Pd Qset模式
		J=2e-3;
		I_PI=10;
		K=1092.77;
		P_set=9;
		Pd_cal();
		Qset_cal();
		mode_1=1;
		break;
	case 2: //Pd Qset模式
		J=3e-5;
		K=5000;
		P_set=500;
		vg_rms=Vg_RMS(e);
		Q_set=0;
		Pd_cal();
		Qd_cal();
		mode_1=2;
		break;
	}
	i_sample();  //可能会有问题
	EPwm1Regs.CMPA.half.CMPA=(e/760.0+0.5)*EPwm1_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=(-e/760.0+0.5)*EPwm2_TIMER_TBPRD;
	P=Te*w;
	sample0[count]=Mfif;
	sample1[count]=e;
	count++;
	if(count==500)
		count=0;
	Pmean=P_Mean(P);
	Qmean=Q_Mean(Q);
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}



