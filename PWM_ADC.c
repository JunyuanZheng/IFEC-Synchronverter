#include "DSP28x_Project.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC相关设置
void ADCInit()//ADC初始化设置
{
	EALLOW;
	#if (CPU_FRQ_150MHZ)   // Default - 150 MHz SYSCLKOUT
	#define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
	#endif
	#if (CPU_FRQ_100MHZ)
	#define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
	#endif
	EDIS;
	InitAdc();
	EALLOW;
	SysCtrlRegs.HISPCP.all=ADC_MODCLK;
	EDIS;
    AdcRegs.ADCTRL3.bit.ADCCLKPS=0x1;
	AdcRegs.ADCTRL1.bit.ACQ_PS=0xf;
	AdcRegs.ADCTRL1.bit.SEQ_CASC=1;
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1=1;//使能ePWM_SOCA启动信号
}
//void ADCRectify(int16 mode)
//{
//
//}
void ADCReInit(int16 mode)//ADC初始化设置
{
	switch(mode)
	{
	case -1:
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=0;
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5;
//		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i采样在4通道
		break;
	case 0: //自同步时需要采样vg
		EPwm1Regs.TBPRD=EPwm_TIMER_TBPRD2;
		EPwm2Regs.TBPRD=EPwm_TIMER_TBPRD2;
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=0; //最大采样值为此值+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg采样在5通道
		break;
	case 1: //Pset,Qset需要采样vg,is
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //最大采样值为此值+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg采样在5通道
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i采样在4通道
	case 2: //Pd,Qd需要采样vg,is
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //最大采样值为此值+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg采样在5通道
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i采样在4通道
	case 3: //Pset,Qd需要采样vg，is
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //最大采样值为此值+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg采样在5通道
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i采样在4通道
		break;
	case 4:
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //最大采样值为此值+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg采样在5通道
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i采样在4通道
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC读取结果
void ReadADC(unsigned int *p,int16 mode)//ADC读取数据
{
	while(AdcRegs.ADCST.bit.SEQ1_BSY==1);
	switch(mode)
	{
	case -1:
		*p=AdcRegs.ADCRESULT0>>4;
//		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	case 0: //将vg储存至[0]，i存至[1]
		*p=AdcRegs.ADCRESULT0>>4;
		break;
	case 1:
		*p=AdcRegs.ADCRESULT0>>4;
		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	case 2:
		*p=AdcRegs.ADCRESULT0>>4;
		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	case 3:
		*p=AdcRegs.ADCRESULT0>>4;
		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	case 4:
		*p=AdcRegs.ADCRESULT0>>4;
		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	}
	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;
}


