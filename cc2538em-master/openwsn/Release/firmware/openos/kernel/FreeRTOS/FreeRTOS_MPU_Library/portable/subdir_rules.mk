################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable/heap_2.obj: ../firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable/heap_2.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -O2 --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path= --include_path= --include_path= --define=ccs="ccs" --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable/heap_2.pp" --obj_directory="firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable/port.obj: ../firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable/port.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -O2 --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path= --include_path= --include_path= --define=ccs="ccs" --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable/port.pp" --obj_directory="firmware/openos/kernel/FreeRTOS/FreeRTOS_MPU_Library/portable" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

