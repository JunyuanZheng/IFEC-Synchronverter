#include "DSP28x_Project.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Timer设置
void TimerInit(void)
{
	InitCpuTimers();
#if(CPU_FRQ_150MHZ)
	ConfigCpuTimer(&CpuTimer0,150,1000000); //定时1s
#endif
#if(CPU_FRQ_100MHZ)
	ConfigCpuTimer(&CpuTimer0,100,1000000); //定时1s
#endif
	CpuTimer0Regs.TCR.all=0x4001;
}



