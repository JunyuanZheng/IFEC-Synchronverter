#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//相关参数
extern Uint16 mode; //三个开关
extern Uint16 adc_results[1];
extern float32 vg,vg_rms; //采集量
extern float32 wn,vn_rms; //wn为标准角速度
extern float32 ig,i,i_1,L,R; //计算i的相关参数
extern float32 Dp,J,I_PI,Dq,K; //控制器参数
extern float32 P_set,P,Tm,T_sum,Te,Te_1,dT,dT_IOutput,wr,w; //P环参数
extern float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen; //Q环运算量
extern float32 angle,e;
//----------------------------------------------------已通过测试函数--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化Calculate。c参数
void CalculateInit(void)
{
	mode=0; //将Sp,Sq,Sc统一规划为mode

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
//vg采样，现在为模拟50hz电网，实际应为采样电网电压
void vg_sample(void) //vg采样运算 10kHz采样 //已修改
{
	static Uint16 count=1,n=200;
	vg=220*sqrt(2)*sin(pie*count*0.01);
	if(count<n)
		count++;
	else
		count=1;
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
		i=(6.0*(adc_results[0])/4095.0-3.0)*1000/470; //对结果进行转换 通过测试
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PQ不同模式计算
//Pset模式
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
//Pd模式
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
//Qset模式
void Qset_cal(void)
{
	Q=-Mfif*i*w*cos(angle);
	Q=QFilter(Q,0.3);
	Q_sum=-Q+Q_set;
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}
//Qd模式
void Qd_cal(void)
{
	Q=-Mfif*i*w*cos(angle);
	Q=QFilter(Q,0.3);
	Q_sum=-Q+Q_set+Dq*(vn_rms-vg_rms); //vg_rms需要计算程序
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
    e=Mfif*w*sin(angle);
}

