################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src/core_cm3.obj: ../firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src/core_cm3.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -O2 --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path= --include_path= --include_path= --define=ccs="ccs" --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src/core_cm3.pp" --obj_directory="firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src/system_LPC17xx.obj: ../firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src/system_LPC17xx.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -O2 --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path= --include_path= --include_path= --define=ccs="ccs" --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src/system_LPC17xx.pp" --obj_directory="firmware/openos/kernel/FreeRTOS/CMSISv2p00_LPC17xx/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


