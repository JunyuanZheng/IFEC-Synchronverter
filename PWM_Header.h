#ifndef PWM_HEADER_H_
#define PWM_HEADER_H_
#include "PWM_PI.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_Main.c相关参数
#define PWMS_FRC_DISABLE 0x0000 //宏定义，禁止ePWM模块强制功能
#define PWMS_ALBL 0x0005 //宏定义，强制A低电平，B低电平
#define PWMS_AHBH 0x000A //宏定义，强制A高电平，B高电平
#define PWMS_AHBL 0x0006 //宏定义，强制A高电平，B低电平
#define PWMS_ALBH 0x0009 //宏定义，强制A低电平，B高电平
#define EPwm1_TIMER_TBPRD 7500 //PWM频率设置
#define EPwm2_TIMER_TBPRD 7500 //PWM频率设置
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_Calculate.c相关参数
#define pie 3.14159
#define T 0.0001 //PWM频率和运算周期
#define T1 0.001 //采样周期
#define a_Filter 0.005 //低通滤波器滤波系数
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_Main.c相关函数
interrupt void epwm1_timer_isr(void); //epwm1中断
interrupt void cpu_timer0_isr(void); //timer0中断
void TimerInit(void);
void InitEPwm1(void);
void InitEPwm2(void);
void ADCInit(void);
void ReadADC(unsigned int *p);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_Calculate.c相关函数
void CalculateInit(void);
void PIfunc_calc(PI_FUNC*p);
float32 Filter(float32 input,float32 input_1);
float32 vg_sample(void);
float32 Vg_RMS(float32 input);
float32 i_sample(void);
float32 Ig_RMS(float32 input);
float32 E_RMS(float32 input);
void Pset_cal(void);
void Pd_cal(void);
void Qset_cal(void);
void Qd_cal(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//自带相关函数
float32 sqrt(float32 input); //开方运算 FPU
float32 sin(float32 input); //正弦云孙FPU
float32 cos(float32 input); //余弦云孙FPU
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif /* PWM_HEADER_H_ */
