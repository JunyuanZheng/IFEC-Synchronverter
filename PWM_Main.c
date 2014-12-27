#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/*mode˵��
 * -2ֻ����mode_1,���ڵ�һ��ADCReInit
 * -1���ڼ����������
 * 0 ������ͬ��
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������������Ҫ���ⲿ����
//ADC�������
extern Uint16 adcresults[6];
//�ⲿ����
extern int16 mode,mode_1; //��������
extern float32 e,vg; //�ɼ���
extern float32 P_set,P,Q,Q_set,w; //PQ������
extern Uint16 flag_PWMEnable; //�������岨
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���������Ա���
Uint16 flag_DataSend=0;
int16 input_back,input_forward,input_backL,input_backH,input_forwardL,input_forwardH;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������
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
    DELAY_US(50000L); //��ʱ50ms���ȴ�����ģ����ɳ�ʼ��

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //����ePWMǰ�ر�TBCLK
    EDIS;
    EALLOW;
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM�ж�
    PieVectTable.SCITXINTA=&sciaTx_isr;
    PieVectTable.SCIRXINTA=&sciaRx_isr;
    PieVectTable.TINT0=&cpu_timer0_isr;
//  PieVectTable.XINT13=&cpu_timer1_isr;
	EDIS;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWMʹ��INT3
    IER|=M_INT9; //SCIA
	IER|=M_INT1; //Timer0
//	IER|=M_INT13; //Timer1
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //PWM
    PieCtrlRegs.PIEIER9.bit.INTx1=1; //SCIA_RX_INT
    PieCtrlRegs.PIEIER9.bit.INTx2=1; //SCIA_TX_INT
    PieCtrlRegs.PIEIER1.bit.INTx7=1; //Timer0

    //����ģ���ʼ��
    ADCInit(); //ADC��ʼ��
    InitEPwm1();//EPwm1��ʼ��
    InitEPwm2();//EPwm2��ʼ��
    InitEPwm1Gpio();//EPwm1 GPIO��ʼ��
    InitEPwm2Gpio();//EPwm2 GPIO��ʼ��
    InitFIR(); //FIR�˲�����ʼ��
    Gpio_setup();
    InitSCI(); //SCI��ʼ��
    TimerInit(); //��ʱ����ʼ��
    CalculateInit(); //������ʼ��
    //��ʼ�����

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //����TBCLK
    EDIS;
    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ

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
    		EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    		EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    	}
    	else
    	{
    		 EPwm1Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    		 EPwm2Regs.AQCSFRC.all=PWMS_FRC_DISABLE; //��ֹǿ�����
    	}
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM1�жϣ���������
interrupt void epwm1_isr(void)
{
	if(mode!=mode_1)
		ADCReInit(mode); //ADC��ʼ��
	switch(mode)
	{
	default:
		ReadADC(adcresults,mode);
		sample();
		P_cal();
		Q_cal();
		EPwm1Regs.CMPA.half.CMPA=(e/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD; //��������Ϊ(e/100+0.5)*EPwm_TIMER_TBPRD�� Aͨ����1��4��Bͨ����2��3��˫���͵���
		EPwm2Regs.CMPA.half.CMPA=(-e/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD; //������Ϊ(+-e/100+0.5)*EPwm_TIMER_TBPRD�� Aͨ����1��2��Bͨ����3��4�������Ե���
//	    flag_DataSend=TransControl();
//		flag_DataSend=1;
	}
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer0�жϣ��������vg���
/*interrupt void cpu_timer0_isr(void)
{
	static float32 vgSup_1=0,vgSup=0;

//	if(mode!=mode_1)
//		ADCReInit(mode); //Ϊ��������vg����ADC��ʼ��
	ReadADC(adcresults,mode); //��ȡadc���
	vgSup=(6.0*(adcresults[0]-10)/4095.0-3.0)/156.6667*0.4*136*1000; //���ת��

	if((vgSup_1<-2)&&(vgSup>2)) //�ж��Ƿ���㣬�����ģʽ�л�����ʱ���Ͷ�ʱ���жϹر�
	{
		mode=0;
	    CpuTimer0Regs.TCR.all=0x0008;
	}
	else
	{
		vgSup_1=vgSup;
		mode=-1;
		mode_1=-1; //׷�ٵ�����λ��modeΪ-1
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
//Timer1�жϣ�ȷ������״̬
//interrupt void cpu_timer1_isr(void)
//{
//	EDIS;
//}
//}



