#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"


float32 TeFilter(float32 input,float32 b)
{
	static float32 output_1=0,input_1=0,output=0;
	output=((input+input_1)-(1-2*b/T)*output_1)/(2*b/T+1);
	input_1=input;
	output_1=output;
	return output;
}
float32 QFilter(float32 input,float32 b)
{
	static float32 output_1=0,input_1=0,output=0;
	output=((input+input_1)-(1-2*b/T)*output_1)/(2*b/T+1);
	input_1=input;
	output_1=output;
	return output;
}



