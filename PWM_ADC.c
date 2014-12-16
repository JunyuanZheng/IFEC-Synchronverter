#include "DSP28x_Project.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void ADCInit(void);
void ReadADC(unsigned int *p);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC相关设置
void ADCInit(void)//ADC初始化设置
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
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1=0; //最大采样值为此值+1
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1=1;//使能ePWM_SOCA启动信号
	AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5;//采样1个通道
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC读取结果
void ReadADC(unsigned int *p)//ADC读取数据
{
	while(AdcRegs.ADCST.bit.SEQ1_BSY==1);
	*p=AdcRegs.ADCRESULT0>>4;
	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
	AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;
}
