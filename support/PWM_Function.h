#ifndef PWM_FUNCTION_H_
#define PWM_FUNCTION_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//自带相关函数
float32 sqrt(float32 input); //开方运算 FPU
float32 sin(float32 input); //正弦云孙FPU
float32 cos(float32 input); //余弦云孙FPU
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_ADC.c相关函数
void ADCInit(void);
void ADCReInit(int16 mode);
void ReadADC(unsigned int *p,int16 mode);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_CoreCalculate.c相关函数
void CalculateInit(void);
void i_cal(void);
void vg_cal(void);
void P_cal(void);
void Q_cal(void);

float32 TeFilter(float32 input,float32 b);
float32 QFilter(float32 input,float32 b);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_ePWM.c相关函数
void InitEPwm1(void);
void InitEPwm2(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_GPIO.c相关函数
void Gpio_setup1(void);
void Gpio_setup2(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_FIR.c相关函数
//void InitFIR(void);
//滤波函数
float32 TeFIR_cal(float32 input);
float32 QFIR_cal(float32 input);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_SCI.c相关函数
void InitSCI(void);
void sci_send(Uint16 input);
interrupt void sciaTx_isr(void);
interrupt void sciaRx_isr(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_SupportCalculate.c相关函数
//模拟电网
void GridImitate(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_SupportFunction.c相关函数
void TransData(float32 input);
Uint16 TransControl(void);
//系统自我保护函数
void SelfProtect(void);
//辅助函数
float32 e_RMS(float32 input);
float32 vg_RMS(float32 input);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PWM_Timer.c相关函数
void TimerInit(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//中断函数
interrupt void epwm1_isr(void); //epwm1中断
interrupt void cpu_timer0_isr(void); //Timer0中断

//interrupt void cpu_timer1_isr(void); //Timer1中断
#endif /* PWM_FUNCTION_H_ */
