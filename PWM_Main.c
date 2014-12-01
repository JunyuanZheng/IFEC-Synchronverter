#include "DSP28x_Project.h"
#include "PWM_Header.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������ı���
extern float32 vg,i,Vg; //vg,igΪ�ɼ���������Ƶ��10khz��VgΪvg��Чֵ��
extern Uint16 Sp,Sq,Sc; //��������
extern float32 e;
extern float32 E;
float32 Ig=0.0;
Uint16 ADCResults[1]={0}; //ADC�������
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
interrupt void cpu_timer0_isr(void) //1khz����
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





