#include "DSP28x_Project.h"
#include "C28x_FPU_FastRTS.h"
#include "PWM_Function.h"
#include "PWM_Parameter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//输入性GPIO设置
void Gpio_setup1(void)
{
	//GPIO67
	EALLOW;
	GpioCtrlRegs.GPCMUX1.bit.GPIO67=0; //数字I/O模式
	GpioCtrlRegs.GPCPUD.bit.GPIO67=0; //内部上拉电阻
	GpioCtrlRegs.GPCDIR.bit.GPIO67=0; //配置为输入方向
	EDIS;

	//GPIO68
	EALLOW;
	GpioCtrlRegs.GPCMUX1.bit.GPIO68=0; //数字I/O模式
	GpioCtrlRegs.GPCPUD.bit.GPIO68=0; //内部上拉电阻
	GpioCtrlRegs.GPCDIR.bit.GPIO68=0; //配置为输入方向
	EDIS;

	//GPIO69
	EALLOW;
	GpioCtrlRegs.GPCMUX1.bit.GPIO69=0; //数字I/O模式
	GpioCtrlRegs.GPCPUD.bit.GPIO69=0; //内部上拉电阻
	GpioCtrlRegs.GPCDIR.bit.GPIO69=0; //配置为输入方向
	EDIS;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//输出性GPIO设置
void Gpio_setup2(void)
{
	//GPIO04
	EALLOW;
	GpioCtrlRegs.GPAMUX1.bit.GPIO4=0;
	GpioCtrlRegs.GPAPUD.bit.GPIO4=0; //内部上拉电阻
	GpioCtrlRegs.GPADIR.bit.GPIO4=1; //配置为输出方向
	EDIS;
}
