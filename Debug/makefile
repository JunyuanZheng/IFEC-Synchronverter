################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := c:/ti/ccsv6/tools/compiler/c2000_6.2.7

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./PWM_ADC.obj" \
"./PWM_CoreCalculate.obj" \
"./PWM_Main.obj" \
"./PWM_Parameter.obj" \
"./PWM_SCI.obj" \
"./PWM_SupportCalculate.obj" \
"./PWM_Timer.obj" \
"./PWM_ePWM.obj" \
"./source/DSP2833x_ADC_cal.obj" \
"./source/DSP2833x_Adc.obj" \
"./source/DSP2833x_CSMPasswords.obj" \
"./source/DSP2833x_CodeStartBranch.obj" \
"./source/DSP2833x_CpuTimers.obj" \
"./source/DSP2833x_DBGIER.obj" \
"./source/DSP2833x_DMA.obj" \
"./source/DSP2833x_DefaultIsr.obj" \
"./source/DSP2833x_DisInt.obj" \
"./source/DSP2833x_ECan.obj" \
"./source/DSP2833x_ECap.obj" \
"./source/DSP2833x_EPwm.obj" \
"./source/DSP2833x_EQep.obj" \
"./source/DSP2833x_GlobalVariableDefs.obj" \
"./source/DSP2833x_Gpio.obj" \
"./source/DSP2833x_I2C.obj" \
"./source/DSP2833x_Mcbsp.obj" \
"./source/DSP2833x_MemCopy.obj" \
"./source/DSP2833x_PieCtrl.obj" \
"./source/DSP2833x_PieVect.obj" \
"./source/DSP2833x_Sci.obj" \
"./source/DSP2833x_Spi.obj" \
"./source/DSP2833x_SysCtrl.obj" \
"./source/DSP2833x_Xintf.obj" \
"./source/DSP2833x_usDelay.obj" \
"./source/FPUmathTables.obj" \
"./source/cos_f32.obj" \
"./source/sin_f32.obj" \
"./source/sqrt_f32.obj" \
"../source/DSP2833x_Headers_nonBIOS.cmd" \
-l"C:/ti/ccsv6/tools/compiler/c2000_6.2.7/lib/rts2800_fpu32_fast_supplement.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include source/subdir_vars.mk
-include subdir_rules.mk
-include source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Inverter.out \

EXE_OUTPUTS__QUOTED += \
"Inverter.out" \

BIN_OUTPUTS += \
Inverter.hex \

BIN_OUTPUTS__QUOTED += \
"Inverter.hex" \


# All Target
all: Inverter.out

# Tool invocations
Inverter.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"c:/ti/ccsv6/tools/compiler/c2000_6.2.7/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --diag_warning=225 --display_error_number --diag_wrap=off -z -m"Inverter.map" --stack_size=0x300 --warn_sections -i"c:/ti/ccsv6/tools/compiler/c2000_6.2.7/lib" -i"c:/ti/ccsv6/tools/compiler/c2000_6.2.7/include" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="Inverter_linkInfo.xml" --rom_model -o "Inverter.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Inverter.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"c:/ti/ccsv6/tools/compiler/c2000_6.2.7/bin/hex2000"  -o "Inverter.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "PWM_ADC.pp" "PWM_CoreCalculate.pp" "PWM_Main.pp" "PWM_Parameter.pp" "PWM_SCI.pp" "PWM_SupportCalculate.pp" "PWM_Timer.pp" "PWM_ePWM.pp" "source\DSP2833x_Adc.pp" "source\DSP2833x_CpuTimers.pp" "source\DSP2833x_DMA.pp" "source\DSP2833x_DefaultIsr.pp" "source\DSP2833x_ECan.pp" "source\DSP2833x_ECap.pp" "source\DSP2833x_EPwm.pp" "source\DSP2833x_EQep.pp" "source\DSP2833x_GlobalVariableDefs.pp" "source\DSP2833x_Gpio.pp" "source\DSP2833x_I2C.pp" "source\DSP2833x_Mcbsp.pp" "source\DSP2833x_MemCopy.pp" "source\DSP2833x_PieCtrl.pp" "source\DSP2833x_PieVect.pp" "source\DSP2833x_Sci.pp" "source\DSP2833x_Spi.pp" "source\DSP2833x_SysCtrl.pp" "source\DSP2833x_Xintf.pp" 
	-$(RM) "PWM_ADC.obj" "PWM_CoreCalculate.obj" "PWM_Main.obj" "PWM_Parameter.obj" "PWM_SCI.obj" "PWM_SupportCalculate.obj" "PWM_Timer.obj" "PWM_ePWM.obj" "source\DSP2833x_ADC_cal.obj" "source\DSP2833x_Adc.obj" "source\DSP2833x_CSMPasswords.obj" "source\DSP2833x_CodeStartBranch.obj" "source\DSP2833x_CpuTimers.obj" "source\DSP2833x_DBGIER.obj" "source\DSP2833x_DMA.obj" "source\DSP2833x_DefaultIsr.obj" "source\DSP2833x_DisInt.obj" "source\DSP2833x_ECan.obj" "source\DSP2833x_ECap.obj" "source\DSP2833x_EPwm.obj" "source\DSP2833x_EQep.obj" "source\DSP2833x_GlobalVariableDefs.obj" "source\DSP2833x_Gpio.obj" "source\DSP2833x_I2C.obj" "source\DSP2833x_Mcbsp.obj" "source\DSP2833x_MemCopy.obj" "source\DSP2833x_PieCtrl.obj" "source\DSP2833x_PieVect.obj" "source\DSP2833x_Sci.obj" "source\DSP2833x_Spi.obj" "source\DSP2833x_SysCtrl.obj" "source\DSP2833x_Xintf.obj" "source\DSP2833x_usDelay.obj" "source\FPUmathTables.obj" "source\cos_f32.obj" "source\sin_f32.obj" "source\sqrt_f32.obj" 
	-$(RM) "source\DSP2833x_ADC_cal.pp" "source\DSP2833x_CSMPasswords.pp" "source\DSP2833x_CodeStartBranch.pp" "source\DSP2833x_DBGIER.pp" "source\DSP2833x_DisInt.pp" "source\DSP2833x_usDelay.pp" "source\FPUmathTables.pp" "source\cos_f32.pp" "source\sin_f32.pp" "source\sqrt_f32.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

