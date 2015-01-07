#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCI��ʼ����������1171875
void InitSCI()
{
	InitSciaGpio(); //��ʼ���õ�GPIO����ΪSCI����ģʽ
	SciaRegs.SCICCR.all=0x0007; //1λֹͣλ����ֹ�ڲ���¸���ܣ�����żУ�飬��ǰģʽ�����ݳ���8λ
	SciaRegs.SCICTL1.all=0x0003; //ʹ��TX��RX��ʹ��SCICLK����ֹRX ERR SLEEP TXWAKE
	#if (CPU_FRQ_150MHZ)
	SciaRegs.SCIHBAUD=0x0000;  // 1171875 baud @LSPCLK = 37.5MHz.
	SciaRegs.SCILBAUD=0x0003;
    #endif
	SciaRegs.SCICTL2.bit.TXINTENA=1; //ʹ�ܱ�׼�����ж�
	SciaRegs.SCICTL2.bit.RXBKINTENA=1; //ʹ�ܱ�׼�����ж�
	SciaRegs.SCICTL1.bit.SWRESET=1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////SCI��������
void sci_send(Uint16 input)
{
	SciaRegs.SCITXBUF=input;
	while(SciaRegs.SCICTL2.bit.TXRDY==0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCI�����ж�
interrupt void sciaTx_isr(void)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCI�����ж�
interrupt void sciaRx_isr(void)
{

}
