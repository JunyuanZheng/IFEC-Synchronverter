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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��Calculate��c����
void CalculateInit(void)
{
	mode=0; //��Sp,Sq,Scͳһ�滮Ϊmode

	adc_results[0]=0;

	vg=0.0;
	vg_rms=0.0;

	wn=100.0*pie;
	vn_rms=30;

	ig=0.0;
	i=0.0;
	i_1=0.0;
	L=10e-3;
	R=2.0;

	Dp=0.56;
	J=2e-5;
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
//��ͨ�˲���
float32 Filter(float32 input,float32 input_1)
{
	float32 output;
	output=(1-1/(1+a_filter))*input_1+input*1/(1+a_filter); //����a
	return output;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vg����������Ϊģ��50hz������ʵ��ӦΪ����������ѹ
float32 vg_sample(void) //vg�������� 1kHz����
{
	static Uint16 count=0;
	if(count*T1>=0.02)
		count=0;
	count++;
	vg=30*sqrt(2)*sin(pie*count*0.1);
	return vg;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VgΪvg����Чֵ���˺�������vg��Чֵ
float32 Vg_RMS(float32 input)
{
	static float32 temp=0.0,output=0.0;
	static Uint16 count=0,n=20; //����Ƶ��1kHz
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
//����i���㣬1��������� 2��������������
float32 i_sample(void)
{
	if(e!=0)
	{
		switch(mode)
			{
			case 0:
				i=((e-vg)*T1+L*i_1)/(R*T1+L); //�����������,T1=1khz��Ӧʱ��
				i_1=i; //i��i_1׼���´�����
				break;
			default:
				i=e/200;
			}
	}
	return i;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//i����Чֵ����
float32 I_RMS(float32 input)
{
	static float32 temp=0.0,output=0.0;
	static Uint16 count=0,n=20; //����Ƶ��1kHz
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
	//�˲�
	Te=Filter(Te,Te_1);
	Te_1=Te;
	dT=Dp*(w-wr);
	dT_IOutput=dT_IOutput+K*T*dT;
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
	Te=Filter(Te,Te_1);
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
	Q=Filter(Q,Q_1);
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
	Q=Filter(Q,Q_1);
	Q_1=Q;
	Q_sum=-Q+Q_set+Dq*(vn_rms-vg_rms);
	Mfif_cal= Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}


