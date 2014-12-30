#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"

float32 TeMean(float32 input)
{
	static float sum=0.0,output=0.0;
	static Uint16 count=0;
	if(count<200)
	{
		count++;
		output=output+input;
	}
	else
	{
		count=0;
		output=sum/200;
		sum=0;
	}
	return output;
}

float32 QMean(float32 input)
{
	static float sum=0.0,output=0.0;
	static Uint16 count=0;
	if(count<200)
	{
		count++;
		output=output+input;
	}
	else
	{
		count=0;
		output=sum/200;
		sum=0;
	}
	return output;
}
