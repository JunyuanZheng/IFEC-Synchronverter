#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定义PWM_CoreCalculate.c函数需要的变量
Uint16 mode; //三个开关
Uint16 adc_results[1];
float32 vg,vg_rms; //采集量
float32 wn,vn_rms; //wn为标准角速度
float32 ig,i,i_1,L,R; //计算i的相关参数
float32 Dp,J,I_PI,Dq,K; //控制器参数
float32 P_set,P,Tm,T_sum,Te,Te_1,dT,dT_IOutput,wr,w; //P环参数
float32 Q_set,Q_sum,Q,Q_1,Mfif,Mfif_cal,Mfif_compen; //Q环运算量
float32 angle,e;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定义PWM_SupportCalculate.c函数需要的变量
/*TransData函数*/
Uint16 data_bit[20];


