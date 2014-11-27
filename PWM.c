/*
 * PWM.c
 *
 *  Created on: 2014-11-19
 *      Author: ZhengJunyuan
 */
#include "DSP28x_Project.h"
//#include "IQmathLib.h"
#include "math.h"

#define	EPWM1_TIMER_TBPRD		3750
#define	EPWM2_TIMER_TBPRD		3750

void InitEPwm1Example(void);
void InitEPwm2Example(void);
void InitTimer(void);
float vg(void);

interrupt void cpu_timer0_isr(void);
interrupt void epwm1_timer_isr(void);
interrupt void epwm2_timer_isr(void);

float v;
Uint32 kt;
float TonC[400] = {0};


void main(void)
{
   InitSysCtrl();
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   EALLOW;

   PieVectTable.TINT0 = &cpu_timer0_isr;
   PieVectTable.EPWM1_INT = &epwm1_timer_isr;
   PieVectTable.EPWM2_INT = &epwm2_timer_isr;
   EDIS;

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
   EDIS;

   InitTimer();
   InitEPwm1Gpio();
   InitEPwm2Gpio();
   InitEPwm1Example();
   InitEPwm2Example();

   EALLOW;
   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
   EDIS;

   IER|=M_INT1;
   IER|=M_INT3;

   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
   PieCtrlRegs.PIEIER3.bit.INTx2 = 1;

   EINT;
   ERTM;

   for(;;)
   {
       asm("   NOP");
   }
}

void InitTimer(void)
{
	InitCpuTimers();
	#if(CPU_FRQ_150MHZ)
	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// 150MHz CPU Freq, 1 uSecond Period
	ConfigCpuTimer(&CpuTimer0,150,100000);
	#endif
	#if(CPU_FRQ_100MHZ)
	// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
	// 100MHz CPU Freq, 1 uSecond Period
	ConfigCpuTimer(&CpuTimer0,100,100000);
	#endif
	CpuTimer0Regs.TCR.all=0x4001;
}
void InitEPwm1Example()
{
   EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;
   EPwm1Regs.TBCTR = 0x0000;

   EPwm1Regs.CMPA.half.CMPA = 1500;
   EPwm1Regs.CMPB = 1500;

   EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
   EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
   EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
   EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

   EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
   EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

   EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
   EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
   EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
   EPwm1Regs.AQCTLB.bit.CAD = AQ_CLEAR;

   EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
   EPwm1Regs.ETSEL.bit.INTEN = 1;
   EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;

   EPwm1Regs.DBCTL.all = 0xb;
   EPwm1Regs.DBRED = 60;
   EPwm1Regs.DBFED = 60;
}
void InitEPwm2Example()
{
   EPwm2Regs.TBPRD = EPWM2_TIMER_TBPRD;
   EPwm2Regs.TBPHS.half.TBPHS = 0x0000;
   EPwm2Regs.TBCTR = 0x0000;

   EPwm2Regs.CMPA.half.CMPA =1500;
   EPwm2Regs.CMPB = 1500;

   EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
   EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;
   EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
   EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

   EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
   EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

   EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
   EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
   EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
   EPwm2Regs.AQCTLB.bit.CAD = AQ_CLEAR;

   EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
   EPwm2Regs.ETSEL.bit.INTEN = 1;
   EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;

   EPwm1Regs.DBCTL.all = 0xb;
   EPwm1Regs.DBRED = 60;
   EPwm1Regs.DBFED = 60;
}
interrupt void epwm1_timer_isr(void)
{
	static int k=0;
	if(k >= 400)
	{
		k = 0;
	}
		EPwm1Regs.CMPA.half.CMPA = (TonC[k]/100+0.5)*3750;
		EPwm1Regs.CMPB = (TonC[k]/100+0.5)*3750;
		k++;
		EPwm1Regs.ETCLR.bit.INT = 1;
		PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
interrupt void epwm2_timer_isr(void)
{
	static int k=0;
   if(k >= 400)
   {
		k = 0;
   }
		EPwm2Regs.CMPA.half.CMPA = (TonC[k]/100+0.5)*3750;
		EPwm2Regs.CMPB = (TonC[k]/100+0.5)*3750;
		k++;
		EPwm2Regs.ETCLR.bit.INT = 1;
		PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

interrupt void cpu_timer0_isr(void)
{
	static int j=0;
	while(j<400)
	{
		TonC[j]=vg();
		j++;
	}
	j=0;
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP1;
}

float vg(void)
{
	kt++;
	if(kt*50/20000>=1)
		kt=0;
	v=30*sqrt(2)*sin(2*3.14159*50*kt/20000);
	return v;
}





