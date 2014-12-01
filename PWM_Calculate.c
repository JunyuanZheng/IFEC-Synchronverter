#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Header.h"
//生成PWM波一个周期需要200个点，因此50Hz的逆变器输出需要200*50=10kHz，最终的计算频率只需要10kHz即可。
//ADC采样，因为采样的是频率50Hz的电流，采样频率至少为100Hz，选择1kHz进行实验
//离散化采样周期选择与计算频率相同即为10kHz
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//相关参数
Uint16 Sp,Sq,Sc; //三个开关
float32 E;
float32 vg,Vg; //采集量
float32 Wn,Vn; //Wn为标准角速度
float32 ig,i,i_1,L,R; //计算i的相关参数
float32 P_set,Tm,T_sum,Te,Te_1,dT,dT_IOutput,Wr,W,Dp,J,P_Ki; //P环参数
float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen,K,Dq; //Q环运算量
float32 angle,e;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PI结构体
//PI_FUNC P_PI;
//PI_FUNC Q_PI;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化Calculate。c参数
void CalculateInit(void)
{
	Sp=1; //开关初始状态设置
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
	Dp=0.3; //0.56   可用参数0.3/2e-2/10
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
	P_PI.Kp=0;//有问题
	P_PI.Ti=20;//有问题
 */

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//低通滤波器
float32 Filter(float32 input,float32 input_1)
{
	float32 output;
	output=(1-a_Filter)*input_1+input*a_Filter; //参数a
	return output;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vg采样，现在为模拟50hz电网，实际应为采样电网电压
float32 vg_sample(void) //vg采样运算 1kHz采样
{
	static Uint16 count=0;
	if(count*T1>=0.02)
		count=0;
	count++;
	vg=30*sqrt(2)*sin(pie*count*0.1);
	return vg;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Vg为vg的有效值，此函数计算vg有效值
float32 Vg_RMS(float32 input)
{
	static float32 temp=0.0,output=0.0;
	static Uint16 count=0,n=20; //采样频率1kHz
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
//电流i计算，1：虚拟电流 2：电网电流采样
float32 i_sample(void)
{
	if((Sc==1)&&(e!=0))
	{
		i=((e-vg)*T1+L*i_1)/(R*T1+L); //计算虚拟电流,T1=1khz对应时间
	    i_1=i; //i给i_1准备下次运算
	}
	else
		i=ig;
	return i;
}
float32 Ig_RMS(float32 input)
{
	static float32 temp=0.0,output=0.0;
	static Uint16 count=0,n=20; //采样频率1kHz
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
	static Uint16 count=0,n=20; //采样频率1kHz
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
//PQ不同模式计算
//Pset模式
void Pset_cal(void)
{
	Tm=P_set/Wn;
	Te=i*Mfif*sin(angle);
	//滤波
	Te=Filter(Te,Te_1);
	Te_1=Te;
	dT=Dp*(W-Wr);
	dT_IOutput=dT_IOutput+P_Ki*T*dT;
	Wr=Wn+dT_IOutput;
	T_sum=Tm-dT-Te;
	W=W+1.0/J*T_sum*T;
	angle=angle+W*T;
	if(angle>=2.0*pie) //待测试
		angle=angle-2.0*pie;
	if(angle<0)
		angle=angle+2.0*pie;
}
//Pd模式
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
//Qset模式
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
//Qd模式
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
