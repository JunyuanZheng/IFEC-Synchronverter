#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"

void TimerInit(void)//Timer0初始化，极其中断设置，定时1us
{
	InitCpuTimers();
#if (CPU_FRQ_150MHZ)
//ConfigCpuTimer(&CpuTimer0,150,Timer0);
#endif
#if (CPU_FRQ_100MHZ)
//ConfigCpuTimer(&CpuTimer0,100,Timer0);
#endif
    CpuTimer0Regs.TCR.all=0x4001;
}


