################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/lib/subdir.mk
-include src/SDCard/src/subdir.mk
-include src/MEMS/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: DragonFire(SemiHosting).axf

# Tool invocations
DragonFire(SemiHosting).axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/CMSISv2p00_LPC11Uxx/Debug" -Xlinker -Map="DragonFire(SemiHosting).map" -Xlinker --gc-sections -mcpu=cortex-m0 -mthumb -T "DragonFire_SemiHosting_Debug.ld" -o "DragonFire(SemiHosting).axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) DragonFire(SemiHosting).axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "DragonFire(SemiHosting).axf"; # arm-none-eabi-objcopy -O binary "DragonFire(SemiHosting).axf" "DragonFire(SemiHosting).bin" ; checksum -p LPC11U14_201 -d "DragonFire(SemiHosting).bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
