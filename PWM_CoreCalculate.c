#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//储存性变量
extern Uint16 adcresults[6];
extern float32 SinTable[200];
extern float32 i[2];
extern float32 w[2];
//计算性变量
extern float32 e,e1,e_rms,vg; //采集量
extern float32 P_set,Q_set,P,Q,Mfif; //P环参数
extern float32 angle;
//控制性变量
extern int16 mode,mode_1; //三个开关
extern Uint16 flag_PWMEnable; //控制脉冲波
float32 ig;
//----------------------------------------------------已通过测试函数--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化Calculate。c参数
void CalculateInit(void)
{
	mode=-1; //将Sp,Sq,Sc统一规划为mode
	mode_1=-2;

	e_rms=0;
	e=0.0;
	i[0]=0;
	i[1]=0;
	w[0]=100*pie;
	w[1]=100*pie;

	P_set=0.0;
	Q_set=0.0;
	Q=0.0;
	Mfif=Vac*sqrt(2)/100.0/pie;

	angle=0.0;

    GpioDataRegs.GPACLEAR.bit.GPIO4=1;
    flag_PWMEnable=0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//i采样运算
void i_cal(void)
{
	const float32 L=10e-3,R=2.0; //恒定量i用
	static float32 input=0,input_1=0,output_1=0;

	switch(mode)
	{
	case -1:
		break;
	case 0:
		i[1]=i[0];
		input=e-vg;
		i[0]=((input+input_1)-(R-2*L/T)*output_1)/(R+2*L/T);
		input_1=input;
		output_1=i[0];
		break;
	default:
		i[0]=(6.0*(adcresults[1]+20)/4095.0-3.0)*200/(470/3); //对结果进行转换 通过测试 adcresults[1]位置可能不对
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vg采样运算
void vg_cal(void)
{
	switch(mode)
	{
	case -1:
		vg=1.033*(6.0*(adcresults[0]-6)/4095.0-3.0)/156.6667*0.4*136*1000+4.5; //结果转换
		break;
	case 0:
		vg=1.033*(6.0*(adcresults[0]-6)/4095.0-3.0)/156.6667*0.4*136*1000+4.5; //结果转换
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PQ不同模式计算
//Pset模式
void P_cal(void)
{
	const float32 Dp=0.56,J=3e-5,I_PI=20;
	const float32 wn=100.0*pie;
	static float32 Te=0.0,Tm=0.0,T_sum=0.0,dT=0.0,dT_IOutput=0.0;
	static float32 wr=100*pie;

	Tm=P_set/wn;
	Te=i[1]*Mfif*sin(angle);
	Te=TeFIR_cal(Te);
	switch(mode)
	{
	case 0:
		dT=Dp*(w[0]-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	case 1:
		dT=Dp*(w[0]-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	}
	T_sum=Tm-dT-Te;
	w[1]=w[0];
	w[0]=w[1]+1.0/J*T_sum*T;
}
//Qset模式
void Q_cal(void)
{
	const float32 Mfif_compen=Vac*sqrt(2)/100.0/pie;
	const float32 K=1092.77;//Dq=43.48;
//	const float32 vn_rms=Vac;
	static float32 Q_sum=0.0;
	static float32 Mfif_cal=0;
	static Uint16 count=0;

	Q=-Mfif*i[1]*w[1]*cos(angle);
    e_rms=e_RMS(vg);
	Q=QFIR_cal(Q);
	switch(mode)
	{
	case 0:
		Q_sum=-Q+Q_set;
		mode_1=0;
		break;
//	case 1:
//		Q_sum=-Q+Q_set;
//		mode_1=1;
//		break;
	}
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
	angle=angle+w[0]*T;
    if(angle>=2.0*pie)
    	angle=angle-2.0*pie;
    e=Mfif*w[0]*SinTable[count];
	if(count<199)
		count++;
	else
		count=0;

//    e1=Mfif*w[0]*sin(angle+0.0314159*2);
}
//---------------------------------------------------
void SinTableGenerate(void)
{
	static Uint16 count=0;

	SinTable[count]=vg/321.0;
	if(count<199)
	{
		count++;
		mode_1=-1;
	}
	else
		mode=0;
}

