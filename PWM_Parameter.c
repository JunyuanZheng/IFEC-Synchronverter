#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����PWM_CoreCalculate.c������Ҫ�ı���
Uint16 mode; //��������
Uint16 adc_results[1];
float32 vg,vg_rms; //�ɼ���
float32 wn,vn_rms; //wnΪ��׼���ٶ�
float32 ig,i,i_1,L,R; //����i����ز���
float32 Dp,J,I_PI,Dq,K; //����������
float32 P_set,P,Tm,T_sum,Te,Te_1,dT,dT_IOutput,wr,w; //P������
float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen; //Q��������
float32 angle,e;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����PWM_SupportCalculate.c������Ҫ�ı���
/*TransData����*/
Uint16 data_bit[20];


