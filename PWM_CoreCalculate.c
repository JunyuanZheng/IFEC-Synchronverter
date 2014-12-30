#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//储存性变量
extern Uint16 adcresults[6];
extern float32 SinTable[1500];
extern float32 i[2];
extern float32 w[2];
//计算性变量
extern float32 e,e_pwm,e_rms,vg; //采集量
extern float32 P_set,Q_set,Q,Mfif; //P环参数
extern float32 angle;
//控制性变量
extern int16 mode,mode_1; //三个开关
extern Uint16 flag_PWMEnable; //控制脉冲波
//----------------------------------------------------已通过测试函数--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化Calculate。c参数
void CalculateInit(void)
{
	mode=-1; //将Sp,Sq,Sc统一规划为mode
	mode_1=-2;

	e_rms=0;
	e=0.0;
	i[k]=0;
	i[k-1]=0;
	w[k]=100*pie;
	w[k-1]=100*pie;

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
	const float32 L=100e-3,R=20.0; //恒定量i用
	static float32 input=0,input_1=0,output_1=0;

	switch(mode)
	{
	case -1:
		break;
	case 0:
		i[k-1]=i[k];
		input=e-vg;
		i[k]=((input+input_1)-(R-2*L/T)*output_1)/(R+2*L/T);
		input_1=input;
		output_1=i[k];
//		ig=(6.0*(adcresults[1]+20)/4095.0-3.0)*200/156.667;
//		if((ig<-3)||(ig>3))
//			SelfProtect();
		break;
	default:
		i[k]=(6.0*(adcresults[1]+20)/4095.0-3.0)*200/156.667; //对结果进行转换 通过测试 adcresults[1]位置可能不对
		if(i[k]<-3||i[k]>3)
			SelfProtect();
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
		vg=1.033*(6.0*(adcresults[0]-5)/4095.0-3.0)/156.667*0.4*136*1000+2.5; //结果转换
		break;
	case 0:
		vg=1.033*(6.0*(adcresults[0]-5)/4095.0-3.0)/156.667*0.4*136*1000+2.5; //结果转换
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
	Te=i[k-1]*Mfif*sin(angle);
	Te=TeFIR_cal(Te);
	switch(mode)
	{
	case 0:
		dT=Dp*(w[k]-wr); //应该有问题！感觉是w[k-1]
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	case 1:
		dT=Dp*(w[k]-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	}
	T_sum=Tm-dT-Te;
	w[k-1]=w[k];
	w[k]=w[k-1]+1.0/J*T_sum*T;
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

	Q=-Mfif*i[k-1]*w[k-1]*cos(angle);
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
	angle=angle+w[k]*T;
    if(angle>=2.0*pie)
    	angle=angle-2.0*pie;
    e=Mfif*w[k]*sin(angle);
    count=(int)((angle+0.0314159*2)*750/pie);
    if(count>=1500)
    	count=count-1500;
    e_pwm=0.968*Mfif*w[k]*SinTable[count];
}
//---------------------------------------------------
void GridImitate(void)
{
	static Uint16 flag=0,count=0; //15个点对应2个相位
	static float32 vg_1=0;

	if(vg>0&&vg_1<0)
		flag=1;

	if(flag==1)
	{
		if(vg>=0)
			SinTable[count]=vg/324.0;
		else
			SinTable[count]=vg/324.0;
	}

	if(count<1499)
		count++;
	else
		mode=0;
	vg_1=vg;
}

