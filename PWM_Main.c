#include "DSP28x_Project.h"
#include "PWM_Header.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本函数的变量
extern float32 vg,i,Vg; //vg,ig为采集量，采样频率10khz，Vg为vg有效值，
extern Uint16 Sp,Sq,Sc; //三个开关
extern float32 e;
extern float32 E;
float32 Ig=0.0;
Uint16 ADCResults[1]={0}; //ADC结果储存
Uint16 SysEnable_flag=0,flag=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	PieVectTable.TINT0=&cpu_timer0_isr; //tmer0中断
	EDIS;
	IER|=M_INT1; //使能CPU的INT1中断，Timer0使用INT1
	IER|=M_INT3; //使能CPU的INT3中断，ePWM使用INT3
	PieCtrlRegs.PIEIER1.bit.INTx7=1; //TINT0与PIE组1中第七位
	PieCtrlRegs.PIEIER3.bit.INTx1=1;
    //各种模块初始化
	TimerInit(); //定时器初始化
    InitEPwm1();//EPwm1初始化
    InitEPwm2();//EPwm2初始化
    InitEPwm1Gpio();//EPwm1 GPIO初始化
    InitEPwm2Gpio();//EPwm2 GPIO初始化
    //初始化完成
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //重启TBCLK
    EDIS;
    EPwm1Regs.AQCSFRC.all=PWMS_ALBL;//强制输出低电平
    EPwm2Regs.AQCSFRC.all=PWMS_ALBL;//强制输出低电平
    ADCInit();//ADC初始化
    CalculateInit();//参数初始化
    EINT;
    ERTM;
    while(1)
    {
    	if(SysEnable_flag==0)
    	{
    		 EPwm1Regs.AQCSFRC.all=PWMS_ALBL;
    		 EPwm2Regs.AQCSFRC.all=PWMS_ALBL;
    	}
    	else
    	{
    		 EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE;
    		 EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE;
    	}
    }
}
interrupt void epwm1_timer_isr(void)
{
	if(SysEnable_flag==1)
	{
		if(Sp==1)
		{
			if(Sq==1)
			{
				Pset_cal();
				Qd_cal();
			}
			else
			{
				Pset_cal();
				Qset_cal();
			}
		}
		else
		{
			if(Sq==1)
			{
				Pd_cal();
				Qd_cal();
			}
			else
			{
				Pd_cal();
				Qset_cal();
			}
		}

	}
	EPwm1Regs.CMPA.half.CMPA=(e/100.0+0.5)*EPwm1_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=(-e/100.0+0.5)*EPwm2_TIMER_TBPRD;
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
interrupt void cpu_timer0_isr(void) //1khz采样
{
	vg=vg_sample();
	Vg=Vg_RMS(vg);
	i=i_sample();
	Ig=Ig_RMS(i);
	E=E_RMS(e);
	if(Vg!=0)
		SysEnable_flag=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}





