#include "DSP28x_Project.h"
#include "PWM_Header.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������������Ҫ���ⲿ����
extern float32 vg,i,vg_rms; //vg,igΪ�ɼ���������Ƶ��10khz��VgΪvg��Чֵ��
extern Uint16 mode; //��������
extern float32 Dp,J,I_PI,K;//���Ʋ�����
extern float32 P,Te,w;
extern float32 e;
extern float32 P_set,Q_set,Q;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������屾��������
Uint16 flag_PWMEnable=0; //���ֿ��Ʊ�־λ
float32 Ig=0; //ig����Чֵ
extern float32 Mfif,Mfif_compen,Mfif_cal,Q_1;
extern float32 Tm,T_sum,Te,Te_1,dT,dT_IOutput;
Uint16 mode_1=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���Ա���
float32 Pmean;
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
    DELAY_US(50000L); //��ʱ50ms���ȴ�����ģ����ɳ�ʼ��
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //����ePWMǰ�ر�TBCLK
    EDIS;
    EALLOW;
	PieVectTable.EPWM1_INT=&epwm1_timer_isr; //ePWM�ж�
	EDIS;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWMʹ��INT3
	PieCtrlRegs.PIEIER3.bit.INTx1=1;
    //����ģ���ʼ��
    InitEPwm1();//EPwm1��ʼ��
    InitEPwm2();//EPwm2��ʼ��
    InitEPwm1Gpio();//EPwm1 GPIO��ʼ��
    InitEPwm2Gpio();//EPwm2 GPIO��ʼ��
    //��ʼ�����
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //����TBCLK
    EDIS;
    EPwm1Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    EPwm2Regs.AQCSFRC.all=PWMS_ALBL; //ǿ������͵�ƽ
    ADCInit(); //ADC��ʼ��
    CalculateInit(); //������ʼ��
    EINT;
    ERTM;
    while(1)
    {
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
interrupt void epwm1_timer_isr(void)
{
	vg_sample(); //���ܻ������� vg_sample,P_cal�Լ�Q_cal�ĺ���λ����Ҫ���
	i_sample();  //���ܻ�������
	Ig=I_RMS(i);
	if(mode_1!=mode) //�������֣�ģʽת�������в��ֲ�������
		Q_1=0.0;
		Mfif_cal=0;
		Mfif_compen=30*sqrt(2)/100.0/pie;
		Mfif=Mfif_cal+Mfif_compen;
	switch(mode)
	{
	case 0: //��ͬ��ģʽ
		Dp=0.56;
		J=2e-3;
		I_PI=20;
		K=1092.77;
		P_set=0;
		Q_set=0;
		Pset_cal();
		Qset_cal();
		mode_1=0;
		break;
	case 1: //Pset Qsetģʽ
		J=2e-3;
		I_PI=10;
		K=1092.77;
		P_set=9;
		Pd_cal();
		Qset_cal();
		mode_1=1;
		break;
	}
	EPwm1Regs.CMPA.half.CMPA=(e/100.0+0.5)*EPwm1_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=(-e/100.0+0.5)*EPwm2_TIMER_TBPRD;
	P=Te*w;
	Pmean=P_Mean(P);
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}



