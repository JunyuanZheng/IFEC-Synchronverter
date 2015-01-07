#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "FPU.h"

#define FIR_ORDER 199

#pragma DATA_SECTION(TeFIR,"TeFilter")
#pragma DATA_SECTION(QFIR,"QFilter")
FIR_FP TeFIR=FIR_FP_DEFAULTS;
FIR_FP QFIR=FIR_FP_DEFAULTS;

#pragma DATA_SECTION(Tedbuffer,"Tedb")
#pragma DATA_SECTION(Qdbuffer,"Qdb")
float Tedbuffer[FIR_ORDER+1];
float Qdbuffer[FIR_ORDER+1];

#pragma DATA_SECTION(coeff,"coefffilt");
float const coeff[FIR_ORDER+1]=FIR_FP_LPF200;

void InitFIR(void)
{
	TeFIR.order=FIR_ORDER;
	TeFIR.dbuffer_ptr=Tedbuffer;
	TeFIR.coeff_ptr=(float *)coeff;
	TeFIR.init(&TeFIR);

	QFIR.order=FIR_ORDER;
	QFIR.dbuffer_ptr=Qdbuffer;
	QFIR.coeff_ptr=(float *)coeff;
	QFIR.init(&QFIR);
}

float32 TeFIR_cal(float32 input)
{
    TeFIR.input=input;
    TeFIR.calc(&TeFIR);
    return TeFIR.output;
}

float32 QFIR_cal(float32 input)
{
    QFIR.input=input;
    QFIR.calc(&QFIR);
    return QFIR.output;
}
