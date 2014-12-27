#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ز���
extern int16 mode,mode_1; //��������
extern float32 vg,e_rms,i,e; //�ɼ���
extern float32 P_set,Q_set,P,Q,Mfif,w; //P������
extern float32 angle;
extern Uint16 adcresults[2];
//----------------------------------------------------��ͨ�����Ժ���--------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��Calculate��c����
void CalculateInit(void)
{
	mode=0; //��Sp,Sq,Scͳһ�滮Ϊmode
	mode_1=-1;

	vg=0.0;
	e_rms=0;
	i=0.0;
	e=0.0;

	P_set=0.0;
	Q_set=0.0;
	Q=0.0;
	Mfif=Vac*sqrt(2)/100.0/pie;
	w=100*pie;

	angle=0.0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//vg����������Ϊģ��50hz������ʵ��ӦΪ����������ѹ
void sample(void) //vg�������� 10kHz���� //���޸�
{
	const float32 L=10e-3,R=2.0; //�㶨��i��
	static float32 input=0,output_1=0,input_1=0; //i��

	switch(mode)
	{
		case 0:
			vg=1.04*(6.0*(adcresults[0]-10)/4095.0-3.0)/156.6667*0.4*136*1000; //���ת��
			input=e-vg;
			i=((input+input_1)-(R-2*L/T)*output_1)/(R+2*L/T);
			input_1=input;
			output_1=i;
			break;
		default:
			i=(6.0*(adcresults[1])/4095.0-3.0)*500/470; //�Խ������ת�� ͨ������ adcresults[1]λ�ÿ��ܲ���
			break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PQ��ͬģʽ����
//Psetģʽ
void P_cal(void)
{
	const float32 Dp=0.56,J=3e-5,I_PI=20;
	const float32 wn=100.0*pie;
	static float32 Te=0.0,Tm=0.0,T_sum=0.0,dT=0.0,dT_IOutput=0.0;
	static float32 wr=100*pie;

/*	static Uint16 count=0;
 *  static float32 output[200];
 *  output[count]=QFIR_cal(Q);
 *  if(count<199)
 *      count++;
 *  else
 *      count=0;
*/
	Tm=P_set/wn;
	Te=i*Mfif*sin(angle);
	Te=TeFIR_cal(Te);
	switch(mode)
	{
	case 0:
		dT=Dp*(w-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	case 1:
		dT=Dp*(w-wn);
		break;
	case 2:
		dT=Dp*(w-wr);
		dT_IOutput=dT_IOutput+I_PI*T*dT;
		wr=wn+dT_IOutput;
		break;
	}
	T_sum=Tm-dT-Te;
	w=w+1.0/J*T_sum*T;
	P=Te*w;
}
//Qsetģʽ
void Q_cal(void)
{
	const float32 Mfif_compen=Vac*sqrt(2)/100.0/pie;
	const float32 K=1092.77,Dq=43.48;
	const float32 vn_rms=Vac;
	static float32 Q_sum=0.0;
	static float32 Mfif_cal=0;

/*	static Uint16 count=0;
 *  static float32 output[200];
 *  output[count]=QFIR_cal(Q);
 *  if(count<199)
 *      count++;
 *  else
 *      count=0;
 */

	Q=-Mfif*i*w*cos(angle);
    e_rms=e_RMS(e);
	Q=QFIR_cal(Q);
	switch(mode)
	{
	case 0:
		Q_sum=-Q+Q_set;
		mode_1=0;
		break;
	case 1:
		Q_sum=-Q+Q_set+Dq*(vn_rms-e_rms);
		mode_1=1;
		break;
	case 2:
		Q_sum=-Q+Q_set;
		mode_1=2;
		break;
	}
	Mfif_cal=Mfif_cal+1.0/K*Q_sum*T;
	Mfif=Mfif_cal+Mfif_compen;
	angle=angle+w*T;
	if(angle>2.0*pie)
		angle=0;
    e=Mfif*w*sin(angle);
}
