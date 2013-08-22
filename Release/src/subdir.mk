################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc8xx.c \
../src/gpio.c \
../src/main.c \
../src/max7221_graph.c \
../src/max7221_lib.c \
../src/power_mode.c \
../src/printf-retarget.c \
../src/printf.c \
../src/spi.c \
../src/system_LPC8xx.c \
../src/systick.c \
../src/uart.c 

OBJS += \
./src/cr_startup_lpc8xx.o \
./src/gpio.o \
./src/main.o \
./src/max7221_graph.o \
./src/max7221_lib.o \
./src/power_mode.o \
./src/printf-retarget.o \
./src/printf.o \
./src/spi.o \
./src/system_LPC8xx.o \
./src/systick.o \
./src/uart.o 

C_DEPS += \
./src/cr_startup_lpc8xx.d \
./src/gpio.d \
./src/main.d \
./src/max7221_graph.d \
./src/max7221_lib.d \
./src/power_mode.d \
./src/printf-retarget.d \
./src/printf.d \
./src/spi.d \
./src/system_LPC8xx.d \
./src/systick.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -D__LPC8XX__ -I"C:\Users\c.dicaprio\Contenitore\Vari per Mark III\LPCXpresso_5.2.4_2122\workspace\LPC810_MAX7221\cmsis" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


