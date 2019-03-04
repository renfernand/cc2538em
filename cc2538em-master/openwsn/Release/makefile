################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include firmware/openos/projects/common/03oos_openwsn/subdir.mk
-include firmware/openos/openwsn/cross-layers/subdir.mk
-include firmware/openos/openwsn/07-App/udpstorm/subdir.mk
-include firmware/openos/openwsn/07-App/udprand/subdir.mk
-include firmware/openos/openwsn/07-App/udpprint/subdir.mk
-include firmware/openos/openwsn/07-App/udplatency/subdir.mk
-include firmware/openos/openwsn/07-App/udpinject/subdir.mk
-include firmware/openos/openwsn/07-App/udpecho/subdir.mk
-include firmware/openos/openwsn/07-App/tcpprint/subdir.mk
-include firmware/openos/openwsn/07-App/tcpinject/subdir.mk
-include firmware/openos/openwsn/07-App/tcpecho/subdir.mk
-include firmware/openos/openwsn/07-App/rwellknown/subdir.mk
-include firmware/openos/openwsn/07-App/rrt/subdir.mk
-include firmware/openos/openwsn/07-App/rreg/subdir.mk
-include firmware/openos/openwsn/07-App/rleds/subdir.mk
-include firmware/openos/openwsn/07-App/rinfo/subdir.mk
-include firmware/openos/openwsn/07-App/rex/subdir.mk
-include firmware/openos/openwsn/07-App/r6t/subdir.mk
-include firmware/openos/openwsn/07-App/ohlone/subdir.mk
-include firmware/openos/openwsn/07-App/layerdebug/subdir.mk
-include firmware/openos/openwsn/04-TRAN/subdir.mk
-include firmware/openos/openwsn/03b-IPv6/subdir.mk
-include firmware/openos/openwsn/03a-IPHC/subdir.mk
-include firmware/openos/openwsn/02b-MAChigh/subdir.mk
-include firmware/openos/openwsn/02a-MAClow/subdir.mk
-include firmware/openos/openwsn/subdir.mk
-include firmware/openos/kernel/openos/subdir.mk
-include firmware/openos/drivers/common/subdir.mk
-include firmware/openos/bsp/boards/cc2538em/source/subdir.mk
-include firmware/openos/bsp/boards/cc2538em/sens_itf/subdir.mk
-include firmware/openos/bsp/boards/cc2538em/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
openwsn_rsc.hex \

SECONDARY_SIZE += \
openwsn_rsc.siz \


# All Target
all: openwsn_rsc.elf secondary-outputs

# Tool invocations
openwsn_rsc.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"openwsn_rsc.map" --specs=nano.specs -o "openwsn_rsc.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

openwsn_rsc.hex: openwsn_rsc.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "openwsn_rsc.elf"  "openwsn_rsc.hex"
	@echo 'Finished building: $@'
	@echo ' '

openwsn_rsc.siz: openwsn_rsc.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "openwsn_rsc.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) openwsn_rsc.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
