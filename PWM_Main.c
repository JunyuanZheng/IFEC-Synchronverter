#include "DSP28x_Project.h"
#include "PWM_Variable.h"
#include "PWM_Header.h"
#include "math.h"

void main(void)
{
	SystemInit();
	EALLOW;
	PieVectTable.EPWM1_INT=&EPwm1_isr;
	EDIS;
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1;
    EDIS;
    IER|=M_INT3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    EINT;
    ERTM;
    for(;;)
    	asm("   NOP");
}
interrupt void EPwm1_isr(void)
{
	ReadADC(ADCResults);
	v=vg();
	EPwm1Regs.CMPA.half.CMPA=(v/100+0.5)*EPwm1_TIMER_TBPRD;
	EPwm2Regs.CMPA.half.CMPA=(v/100+0.5)*EPwm2_TIMER_TBPRD;
	EPwm1Regs.ETCLR.bit.INT=1;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;
}

float vg(void)
{
	kt++;
	if(kt*50/20000>=1)
		kt=0;
	v=30*sqrt(2)*sin(2*3.14159*50*kt/20000);
	return v;
}





