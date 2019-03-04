################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include firmware/projects/common/03oos_openwsn/subdir.mk
-include firmware/openstack/cross-layers/subdir.mk
-include firmware/openstack/04-TRAN/subdir.mk
-include firmware/openstack/03b-IPv6/subdir.mk
-include firmware/openstack/03a-IPHC/subdir.mk
-include firmware/openstack/02b-MAChigh/subdir.mk
-include firmware/openstack/02a-MAClow/subdir.mk
-include firmware/openstack/subdir.mk
-include firmware/openapps/userialbridge/subdir.mk
-include firmware/openapps/uinject/subdir.mk
-include firmware/openapps/uexpiration_monitor/subdir.mk
-include firmware/openapps/uexpiration/subdir.mk
-include firmware/openapps/uecho/subdir.mk
-include firmware/openapps/rrt/subdir.mk
-include firmware/openapps/osens/subdir.mk
-include firmware/openapps/opencoap/subdir.mk
-include firmware/openapps/cwellknown/subdir.mk
-include firmware/openapps/cstorm/subdir.mk
-include firmware/openapps/cleds/subdir.mk
-include firmware/openapps/cjoin/subdir.mk
-include firmware/openapps/cinfo/subdir.mk
-include firmware/openapps/c6t/subdir.mk
-include firmware/openapps/subdir.mk
-include firmware/kernel/openos/subdir.mk
-include firmware/drivers/common/subdir.mk
-include firmware/bsp/boards/common/subdir.mk
-include firmware/bsp/boards/cc2538em/source/subdir.mk
-include firmware/bsp/boards/cc2538em/osens_itf/subdir.mk
-include firmware/bsp/boards/cc2538em/subdir.mk
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
openwsn.hex \

SECONDARY_SIZE += \
openwsn.siz \


# All Target
all: openwsn.elf secondary-outputs

# Tool invocations
openwsn.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T "C:\owsn_fw_r1_22_1\openwsn\ldscripts\mem.ld" -T "C:\owsn_fw_r1_22_1\openwsn\ldscripts\libs.ld" -T "C:\owsn_fw_r1_22_1\openwsn\ldscripts\sections.ld" -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"openwsn.map" --specs=nano.specs -o "openwsn.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

openwsn.hex: openwsn.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "openwsn.elf"  "openwsn.hex"
	@echo 'Finished building: $@'
	@echo ' '

openwsn.siz: openwsn.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "openwsn.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) openwsn.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
