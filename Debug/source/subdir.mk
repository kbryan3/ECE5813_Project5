################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/System.c \
../source/Testsuite.c \
../source/circularbuffer.c \
../source/led_control.c \
../source/logger.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/uart.c 

OBJS += \
./source/System.o \
./source/Testsuite.o \
./source/circularbuffer.o \
./source/led_control.o \
./source/logger.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/uart.o 

C_DEPS += \
./source/System.d \
./source/Testsuite.d \
./source/circularbuffer.d \
./source/led_control.d \
./source/logger.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


