#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCI初始化，波特率1171875
void InitSCI()
{
	InitSciaGpio(); //初始化用到GPIO引脚为SCI外设模式
	SciaRegs.SCICCR.all=0x0007; //1位停止位，静止内部贿赂功能，无奇偶校验，空前模式，数据长度8位
	SciaRegs.SCICTL1.all=0x0003; //使能TX，RX，使能SCICLK，禁止RX ERR SLEEP TXWAKE
	#if (CPU_FRQ_150MHZ)
	SciaRegs.SCIHBAUD=0x0000;  // 1171875 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD=0x0003;
    #endif
	SciaRegs.SCICTL2.bit.TXINTENA=1; //使能标准发送中断
	SciaRegs.SCICTL2.bit.RXBKINTENA=1; //使能标准接收中断
	SciaRegs.SCICTL1.bit.SWRESET=1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////SCI发送数据
void sci_send(Uint16 input)
{
	SciaRegs.SCITXBUF=input;
	while(SciaRegs.SCICTL2.bit.TXRDY==0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCI发送中断
interrupt void sciaTx_isr(void)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCI接收中断
interrupt void sciaRx_isr(void)
{

}
