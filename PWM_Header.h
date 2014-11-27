#ifndef PWM_HEADER_H_
#define PWM_HEADER_H_

#define	EPwm1_TIMER_TBPRD		3750
#define	EPwm2_TIMER_TBPRD		3750

void SystemInit(void);//系统初始化
void InitEPwm1(void);//EPwm1初始化
void InitEPwm2(void);//EPwm2初始化

void ADCInit(void);//ADC初始化
void ReadADC(unsigned int *p);//ADC结果转化

interrupt void EPwm1_isr(void);//ePWM1 中断函数

float vg(void);

#endif /* PWM_HEADER_H_ */
