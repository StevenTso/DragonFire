################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/delay.c \
../src/lib/gpio.c \
../src/lib/i2c.c \
../src/lib/ssp.c \
../src/lib/timer16.c 

OBJS += \
./src/lib/delay.o \
./src/lib/gpio.o \
./src/lib/i2c.o \
./src/lib/ssp.o \
./src/lib/timer16.o 

C_DEPS += \
./src/lib/delay.d \
./src/lib/gpio.d \
./src/lib/i2c.d \
./src/lib/ssp.d \
./src/lib/timer16.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC11Uxx -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/CMSISv2p00_LPC11Uxx/inc" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFire(SemiHosting)/src/lib" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFire(SemiHosting)/src/SDCard/src" -I"/Users/steven/Desktop/DragonFire/Projects/Embedded/DragonFire(SemiHosting)/src/MEMS" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


