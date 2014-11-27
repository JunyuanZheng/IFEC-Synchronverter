#include "DSP28x_Project.h"
#include "PWM_Header.h"

void InitEPwm1(void)
{
   EPwm1Regs.TBPRD = EPwm1_TIMER_TBPRD;
   EPwm1Regs.TBPHS.half.TBPHS = 0x0000;
   EPwm1Regs.TBCTR = 0x0000;

   EPwm1Regs.CMPA.half.CMPA = 3000;
   EPwm1Regs.CMPB = 3000;

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
void InitEPwm2(void)
{
   EPwm2Regs.TBPRD = EPwm1_TIMER_TBPRD;
   EPwm2Regs.TBPHS.half.TBPHS = 0x0000;
   EPwm2Regs.TBCTR = 0x0000;

   EPwm2Regs.CMPA.half.CMPA =1900;
   EPwm2Regs.CMPB = 1900;

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



