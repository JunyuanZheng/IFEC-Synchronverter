#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/*mode˵��
 * -2 ֻ����mode_1,���ڵ�һ��ADCReInit
 * -1 ��ʼ���������ұ�
 * 0 ������ͬ��
 * 9 ���ڲ���Pset��Qset
 *
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������������Ҫ���ⲿ����
//�����Ա���
extern Uint16 adcresults[6];
//�����Ա���
extern float32 e_pwm;
//�����Ա���
extern int16 mode,mode_1; //ģʽ����
extern Uint16 flag_PWMEnable; //�������岨
Uint16 flag_DataSend=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���������Ա���
int16 input_back,input_forward,input_backL,input_backH,input_forwardL,input_forwardH;
Uint16 number;
extern float32 w,vg,e,ig,Q,Te,i[2],Mfif; //�ɼ���
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
//    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
//    InitFlash();
    DELAY_US(50000L); //��ʱ50ms���ȴ�����ģ����ɳ�ʼ��

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0; //����ePWMǰ�ر�TBCLK
    EDIS;
    EALLOW;
	PieVectTable.EPWM1_INT=&epwm1_isr; //ePWM�ж�
    PieVectTable.SCITXINTA=&sciaTx_isr; //SCI�����ж�
    PieVectTable.SCIRXINTA=&sciaRx_isr; //SCI�����ж�
    PieVectTable.TINT0=&cpu_timer0_isr; //Timer0�ж�
	EDIS;
	IER|=M_INT3; //ʹ��CPU��INT3�жϣ�ePWMʹ��INT3
    IER|=M_INT9; //SCIA
	IER|=M_INT1; //Timer0
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
//    InitFIR(); //FIR�˲�����ʼ��
    Gpio_setup1(); //GPIO��ʼ������
    Gpio_setup2(); //GPIO��ʼ������
    InitSCI(); //SCI��ʼ��
    TimerInit(); //��ʱ����ʼ��
    CalculateInit(); //������ʼ��
    //��ʼ�����

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1; //����TBCLK
    EDIS;
    EINT;
    ERTM;
    number=0;


    while(1)
    {
    	if(flag_DataSend==1)
    	{
    		TransData(i[k]);
//    		sci_send(255);
//    		sci_send(input_backL);
//   		    sci_send(input_backH);
//    		sci_send(input_forwardL);
//    		sci_send(input_forwardH);
    		flag_DataSend=0;
    	}


//    	if(GpioDataRegs.GPCDAT.bit.GPIO67==0)
//    	{
//    		DELAY_US(50000L);
//    		if(GpioDataRegs.GPCDAT.bit.GPIO67==0)
//    			while(GpioDataRegs.GPCDAT.bit.GPIO67==0);
//    		mode=9;
//    	}


/*    	if(GpioDataRegs.GPCDAT.bit.GPIO68==0)
    	{
    		DELAY_US(50000L);
    		if(GpioDataRegs.GPCDAT.bit.GPIO68==0)
    			while(GpioDataRegs.GPCDAT.bit.GPIO68==0);
    		Amp2=Amp2-0.005;
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
	ReadADC(adcresults,mode);
	i_cal();
	vg_cal();
	switch(mode)
	{
	case -1:
		GridImitate();
		break;
	default:
		P_cal();
		Q_cal();
		EPwm1Regs.CMPA.half.CMPA =(e_pwm/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD2; //��������Ϊ(e/100+0.5)*EPwm_TIMER_TBPRD�� Aͨ����1��4��Bͨ����2��3��˫���͵���
		EPwm2Regs.CMPA.half.CMPA=(-e_pwm/(2*Vdc)+0.5)*EPwm_TIMER_TBPRD2; //������Ϊ(+-e/100+0.5)*EPwm_TIMER_TBPRD�� Aͨ����1��2��Bͨ����3��4�������Ե���
//   	flag_DataSend=TransControl();
		flag_DataSend=1;
		break;
	}
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer0�жϣ����𿪹�λ���жϣ����ض��������������ڵ�λ���������ض�˳�����
interrupt void cpu_timer0_isr(void)
{
	if(GpioDataRegs.GPCDAT.bit.GPIO69==1)
		SelfProtect();

	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}



