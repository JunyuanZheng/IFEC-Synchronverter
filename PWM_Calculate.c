#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Header.h"
//����PWM��һ��������Ҫ200���㣬���50Hz������������Ҫ200*50=10kHz�����յļ���Ƶ��ֻ��Ҫ10kHz���ɡ�
//ADC��������Ϊ��������Ƶ��50Hz�ĵ���������Ƶ������Ϊ100Hz��ѡ��1kHz����ʵ��
//��ɢ����������ѡ�������Ƶ����ͬ��Ϊ10kHz
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ز���
Uint16 mode; //��������
Uint16 adc_results[1];
float32 vg,vg_rms; //�ɼ���
float32 wn,vn_rms; //wnΪ��׼���ٶ�
float32 ig,i,i_1,L,R; //����i����ز���
float32 Dp,J,I_PI,Dq,K; //����������
float32 P_set,P,Tm,T_sum,Te,Te_1,dT,dT_IOutput,wr,w; //P������
float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen; //Q��������
float32 angle,e;
//----------------------------------------------------��ͨ�����Ժ���--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��Calculate��c����
void CalculateInit(void)
{
	mode=0; //��Sp,Sq,Scͳһ�滮Ϊmode

	adc_results[0]=0;

	vg=0.0;
	vg_rms=30;

	wn=100.0*pie;
	vn_rms=30;

	ig=0.0;
	i=0.0;
	i_1=0.0;
	L=10e-3;
	R=2.0;

	Dp=0;
	J=0;
    I_PI=0;
    K=0;
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
	Mfif_compen=30*sqrt(2)/100.0/pie;
	Mfif=Mfif_cal+Mfif_compen;

	angle=0.0;
	e=0.0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vg����������Ϊģ��50hz������ʵ��ӦΪ����������ѹ
void vg_sample(void) //vg�������� 1kHz����
{
	static Uint16 count=0,n=200;
	if(count<n)
		count++;
	else
		count=1;
	vg=30*sqrt(2)*sin(pie*count*0.01);
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
		i=(6.0*adc_results[0]/4096.0-3.0)*1000/470; //�Խ������ת�� ͨ������
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//i����Чֵ����
float32 I_RMS(float32 input)
{
	static float32 temp=0.0,output=0.0;
	static Uint16 count=0,n=200; //����Ƶ��1kHz
	if(count<n)
	{
		count++;
		temp=temp+input*input;
	}
	else
	{
		count=0;
		output=sqrt(temp/n);
		temp=0;
	}
	return output;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PQ��ͬģʽ����
//Psetģʽ
void Pset_cal(void)
{
	Tm=P_set/wn;
	Te=i*Mfif*sin(angle);
	Te=Filter(Te,Te_1,0.031416);
	Te_1=Te;
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
	Te=Filter(Te,Te_1,0.031416);
	Te_1=Te;
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
	Q=Filter(Q,Q_1,0.015708); //�˲�ϵ��ȡֵ���������
	Q_1=Q;
	Q_sum=-Q+Q_set;
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}
//Qdģʽ
void Qd_cal(void)
{
	Q=-Mfif*i*w*cos(angle);
	Q=Filter(Q,Q_1,0.015708); //�˲�ϵ��ȡֵ���������
	Q_1=Q;
	Q_sum=-Q+Q_set+Dq*(vn_rms-vg_rms); //vg_rms��Ҫ�������
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}
//---------------------------------------------------�����ú���-----------------------------------------------------------------
float32 P_Mean(float32 input)
{
	static float32 sum=0.0,output=0.0;
	static Uint16 count=0,n=200; //�ɸ���
	if(count<n)
	{
		count++;
		sum=sum+input;
	}
	else
	{
		count=0;
		output=sum/n;
		sum=0;
	}
	return output;
}
//---------------------------------------------------���Ż�����-----------------------------------------------------------------
//�˲�����a_filter�ͽ�ֹƵ���й�
float32 Filter(float32 input,float32 input_1,float32 a_filter) //
{
	float32 output;
	output=(1-a_filter)*input_1+input*a_filter; //����a
	return output;
}
