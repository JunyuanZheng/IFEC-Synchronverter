#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ز���
extern Uint16 mode; //��������
extern Uint16 adc_results[1];
extern float32 vg,vg_rms; //�ɼ���
extern float32 wn,vn_rms; //wnΪ��׼���ٶ�
extern float32 ig,i,i_1,L,R; //����i����ز���
extern float32 Dp,J,I_PI,Dq,K; //����������
extern float32 P_set,P,Tm,T_sum,Te,Te_1,dT,dT_IOutput,wr,w; //P������
extern float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen; //Q��������
extern float32 angle,e;
//----------------------------------------------------��ͨ�����Ժ���--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��Calculate��c����
void CalculateInit(void)
{
	mode=0; //��Sp,Sq,Scͳһ�滮Ϊmode

	adc_results[0]=0;

	vg=0.0;
	vg_rms=0;

	wn=100.0*pie;
	vn_rms=220;//60;//30;

	ig=0.0;
	i=0.0;
	i_1=0.0;
	L=10e-3;
	R=2.0;

	Dp=0.56;
	J=3e-5;
    I_PI=20;
    K=1092.77;
    Dq=43.48;

	P_set=0.0;
	Tm=0.0;
	T_sum=0.0;
	Te=0.0;
	Te_1=0.0;
	dT=0.0;
	dT_IOutput=0.0;
	wr=100*pie;
	w=100*pie;

	Q_set=0.0;
	Q_sum=0.0;
	Q=0.0;
	Q_1=0.0;
	Mfif_cal=0;
	Mfif_compen=220*sqrt(2)/100.0/pie;
	Mfif=Mfif_cal+Mfif_compen;

	angle=0.0;
	e=0.0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vg����������Ϊģ��50hz������ʵ��ӦΪ����������ѹ
void vg_sample(void) //vg�������� 10kHz���� //���޸�
{
	static Uint16 count=1,n=200;
	vg=220*sqrt(2)*sin(pie*count*0.01);
	if(count<n)
		count++;
	else
		count=1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����i���㣬1��������� 2��������������
void i_sample(void)
{
	switch(mode)
	{
	case 0:
		i=((e-vg)*T+L*i_1)/(R*T+L); //�����������,T=10khz
		i_1=i; //i��i_1׼���´�����
		break;
	default:
		ReadADC(adc_results); //��ȡADC���
		i=(6.0*(adc_results[0])/4095.0-3.0)*1000/470; //�Խ������ת�� ͨ������
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PQ��ͬģʽ����
//Psetģʽ
void Pset_cal(void)
{
	Tm=P_set/wn;
	Te=i*Mfif*sin(angle);
	Te=TeFilter(Te,0.02);
	dT=Dp*(w-wr);
	dT_IOutput=dT_IOutput+I_PI*T*dT;
	wr=wn+dT_IOutput;
	T_sum=Tm-dT-Te;
	w=w+1.0/J*T_sum*T;
	angle=angle+w*T;
	if(angle>=2.0*pie)
		angle=angle-2.0*pie;
	if(angle<0)
		angle=angle+2.0*pie;
}
//Pdģʽ
void Pd_cal(void)
{
	Tm=P_set/wn;
	Te=i*Mfif*sin(angle);
	Te=TeFilter(Te,0.02);
	dT=Dp*(w-wn);
	T_sum=Tm-dT-Te;
 	w=w+1.0/J*T_sum*T;
	angle=angle+w*T;
	if(angle>=2.0*pie)
		angle=angle-2.0*pie;
	if(angle<0)
		angle=angle+2.0*pie;
}
//Qsetģʽ
void Qset_cal(void)
{
	Q=-Mfif*i*w*cos(angle);
	Q=QFilter(Q,0.3);
	Q_sum=-Q+Q_set;
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}
//Qdģʽ
void Qd_cal(void)
{
	Q=-Mfif*i*w*cos(angle);
	Q=QFilter(Q,0.3);
	Q_sum=-Q+Q_set+Dq*(vn_rms-vg_rms); //vg_rms��Ҫ�������
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}

