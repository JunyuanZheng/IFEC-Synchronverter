#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������������Ҫ���ⲿ����
extern Uint16 mode; //��������
extern float32 vg,vg_rms,i,e; //�ɼ���
extern float32 P_set,Q_set,Mfif,w; //P������
extern float32 angle;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
Uint16 flag_DataSend=0;
Uint16 flag_PWMEnable=0;
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
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM�ж�
    PieVectTable.SCITXINTA=&sciaTx_isr;
    PieVectTable.SCIRXINTA=&sciaRx_isr;
	EDIS;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWMʹ��INT3
    IER|=M_INT9; //SCIA
	PieCtrlRegs.PIEIER3.bit.INTx1=1; //PWM
    PieCtrlRegs.PIEIER9.bit.INTx1=1; //SCIA_RX_INT
    PieCtrlRegs.PIEIER9.bit.INTx2=1; //SCIA_TX_INT

    //����ģ���ʼ��
    ADCInit(); //ADC��ʼ��
    InitEPwm1();//EPwm1��ʼ��
    InitEPwm2();//EPwm2��ʼ��
    InitEPwm1Gpio();//EPwm1 GPIO��ʼ��
    InitEPwm2Gpio();//EPwm2 GPIO��ʼ��
    InitSCI(); //SCI��ʼ��
    InitFIR(); //FIR�˲�����ʼ��
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
    		TransData(e-vg);
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
	P_cal();
	Q_cal();
	i_sample();  //���ܻ�������
	EPwm1Regs.CMPA.half.CMPA=(e/760+0.5)*EPwm_TIMER_TBPRD; //
	EPwm2Regs.CMPA.half.CMPA=(-e/760+0.5)*EPwm_TIMER_TBPRD; //
//	flag_DataSend=TransControl();
	flag_DataSend=1;
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}



