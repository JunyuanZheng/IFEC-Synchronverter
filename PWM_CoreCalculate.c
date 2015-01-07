#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����Ա���
extern Uint16 adcresults[6];
extern float32 SinTable[grid_freq];
extern float32 i[2];
extern float32 w[2];
//�����Ա���
extern float32 e,e_pwm,e_rms,vg; //�ɼ���
extern float32 P_set,Q_set,Mfif; //P������
extern float32 angle;
//�����Ա���
extern int16 mode,mode_1; //��������
extern Uint16 flag_PWMEnable; //�������岨
extern float32 phase,vn;
//�����Ա���
extern float32 DriftRectify_Vg;
extern float32 Q=0,Te,vg_rms;
//----------------------------------------------------��ͨ�����Ժ���--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��Calculate��c����
void CalculateInit(void)
{
	static Uint16 count=0;

	mode=-1; //��Sp,Sq,Scͳһ�滮Ϊmode
	mode_1=-2;

	for(count=0;count<(grid_freq-1);count++)
		SinTable[count]=0;
	i[k]=0;
	i[k-1]=0;
	w[k]=100*pie;
	w[k-1]=100*pie;

	e_rms=0;
	e=0.0;
	P_set=0.0;
	Q_set=0.0;
	Mfif=Vac*sqrt(2)/100.0/pie;

	angle=0.0;

    flag_PWMEnable=0;


    DriftRectify_Vg=0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//i��������
void i_cal(void)
{
	const float32 L=100e-3,R=20.0; //�㶨��i��
	static float32 input=0,input_1=0,output_1=0;

	i[k-1]=i[k];
	switch(mode)
	{
	case -1:
		break;
	case 0:
		input=e-vg;
		i[k]=((input+input_1)-(R-2*L/T)*output_1)/(R+2*L/T);
		input_1=input;
		output_1=i[k];
		break;
	default:
		i[k]=(6.0*(adcresults[1]+19)/4095.0-3.0)*200/156.667; //�Խ������ת�� ͨ������ adcresults[1]λ�ÿ��ܲ���
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vg��������
void vg_cal(void)
{
	switch(mode)
	{
	case -1:
		vg=(6.0*(adcresults[0]-8)/4095.0-3.0)/156.667*0.4*136*1000+DriftRectify_Vg; //���ת��
		break;
	case 0:
		vg=(6.0*(adcresults[0]-8)/4095.0-3.0)/156.667*0.4*136*1000+DriftRectify_Vg; //���ת��
		break;
	default:
		vg=(6.0*(adcresults[0]-8)/4095.0-3.0)/156.667*0.4*136*1000+DriftRectify_Vg; //���ת��
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PQ��ͬģʽ����
//Psetģʽ
void P_cal(void)
{
	const float32 Dp=0.56,J=3e-5,I_PI=2;
	const float32 wn=100.0*pie;
	static float32 Tm=0,T_sum=0,dT=0,dT_IOutput=0;
	static float32 wr=100*pie;

	Tm=P_set/wn;
	Te=i[k-1]*Mfif*sin(angle);
//	Te=TeFIR_cal(Te);
    Te=TeFilter(Te,0.1);
	switch(mode)
	{
	case 0:
		dT=Dp*(w[k]-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	case 1: //Pset,Qset
		dT=Dp*(w[k]-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	case 2: //Pd,Qd
		dT=Dp*(w[k]-wn);
		break;
	case 3: //Pset,Qd
		dT=Dp*(w[k]-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	case 4: //Pd,Qset
		dT=Dp*(w[k]-wn);
		break;
	}
	T_sum=Tm-dT-Te;
	w[k-1]=w[k];
	w[k]=w[k-1]+1.0/J*T_sum*T;
}
//Qsetģʽ
void Q_cal(void)
{
	const float32 Mfif_compen=Vac*sqrt(2)/100.0/pie;
	const float32 K=3000,Dq=43.48;
	static float32 Q_sum=0;
	static float32 Mfif_cal=0;
	static Uint16 point=0;

	Q=-Mfif*i[k-1]*w[k-1]*cos(angle);
    e_rms=e_RMS(e);
    vg_rms=vg_RMS(vg);
//	Q=QFIR_cal(Q);
    Q=QFilter(Q,0.02);
	switch(mode)
	{
	case 0: //Pset,Qset
		Q_sum=-Q+Q_set;
		mode_1=0;
		break;
	case 1: //Pset,Qset
		Q_sum=-Q+Q_set;
		mode_1=1;
		break;
	case 2: //Pd,Qd
		Q_sum=-Q+Q_set+Dq*(vn-e_rms); //vg_rms
		mode_1=2;
		break;
	case 3: //Pset,Qd
		Q_sum=-Q+Q_set+Dq*(vn-vg_rms);
		mode_1=3;
		break;
	case 4: //Pd,Qset
		Q_sum=-Q+Q_set;
		mode_1=4;
		break;
	}
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w[k-1]*sin(angle);
    point=(int)((angle+0.0314159*phase)*(grid_freq/2)/pie);
    if(point>=grid_freq)
    	point=point-grid_freq;
    e_pwm=Mfif*w[k-1]*SinTable[point];
//    e_pwm=Mfif*w[k-1]*sin(angle+0.0314159*phase);
	angle=angle+w[k-1]*T;
    if(angle>=2.0*pie)
    	angle=angle-2.0*pie;
}
