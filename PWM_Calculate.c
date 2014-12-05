#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Header.h"
//生成PWM波一个周期需要200个点，因此50Hz的逆变器输出需要200*50=10kHz，最终的计算频率只需要10kHz即可。
//ADC采样，因为采样的是频率50Hz的电流，采样频率至少为100Hz，选择1kHz进行实验
//离散化采样周期选择与计算频率相同即为10kHz
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//相关参数
Uint16 mode; //三个开关
Uint16 adc_results[1];
float32 vg,vg_rms; //采集量
float32 wn,vn_rms; //wn为标准角速度
float32 ig,i,i_1,L,R; //计算i的相关参数
float32 Dp,J,I_PI,Dq,K; //控制器参数
float32 P_set,P,Tm,T_sum,Te,Te_1,dT,dT_IOutput,wr,w; //P环参数
float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen; //Q环运算量
float32 angle,e;
//----------------------------------------------------已通过测试函数--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化Calculate。c参数
void CalculateInit(void)
{
	mode=0; //将Sp,Sq,Sc统一规划为mode

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
//vg采样，现在为模拟50hz电网，实际应为采样电网电压
void vg_sample(void) //vg采样运算 1kHz采样
{
	static Uint16 count=0,n=200;
	if(count<n)
		count++;
	else
		count=1;
	vg=30*sqrt(2)*sin(pie*count*0.01);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//电流i计算，1：虚拟电流 2：电网电流采样
void i_sample(void)
{
	switch(mode)
	{
	case 0:
		i=((e-vg)*T+L*i_1)/(R*T+L); //计算虚拟电流,T=10khz
		i_1=i; //i给i_1准备下次运算
		break;
	default:
		ReadADC(adc_results); //读取ADC解雇
		i=(6.0*adc_results[0]/4096.0-3.0)*1000/470; //对结果进行转换 通过测试
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//i的有效值计算
float32 I_RMS(float32 input)
{
	static float32 temp=0.0,output=0.0;
	static Uint16 count=0,n=200; //采样频率1kHz
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
//Pd模式
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
//Qset模式
void Qset_cal(void)
{
	Q=-Mfif*i*w*cos(angle);
	Q=Filter(Q,Q_1,0.015708); //滤波系数取值问题带考虑
	Q_1=Q;
	Q_sum=-Q+Q_set;
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}
//Qd模式
void Qd_cal(void)
{
	Q=-Mfif*i*w*cos(angle);
	Q=Filter(Q,Q_1,0.015708); //滤波系数取值问题带考虑
	Q_1=Q;
	Q_sum=-Q+Q_set+Dq*(vn_rms-vg_rms); //vg_rms需要计算程序
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}
//---------------------------------------------------测试用函数-----------------------------------------------------------------
float32 P_Mean(float32 input)
{
	static float32 sum=0.0,output=0.0;
	static Uint16 count=0,n=200; //可更改
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
//---------------------------------------------------可优化函数-----------------------------------------------------------------
//滤波函数a_filter和截止频率有关
float32 Filter(float32 input,float32 input_1,float32 a_filter) //
{
	float32 output;
	output=(1-a_filter)*input_1+input*a_filter; //参数a
	return output;
}
