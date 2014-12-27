#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"

void TimerInit(void)//Timer0��ʼ���������ж����ã���ʱ1us
{
	InitCpuTimers();
#if (CPU_FRQ_150MHZ)
ConfigCpuTimer(&CpuTimer0,150,Timer0);
//ConfigCpuTimer(&CpuTimer1,150,Timer1);
#endif
#if (CPU_FRQ_100MHZ)
ConfigCpuTimer(&CpuTimer0,100,Timer0);
//ConfigCpuTimer(&CpuTimer1,100,Timer1);
#endif
    CpuTimer0Regs.TCR.all=0x4001;
//    CpuTimer1Regs.TCR.all=0x4001;
}



