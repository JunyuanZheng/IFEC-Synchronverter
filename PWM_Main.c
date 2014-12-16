#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������������Ҫ���ⲿ����
extern float32 vg,i,vg_rms; //vg,igΪ�ɼ���������Ƶ��10khz��VgΪvg��Чֵ��
extern Uint16 mode; //��������
extern float32 Dp,J,I_PI,K;//���Ʋ�����
extern float32 Mfif,Mfif_compen,Mfif_cal,Q_1;
extern float32 Tm,T_sum,Te,Te_1,dT,dT_IOutput;
extern float32 P,Te,w;
extern float32 e;
extern float32 P_set,Q_set,Q;
extern Uint16 data[8];

extern Uint16 flag_PWMEnable=0; //���ֿ��Ʊ�־λ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���Ա���
float32 Pmean,Qmean;
Uint16  flag_DataSend=0;
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
//    PieVectTable.TINT0=&cpu_timer0_isr; //Timer0�ж�
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM�ж�
    PieVectTable.SCITXINTA=&SCITxA_isr; //SCIA�����ж�
	EDIS;
//    IER|=M_INT1;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWMʹ��INT3
    IER|=M_INT9;
//    PieCtrlRegs.PIEIER1.bit.INTx7=1; //Timer0
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //PWM
    PieCtrlRegs.PIEIER9.bit.INTx1=1; //SCI
    PieCtrlRegs.PIEIER9.bit.INTx2=1; //SCI

    //����ģ���ʼ��
    ADCInit(); //ADC��ʼ��
    InitEPwm1();//EPwm1��ʼ��
    InitEPwm2();//EPwm2��ʼ��
    InitEPwm1Gpio();//EPwm1 GPIO��ʼ��
    InitEPwm2Gpio();//EPwm2 GPIO��ʼ��
    SCIAInit();
//    TimerInit();
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
    	if(flag_DataSend==1)
    	{
    		TransData(w/(2*pie));
    		flag_DataSend=0;
    	}
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
	vg_sample(); //���ܻ������� vg_sample,P_cal�Լ�Q_cal�ĺ���λ����Ҫ���
	switch(mode)
	{
	case 0: //��ͬ��ģʽ
		P_set=0;
		Q_set=0;
		Pset_cal();
		Qset_cal();
		break;
	case 1: //Pd Qsetģʽ
		P_set=500;
		vg_rms=Vg_RMS(e);
		Q_set=0;
		Pd_cal();
		Qd_cal();
		break;
	}
	i_sample();  //���ܻ�������
	EPwm1Regs.CMPA.half.CMPA=(e/760+0.5)*EPwm_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=(-e/760+0.5)*EPwm_TIMER_TBPRD;
	flag_DataSend=1;
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer0�жϣ���������
/*interrupt void cpu_timer0_isr(void)
{
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}
*/



