################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/MPU6050.c \
../Src/core.c \
../Src/i2c.c \
../Src/main.c \
../Src/sys_init.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/time_base.c \
../Src/uart.c 

OBJS += \
./Src/MPU6050.o \
./Src/core.o \
./Src/i2c.o \
./Src/main.o \
./Src/sys_init.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/time_base.o \
./Src/uart.o 

C_DEPS += \
./Src/MPU6050.d \
./Src/core.d \
./Src/i2c.d \
./Src/main.d \
./Src/sys_init.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/time_base.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE=STM32F411xE -c -I"C:/Users/Public/Documents/STM32_CUBE/MPU6050/Inc" -I"C:/Users/Public/Documents/STM32_CUBE/MPU6050/F4_chip_headers/CMSIS/Include" -I"C:/Users/Public/Documents/STM32_CUBE/MPU6050/F4_chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/MPU6050.cyclo ./Src/MPU6050.d ./Src/MPU6050.o ./Src/MPU6050.su ./Src/core.cyclo ./Src/core.d ./Src/core.o ./Src/core.su ./Src/i2c.cyclo ./Src/i2c.d ./Src/i2c.o ./Src/i2c.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/sys_init.cyclo ./Src/sys_init.d ./Src/sys_init.o ./Src/sys_init.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/time_base.cyclo ./Src/time_base.d ./Src/time_base.o ./Src/time_base.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

