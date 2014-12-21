#include "DSP28x_Project.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EPwm1,EPwm2初始化，其中EPwm1A触发中断，EPwm2A每触发3次产生1次ADC转换信号
void InitEPwm1(void)
{
   EPwm1Regs.TBPRD=EPwm_TIMER_TBPRD;
   EPwm1Regs.TBPHS.half.TBPHS=0x0000;
   EPwm1Regs.TBCTR=0x0000;
   EPwm1Regs.CMPA.half.CMPA=0;
   EPwm1Regs.CMPB=0;
   EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
   EPwm1Regs.TBCTL.bit.PHSEN=TB_DISABLE;
   EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
   EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV1;
   EPwm1Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
   EPwm1Regs.CMPCTL.bit.LOADAMODE=CC_CTR_PRD;//待分析
   EPwm1Regs.CMPCTL.bit.LOADBMODE=CC_CTR_PRD;//待分析
   EPwm1Regs.AQCTLA.bit.CAU=AQ_CLEAR;
   EPwm1Regs.AQCTLA.bit.CAD=AQ_SET;
   EPwm1Regs.AQCTLB.bit.CAU=AQ_SET;
   EPwm1Regs.AQCTLB.bit.CAD=AQ_CLEAR;
   EPwm1Regs.ETSEL.bit.INTSEL=ET_CTR_PRD;//待分析
   EPwm1Regs.ETSEL.bit.INTEN=1;
   EPwm1Regs.ETPS.bit.INTPRD=ET_1ST;
   EPwm1Regs.ETCLR.bit.INT=1;
   EPwm1Regs.ETSEL.bit.SOCAEN=1;//设置ePWM_SOCA为ADC模块的启动信号，使能A组的启动信号SOCA
   EPwm1Regs.ETSEL.bit.SOCASEL=ET_CTR_PRD;//选择SOCA信号产生的时刻为CTR=PRD
   EPwm1Regs.ETPS.bit.SOCAPRD=ET_1ST;//每次触发事件产生一个启动信号
   EPwm1Regs.ETCLR.bit.SOCA=1;//清除SOCA标志位
   EPwm1Regs.DBCTL.all=0xb;
   EPwm1Regs.DBRED=50;
   EPwm1Regs.DBFED=50;
}
void InitEPwm2(void)
{
   EPwm2Regs.TBPRD=EPwm_TIMER_TBPRD;
   EPwm2Regs.TBPHS.half.TBPHS=0x0000;
   EPwm2Regs.TBCTR=0x0000;
   EPwm2Regs.CMPA.half.CMPA=0;
   EPwm2Regs.CMPB=0;
   EPwm2Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
   EPwm2Regs.TBCTL.bit.PHSEN=TB_DISABLE;
   EPwm2Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
   EPwm2Regs.TBCTL.bit.CLKDIV=TB_DIV1;
   EPwm2Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
   EPwm2Regs.CMPCTL.bit.LOADAMODE=CC_CTR_PRD;//待分析
   EPwm2Regs.CMPCTL.bit.LOADBMODE=CC_CTR_PRD;//待分析
   EPwm2Regs.AQCTLA.bit.CAU=AQ_CLEAR; //待测试
   EPwm2Regs.AQCTLA.bit.CAD=AQ_SET;
   EPwm2Regs.AQCTLB.bit.CAU=AQ_SET;
   EPwm2Regs.AQCTLB.bit.CAD=AQ_CLEAR;
   EPwm2Regs.DBCTL.all=0xb;
   EPwm2Regs.DBRED=50;
   EPwm2Regs.DBFED=50;
}



