#include "DSP28x_Project.h"
#include "PWM_Header.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������������Ҫ���ⲿ����
extern float32 vg,i,vg_rms; //vg,igΪ�ɼ���������Ƶ��10khz��VgΪvg��Чֵ��
extern Uint16 mode; //��������
extern float32 Dp,J,I_PI,K;//���Ʋ�����
extern float32 P,Te,W;
extern float32 e;
extern float32 P_set,Q_set,Q;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������屾��������
Uint16 flag_SysEnable=0,flag_PWMEnable=0; //���ֿ��Ʊ�־λ
float32 Ig=0; //ig����Чֵ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���Ա���

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
	PieVectTable.TINT0=&cpu_timer0_isr; //tmer0�ж�
	EDIS;
	IER|=M_INT1; //ʹ��CPU��INT1�жϣ�Timer0ʹ��INT1
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWMʹ��INT3
	PieCtrlRegs.PIEIER1.bit.INTx7=1; //TINT0��PIE��1�е���λ
	PieCtrlRegs.PIEIER3.bit.INTx1=1;
    //����ģ���ʼ��
	TimerInit(); //��ʱ����ʼ��
    InitEPwm1();//EPwm1��ʼ��
    InitEPwm2();//EPwm2��ʼ��
    InitEPwm1Gpio();//EPwm1 GPIO��ʼ��
    InitEPwm2Gpio();//EPwm2 GPIO��ʼ��
    //��ʼ�����
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //����TBCLK
    EDIS;
    EPwm1Regs.AQCSFRC.all=PWMS_ALBL;//ǿ������͵�ƽ
    EPwm2Regs.AQCSFRC.all=PWMS_ALBL;//ǿ������͵�ƽ
    ADCInit();//ADC��ʼ��
    CalculateInit();//������ʼ��
    EINT;
    ERTM;
    while(1)
    {
    	if(flag_PWMEnable==0)
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ePWM1�жϣ���������
interrupt void epwm1_timer_isr(void)
{
	if(flag_SysEnable==1) //�����Ż�
	{
		switch(mode)
		{
		case 0: //��ͬ��ģʽ
			I_PI=10;
			K=1000000;
			P_set=0;
			Q_set=0;
			Pset_cal();
			Qset_cal();
			break;
		case 1: //Pset Qsetģʽ
			I_PI=10;
			K=1000000;
			P_set=4.5;
			Pset_cal();
			Qset_cal();
			break;
		}
	}
	EPwm1Regs.CMPA.half.CMPA=(e/100.0+0.5)*EPwm1_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=(-e/100.0+0.5)*EPwm2_TIMER_TBPRD;
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer0�жϣ��������
interrupt void cpu_timer0_isr(void) //1khz����
{
	vg=vg_sample();
	vg_rms=Vg_RMS(vg);
	i=i_sample();
	Ig=I_RMS(i);
	if(vg_rms!=0)
		flag_SysEnable=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}





