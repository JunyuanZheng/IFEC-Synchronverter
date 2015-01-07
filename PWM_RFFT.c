#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "PWM_Function.h"
#include "PWM_Parameter.h"
#include "C28x_FPU_FastRTS.h"
#include "FPU.h"

#define	RFFT_STAGES	8 //7
#define	RFFT_SIZE	(1 << RFFT_STAGES) //128

#pragma DATA_SECTION(RFFTinBuff,"RFFTdata1");  //Buffer alignment for the input array,
float32 RFFTinBuff[RFFT_SIZE];                 //RFFT_f32u(optional), RFFT_f32(required)
                                                //Output of FFT overwrites input if
                                                //RFFT_STAGES is ODD
#pragma DATA_SECTION(RFFToutBuff,"RFFTdata2");
float32 RFFToutBuff[RFFT_SIZE];                 //Output of FFT here if RFFT_STAGES is EVEN

#pragma DATA_SECTION(RFFTF32Coef,"RFFTdata3");
float32 RFFTF32Coef[RFFT_SIZE];                 //Twiddle buffer

#pragma DATA_SECTION(RFFTMagBuff,"RFFTdata4");
float32 RFFTmagBuff[RFFT_SIZE/2+1];             //Additional Buffer used in Magnitude calc

#pragma DATA_SECTION(RFFTphaseBuff,"RFFTdata5");
float32 RFFTphaseBuff[RFFT_SIZE/2+1];             //Additional Buffer used in Phase calc

RFFT_F32_STRUCT RFFT;

void RFFTInit(void)
{
	for(Uint16 i=0;i<RFFT_SIZE;i++)
	{
		RFFTinBuff[i]=0;
	}

	 RFFT.FFTSize=RFFT_SIZE;
	 RFFT.FFTStages=RFFT_STAGES;
	 RFFT.InBuf=&RFFTinBuff[0];  //Input buffer
	 RFFT.OutBuf=&RFFToutBuff[0];  //Output buffer
	 RFFT.CosSinBuf=&RFFTF32Coef[0];  //Twiddle factor buffer
	 RFFT.MagBuf=&RFFTmagBuff[0];  //Magnitude buffer
	 RFFT.PhaseBuf=&RFFTphaseBuff[0];

	 RFFT_f32_sincostable(&RFFT);       //Calculate twiddle factor

	 for(Uint16 i=0;i<RFFT_SIZE;i++)
	 {
		 RFFToutBuff[i] = 0;			   //Clean up output buffer
	 }

	 for(Uint16 i=0;i<RFFT_SIZE/2;i++)
	 {
		 RFFTmagBuff[i] = 0;		       //Clean up magnitude buffer
	 }

	 for(Uint16 i=0;i<RFFT_SIZE/2;i++)
	 {
		 RFFTphaseBuff[i] = 0;		       //Clean up magnitude buffer
	 }
}
