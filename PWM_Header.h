#ifndef PWM_HEADER_H_
#define PWM_HEADER_H_

#define	EPwm1_TIMER_TBPRD		3750
#define	EPwm2_TIMER_TBPRD		3750

void SystemInit(void);//ϵͳ��ʼ��
void InitEPwm1(void);//EPwm1��ʼ��
void InitEPwm2(void);//EPwm2��ʼ��

void ADCInit(void);//ADC��ʼ��
void ReadADC(unsigned int *p);//ADC���ת��

interrupt void EPwm1_isr(void);//ePWM1 �жϺ���

float vg(void);

#endif /* PWM_HEADER_H_ */
