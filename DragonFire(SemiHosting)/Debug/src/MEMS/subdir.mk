################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MEMS/MPU_6050.c 

OBJS += \
./src/MEMS/MPU_6050.o 

C_DEPS += \
./src/MEMS/MPU_6050.d 


# Each subdirectory must supply rules for building sources it contributes
src/MEMS/%.o: ../src/MEMS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/CMSISv2p00_LPC11Uxx/inc" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/DragonFire(SemiHosting)/src/lib" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/DragonFire(SemiHosting)/src/SDCard/src" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/DragonFire(SemiHosting)/src/MEMS" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


