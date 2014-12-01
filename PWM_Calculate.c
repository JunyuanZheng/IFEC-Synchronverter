#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Header.h"
//����PWM��һ��������Ҫ200���㣬���50Hz������������Ҫ200*50=10kHz�����յļ���Ƶ��ֻ��Ҫ10kHz���ɡ�
//ADC��������Ϊ��������Ƶ��50Hz�ĵ���������Ƶ������Ϊ100Hz��ѡ��1kHz����ʵ��
//��ɢ����������ѡ�������Ƶ����ͬ��Ϊ10kHz
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ز���
Uint16 Sp,Sq,Sc; //��������
float32 E;
float32 vg,Vg; //�ɼ���
float32 Wn,Vn; //WnΪ��׼���ٶ�
float32 ig,i,i_1,L,R; //����i����ز���
float32 P_set,Tm,T_sum,Te,Te_1,dT,dT_IOutput,Wr,W,Dp,J,P_Ki; //P������
float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen,K,Dq; //Q��������
float32 angle,e;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PI�ṹ��
//PI_FUNC P_PI;
//PI_FUNC Q_PI;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��Calculate��c����
void CalculateInit(void)
{
	Sp=1; //���س�ʼ״̬����
	Sq=0;
	Sc=1;

	vg=0.0;
	Vg=0.0;

	Wn=100.0*pie;
	Vn=30;

	ig=0.0;
	i=0.0;
	i_1=0.0;
	L=10e-3;
	R=2.0;

	P_set=0.0;
	Tm=0.0;
	T_sum=0.0;
	Te=0.0;
	Te_1=0.0;
	dT=0.0;
	dT_IOutput=0.0;
	Wr=100*pie;
	W=100*pie;
	Dp=0.3; //0.56   ���ò���0.3/2e-2/10
	J=2e-2; //2E-5
	P_Ki=10; //20

	Q_set=0.0;
	Q_sum=0.0;
	Q=0.0;
	Q_1=0.0;
	Mfif_cal=0;
	Mfif_compen=30*sqrt(2)/100.0/pie;
	Mfif=Mfif_cal+Mfif_compen;
	K=1092.77;
	Dq=43.48;

	angle=0.0;
	e=0.0;
/*
    P_PI.Give=0;
	P_PI.Feedback=0;
	P_PI.Kp=0;//������
	P_PI.Ti=20;//������
 */

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ͨ�˲���
float32 Filter(float32 input,float32 input_1)
{
	float32 output;
	output=(1-a_Filter)*input_1+input*a_Filter; //����a
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
	if((Sc==1)&&(e!=0))
	{
		i=((e-vg)*T1+L*i_1)/(R*T1+L); //�����������,T1=1khz��Ӧʱ��
	    i_1=i; //i��i_1׼���´�����
	}
	else
		i=ig;
	return i;
}
float32 Ig_RMS(float32 input)
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
float32 E_RMS(float32 input)
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
	Tm=P_set/Wn;
	Te=i*Mfif*sin(angle);
	//�˲�
	Te=Filter(Te,Te_1);
	Te_1=Te;
	dT=Dp*(W-Wr);
	dT_IOutput=dT_IOutput+P_Ki*T*dT;
	Wr=Wn+dT_IOutput;
	T_sum=Tm-dT-Te;
	W=W+1.0/J*T_sum*T;
	angle=angle+W*T;
	if(angle>=2.0*pie) //������
		angle=angle-2.0*pie;
	if(angle<0)
		angle=angle+2.0*pie;
}
//Pdģʽ
void Pd_cal(void)
{
	Tm=P_set/Wn;
	Te=i*Mfif*sin(angle);
	Te=Filter(Te,Te_1);
	Te_1=Te;
	dT=Dp*(W-Wn);
	T_sum=Tm-dT-Te;
 	W=W+1.0/J*T_sum*T;
	angle=angle+W*T;
	if(angle>=2.0*pie)
		angle=angle-2.0*pie;
	if(angle<0)
		angle=angle+2.0*pie;
}
//Qsetģʽ
void Qset_cal(void)
{
	Q=-Mfif*i*W*cos(angle);
	Q=Filter(Q,Q_1);
	Q_1=Q;
	Q_sum=-Q+Q_set;
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*W*sin(angle);
}
//Qdģʽ
void Qd_cal(void)
{
	Q=-Mfif*i*W*cos(angle);
	Q=Filter(Q,Q_1);
	Q_1=Q;
	Q_sum=-Q+Q_set+Dq*(Vn-Vg);
	Mfif_cal= Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*W*sin(angle);
}
