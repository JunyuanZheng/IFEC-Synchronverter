#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCI
void SCIAInit()
{
	InitSciaGpio();
	SciaRegs.SCICCR.all=0x0007;   // 1 stop bit,  No loopback
	SciaRegs.SCICTL1.all=0x0003;  // enable TX, RX, internal SCICLK,

	#if (CPU_FRQ_150MHZ)
	SciaRegs.SCIHBAUD=0x0000;  // 1171875 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD=0x0003;
    #endif
    #if (CPU_FRQ_100MHZ)
    SciaRegs.SCIHBAUD=0x0001;  // 9600 baud @LSPCLK = 20MHz.
    SciaRegs.SCILBAUD=0x0044;
    #endif

	SciaRegs.SCICTL2.bit.TXINTENA=1;    //使能标准发送中断
	SciaRegs.SCICTL2.bit.RXBKINTENA=1;  //使能标准接收中断
	SciaRegs.SCICTL1.bit.SWRESET=1;
	SciaRegs.SCIFFTX.all=0xC000;   //使能FIFO发送中断
	SciaRegs.SCIFFRX.all=0x0031;   //使能FIFO接收中断
	SciaRegs.SCIFFCT.all=0x00;
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	GpioDataRegs.GPACLEAR.bit.GPIO23=1;  //使能485接收
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void SCIASend(Uint16 input)
{
	SciaRegs.SCITXBUF=input;
    DELAY_US(10L);//一定要有延时
}

interrupt void SCITxA_isr(void)
{

}
