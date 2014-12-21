/*
// TI File $Revision: /main/8 $
// Checkin $Date: June 2, 2008   11:12:24 $
//###########################################################################
//
// FILE:    DSP2833x_Headers_nonBIOS.cmd
//
// TITLE:   DSP2833x Peripheral registers linker command file 
//
// DESCRIPTION: 
// 
//          This file is for use in Non-BIOS applications.
//
//          Linker command file to place the peripheral structures 
//          used within the DSP2833x headerfiles into the correct memory
//          mapped locations.
//
//          This version of the file includes the PieVectorTable structure.
//          For BIOS applications, please use the DSP2833x_Headers_BIOS.cmd file
//          which does not include the PieVectorTable structure.
//
//###########################################################################
*/

MEMORY
{
 PAGE 0:    /* Program Memory */

   BEGIN      : origin = 0x000000, length = 0x000002     /* Boot to M0 will go here                      */
   RAMM0      : origin = 0x000050, length = 0x0003B0
   RAML0      : origin = 0x008000, length = 0x000600
   RAML1      : origin = 0x008600, length = 0x001A00
   RAML2      : origin = 0x00A000, length = 0x001000
   RAML3      : origin = 0x00B000, length = 0x001000
   ZONE7A     : origin = 0x200000, length = 0x00FC00     /* XINTF zone 7 - program space */
   CSM_RSVD   : origin = 0x33FF80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   CSM_PWL    : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA            */
   ADC_CAL    : origin = 0x380080, length = 0x000009
   RESET      : origin = 0x3FFFC0, length = 0x000002
   IQTABLES   : origin = 0x3FE000, length = 0x000b50
   IQTABLES2  : origin = 0x3FEB50, length = 0x00008c
   FPUTABLES  : origin = 0x3FEBDC, length = 0x0006A0
   BOOTROM    : origin = 0x3FF27C, length = 0x000D44

 PAGE 1:    /* Data Memory */
 
   BOOT_RSVD  : origin = 0x000002, length = 0x00004E     /* Part of M0, BOOT rom will use this for stack */
   RAMM1      : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
   RAML4      : origin = 0x00C000, length = 0x001000
   RAML5      : origin = 0x00D000, length = 0x001000
   RAML6      : origin = 0x00E000, length = 0x001000
   RAML7      : origin = 0x00F000, length = 0x001000
   ZONE7B     : origin = 0x20FC00, length = 0x000400     /* XINTF zone 7 - data space */

 PAGE 2:

   DEV_EMU     : origin = 0x000880, length = 0x000180     /* device emulation registers */
   FLASH_REGS  : origin = 0x000A80, length = 0x000060     /* FLASH registers */
   CSM         : origin = 0x000AE0, length = 0x000010     /* code security module registers */
  
   ADC_MIRROR  : origin = 0x000B00, length = 0x000010     /* ADC Results register mirror */

   XINTF       : origin = 0x000B20, length = 0x000020     /* external interface registers */
   
   CPU_TIMER0  : origin = 0x000C00, length = 0x000008     /* CPU Timer0 registers */
   CPU_TIMER1  : origin = 0x000C08, length = 0x000008     /* CPU Timer0 registers (CPU Timer1 & Timer2 reserved TI use)*/
   CPU_TIMER2  : origin = 0x000C10, length = 0x000008     /* CPU Timer0 registers (CPU Timer1 & Timer2 reserved TI use)*/

   PIE_CTRL    : origin = 0x000CE0, length = 0x000020     /* PIE control registers */
   PIE_VECT    : origin = 0x000D00, length = 0x000100     /* PIE Vector Table */

   DMA         : origin = 0x001000, length = 0x000200     /* DMA registers */

   MCBSPA      : origin = 0x005000, length = 0x000040     /* McBSP-A registers */
   MCBSPB      : origin = 0x005040, length = 0x000040     /* McBSP-B registers */

   ECANA       : origin = 0x006000, length = 0x000040     /* eCAN-A control and status registers */ 
   ECANA_LAM   : origin = 0x006040, length = 0x000040     /* eCAN-A local acceptance masks */
   ECANA_MOTS  : origin = 0x006080, length = 0x000040     /* eCAN-A message object time stamps */
   ECANA_MOTO  : origin = 0x0060C0, length = 0x000040     /* eCAN-A object time-out registers */
   ECANA_MBOX  : origin = 0x006100, length = 0x000100     /* eCAN-A mailboxes */

   ECANB       : origin = 0x006200, length = 0x000040     /* eCAN-B control and status registers */ 
   ECANB_LAM   : origin = 0x006240, length = 0x000040     /* eCAN-B local acceptance masks */
   ECANB_MOTS  : origin = 0x006280, length = 0x000040     /* eCAN-B message object time stamps */
   ECANB_MOTO  : origin = 0x0062C0, length = 0x000040     /* eCAN-B object time-out registers */
   ECANB_MBOX  : origin = 0x006300, length = 0x000100     /* eCAN-B mailboxes */

   EPWM1       : origin = 0x006800, length = 0x000022     /* Enhanced PWM 1 registers */
   EPWM2       : origin = 0x006840, length = 0x000022     /* Enhanced PWM 2 registers */
   EPWM3       : origin = 0x006880, length = 0x000022     /* Enhanced PWM 3 registers */
   EPWM4       : origin = 0x0068C0, length = 0x000022     /* Enhanced PWM 4 registers */
   EPWM5       : origin = 0x006900, length = 0x000022     /* Enhanced PWM 5 registers */
   EPWM6       : origin = 0x006940, length = 0x000022     /* Enhanced PWM 6 registers */

   ECAP1       : origin = 0x006A00, length = 0x000020     /* Enhanced Capture 1 registers */
   ECAP2       : origin = 0x006A20, length = 0x000020     /* Enhanced Capture 2 registers */
   ECAP3       : origin = 0x006A40, length = 0x000020     /* Enhanced Capture 3 registers */
   ECAP4       : origin = 0x006A60, length = 0x000020     /* Enhanced Capture 4 registers */         
   ECAP5       : origin = 0x006A80, length = 0x000020     /* Enhanced Capture 5 registers */         
   ECAP6       : origin = 0x006AA0, length = 0x000020     /* Enhanced Capture 6 registers */         
 
   EQEP1       : origin = 0x006B00, length = 0x000040     /* Enhanced QEP 1 registers */
   EQEP2       : origin = 0x006B40, length = 0x000040     /* Enhanced QEP 2 registers */   

   GPIOCTRL    : origin = 0x006F80, length = 0x000040     /* GPIO control registers */
   GPIODAT     : origin = 0x006FC0, length = 0x000020     /* GPIO data registers */
   GPIOINT     : origin = 0x006FE0, length = 0x000020     /* GPIO interrupt/LPM registers */
                 
   SYSTEM      : origin = 0x007010, length = 0x000020     /* System control registers */
   SPIA        : origin = 0x007040, length = 0x000010     /* SPI-A registers */
   SCIA        : origin = 0x007050, length = 0x000010     /* SCI-A registers */
   XINTRUPT    : origin = 0x007070, length = 0x000010     /* external interrupt registers */

   ADC         : origin = 0x007100, length = 0x000020     /* ADC registers */

   SCIB        : origin = 0x007750, length = 0x000010     /* SCI-B registers */

   SCIC        : origin = 0x007770, length = 0x000010     /* SCI-C registers */
   
   I2CA        : origin = 0x007900, length = 0x000040     /* I2C-A registers */
   
   CSM_PWL     : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA.  CSM password locations. */

   PARTID      : origin = 0x380090, length = 0x000001     /* Part ID register location */
}

 
SECTIONS
{
  /* Setup for "boot to SARAM" mode:
      The codestart section (found in DSP28_CodeStartBranch.asm)
      re-directs execution to the start of user code.  */
   codestart        : > BEGIN,     PAGE = 0
   ramfuncs         : > RAML0,     PAGE = 0
   .text            : > RAML1,     PAGE = 0
   .cinit           : > RAML0,     PAGE = 0
   .pinit           : > RAML0,     PAGE = 0
   .switch          : > RAML0,     PAGE = 0

   .stack           : > RAMM1,     PAGE = 1
   .ebss            : > RAML4,     PAGE = 1
   .econst          : > RAML5,     PAGE = 1
   .esysmem         : > RAMM1,     PAGE = 1

   Tedb     align(0x100) > RAML0   PAGE = 0
   Qdb      align(0x100) > RAML0   PAGE = 0
   TeFilter	align(0x100) > RAML1   PAGE = 0
   QFilter	align(0x100) > RAML1   PAGE = 0
   coefffilt align(0x100)> RAML2   PAGE = 0

   IQmath           : > RAML1,     PAGE = 0
   IQmathTables     : > IQTABLES,  PAGE = 0, TYPE = NOLOAD

   /* Uncomment the section below if calling the IQNexp() or IQexp()
      functions from the IQMath.lib library in order to utilize the
      relevant IQ Math table in Boot ROM (This saves space and Boot ROM
      is 1 wait-state). If this section is not uncommented, IQmathTables2
      will be loaded into other memory (SARAM, Flash, etc.) and will take
      up space, but 0 wait-state is possible.
   */
   /*
   IQmathTables2    : > IQTABLES2, PAGE = 0, TYPE = NOLOAD
   {

              IQmath.lib<IQNexpTable.obj> (IQmathTablesRam)

   }
   */
   IQmathTablesRam  : > RAML4,     PAGE = 1
   FPUmathTables    : > FPUTABLES, PAGE = 0, TYPE = NOLOAD

   DMARAML4         : > RAML4,     PAGE = 1
   DMARAML5         : > RAML5,     PAGE = 1
   DMARAML6         : > RAML6,     PAGE = 1
   DMARAML7         : > RAML7,     PAGE = 1

   ZONE7DATA        : > ZONE7B,    PAGE = 1

   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used                    */
   csm_rsvd         : > CSM_RSVD   PAGE = 0, TYPE = DSECT /* not used for SARAM examples */
   csmpasswds       : > CSM_PWL    PAGE = 0, TYPE = DSECT /* not used for SARAM examples */

   /* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
   .adc_cal         : load = ADC_CAL,   PAGE = 0, TYPE = NOLOAD

   PieVectTableFile : > PIE_VECT,   PAGE = 2

/*** Peripheral Frame 0 Register Structures ***/
   DevEmuRegsFile    : > DEV_EMU,     PAGE = 2
   FlashRegsFile     : > FLASH_REGS,  PAGE = 2
   CsmRegsFile       : > CSM,         PAGE = 2
   AdcMirrorFile     : > ADC_MIRROR,  PAGE = 2
   XintfRegsFile     : > XINTF,       PAGE = 2
   CpuTimer0RegsFile : > CPU_TIMER0,  PAGE = 2
   CpuTimer1RegsFile : > CPU_TIMER1,  PAGE = 2
   CpuTimer2RegsFile : > CPU_TIMER2,  PAGE = 2
   PieCtrlRegsFile   : > PIE_CTRL,    PAGE = 2
   DmaRegsFile       : > DMA,         PAGE = 2

/*** Peripheral Frame 3 Register Structures ***/
   McbspaRegsFile    : > MCBSPA,      PAGE = 2
   McbspbRegsFile    : > MCBSPB,      PAGE = 2

/*** Peripheral Frame 1 Register Structures ***/
   ECanaRegsFile     : > ECANA,       PAGE = 2
   ECanaLAMRegsFile  : > ECANA_LAM    PAGE = 2
   ECanaMboxesFile   : > ECANA_MBOX   PAGE = 2
   ECanaMOTSRegsFile : > ECANA_MOTS   PAGE = 2
   ECanaMOTORegsFile : > ECANA_MOTO   PAGE = 2
   
   ECanbRegsFile     : > ECANB,       PAGE = 2
   ECanbLAMRegsFile  : > ECANB_LAM    PAGE = 2
   ECanbMboxesFile   : > ECANB_MBOX   PAGE = 2
   ECanbMOTSRegsFile : > ECANB_MOTS   PAGE = 2
   ECanbMOTORegsFile : > ECANB_MOTO   PAGE = 2
   
   EPwm1RegsFile     : > EPWM1        PAGE = 2
   EPwm2RegsFile     : > EPWM2        PAGE = 2
   EPwm3RegsFile     : > EPWM3        PAGE = 2
   EPwm4RegsFile     : > EPWM4        PAGE = 2
   EPwm5RegsFile     : > EPWM5        PAGE = 2
   EPwm6RegsFile     : > EPWM6        PAGE = 2
   
   ECap1RegsFile     : > ECAP1        PAGE = 2
   ECap2RegsFile     : > ECAP2        PAGE = 2
   ECap3RegsFile     : > ECAP3        PAGE = 2
   ECap4RegsFile     : > ECAP4        PAGE = 2
   ECap5RegsFile     : > ECAP5        PAGE = 2
   ECap6RegsFile     : > ECAP6        PAGE = 2

   EQep1RegsFile     : > EQEP1        PAGE = 2
   EQep2RegsFile     : > EQEP2        PAGE = 2

   GpioCtrlRegsFile  : > GPIOCTRL     PAGE = 2
   GpioDataRegsFile  : > GPIODAT      PAGE = 2
   GpioIntRegsFile   : > GPIOINT      PAGE = 2
   
/*** Peripheral Frame 2 Register Structures ***/
   SysCtrlRegsFile   : > SYSTEM,      PAGE = 2
   SpiaRegsFile      : > SPIA,        PAGE = 2
   SciaRegsFile      : > SCIA,        PAGE = 2
   XIntruptRegsFile  : > XINTRUPT,    PAGE = 2
   AdcRegsFile       : > ADC,         PAGE = 2
   ScibRegsFile      : > SCIB,        PAGE = 2
   ScicRegsFile      : > SCIC,        PAGE = 2
   I2caRegsFile      : > I2CA,        PAGE = 2
              
/*** Code Security Module Register Structures ***/
   CsmPwlFile        : > CSM_PWL,     PAGE = 2

/*** Device Part ID Register Structures ***/
   PartIdRegsFile    : > PARTID,      PAGE = 2

}


/*
//===========================================================================
// End of file.
//===========================================================================
*/
