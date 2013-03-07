################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SDCard/src/ff.c \
../src/SDCard/src/mmc.c 

OBJS += \
./src/SDCard/src/ff.o \
./src/SDCard/src/mmc.o 

C_DEPS += \
./src/SDCard/src/ff.d \
./src/SDCard/src/mmc.d 


# Each subdirectory must supply rules for building sources it contributes
src/SDCard/src/%.o: ../src/SDCard/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/CMSISv2p00_LPC11Uxx/inc" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/DragonFire(SemiHosting)/src/lib" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/DragonFire(SemiHosting)/src/SDCard/src" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFireV2/DragonFire(SemiHosting)/src/MEMS" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


