#include "DSP28x_Project.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC�������
void ADCInit()//ADC��ʼ������
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
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1=1;//ʹ��ePWM_SOCA�����ź�
}
//void ADCRectify(int16 mode)
//{
//
//}
void ADCReInit(int16 mode)//ADC��ʼ������
{
	switch(mode)
	{
	case -1:
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=0;
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5;
//		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i������4ͨ��
		break;
	case 0: //��ͬ��ʱ��Ҫ����vg
		EPwm1Regs.TBPRD=EPwm_TIMER_TBPRD2;
		EPwm2Regs.TBPRD=EPwm_TIMER_TBPRD2;
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=0; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg������5ͨ��
		break;
	case 1: //Pset,Qset��Ҫ����vg,is
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg������5ͨ��
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i������4ͨ��
	case 2: //Pd,Qd��Ҫ����vg,is
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg������5ͨ��
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i������4ͨ��
	case 3: //Pset,Qd��Ҫ����vg��is
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg������5ͨ��
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i������4ͨ��
		break;
	case 4:
		AdcRegs.ADCMAXCONV.bit.MAX_CONV1=1; //������ֵΪ��ֵ+1
		AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x5; //vg������5ͨ��
		AdcRegs.ADCCHSELSEQ1.bit.CONV01=0x4; //i������4ͨ��
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC��ȡ���
void ReadADC(unsigned int *p,int16 mode)//ADC��ȡ����
{
	while(AdcRegs.ADCST.bit.SEQ1_BSY==1);
	switch(mode)
	{
	case -1:
		*p=AdcRegs.ADCRESULT0>>4;
//		*(p+1)=AdcRegs.ADCRESULT1>>4;
		break;
	case 0: //��vg������[0]��i����[1]
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


