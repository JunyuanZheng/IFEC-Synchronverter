#ifndef PWM_PI_H_
#define PWM_PI_H_

typedef struct{
	float Give;
	float Feedback;
//PID调节器部分
	float Kp;
	float Ti;
	float T;
	float a0;
	float a1;
	float ek;
	float ek_1;
	float Output;
	float LastOutput;
	void(*calc)();
}PI_FUNC;

typedef PI_FUNC*PID_FUNC_handle;


#endif /* PWM_PI_H_ */
