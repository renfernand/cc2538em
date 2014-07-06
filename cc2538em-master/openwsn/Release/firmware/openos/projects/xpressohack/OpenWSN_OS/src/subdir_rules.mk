################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
firmware/openos/projects/xpressohack/OpenWSN_OS/src/cr_startup_lpc17.obj: ../firmware/openos/projects/xpressohack/OpenWSN_OS/src/cr_startup_lpc17.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -O2 --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path= --include_path= --include_path= --define=ccs="ccs" --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="firmware/openos/projects/xpressohack/OpenWSN_OS/src/cr_startup_lpc17.pp" --obj_directory="firmware/openos/projects/xpressohack/OpenWSN_OS/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

firmware/openos/projects/xpressohack/OpenWSN_OS/src/main.obj: ../firmware/openos/projects/xpressohack/OpenWSN_OS/src/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -O2 --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.5/include" --include_path= --include_path= --include_path= --define=ccs="ccs" --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="firmware/openos/projects/xpressohack/OpenWSN_OS/src/main.pp" --obj_directory="firmware/openos/projects/xpressohack/OpenWSN_OS/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


