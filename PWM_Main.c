#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//声明本函数需要的外部变量
extern Uint16 mode; //三个开关
extern float32 vg,vg_rms,i,e; //采集量
extern float32 P_set,Q_set,Mfif,w; //P环参数
extern float32 angle;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//变量
Uint16 flag_DataSend=0;
Uint16 flag_PWMEnable=0;
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
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM中断
    PieVectTable.SCITXINTA=&sciaTx_isr;
    PieVectTable.SCIRXINTA=&sciaRx_isr;
	EDIS;
	IER|=M_INT3; //使能CPU的INT3中断，ePWM使用INT3
    IER|=M_INT9; //SCIA
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //PWM
    PieCtrlRegs.PIEIER9.bit.INTx1=1; //SCIA_RX_INT
    PieCtrlRegs.PIEIER9.bit.INTx2=1; //SCIA_TX_INT

    //各种模块初始化
    ADCInit(); //ADC初始化
    InitEPwm1();//EPwm1初始化
    InitEPwm2();//EPwm2初始化
    InitEPwm1Gpio();//EPwm1 GPIO初始化
    InitEPwm2Gpio();//EPwm2 GPIO初始化
    InitSCI(); //SCI初始化
    InitFIR(); //FIR滤波器初始化
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
    		TransData(e-vg);
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
	P_cal();
	Q_cal();
	i_sample();  //可能会有问题
	EPwm1Regs.CMPA.half.CMPA=(e/760+0.5)*EPwm_TIMER_TBPRD; //
	EPwm2Regs.CMPA.half.CMPA=(-e/760+0.5)*EPwm_TIMER_TBPRD; //
//	flag_DataSend=TransControl();
	flag_DataSend=1;
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}



