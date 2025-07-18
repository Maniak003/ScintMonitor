################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AD5245.c \
../Core/Src/dhcp.c \
../Core/Src/main.c \
../Core/Src/network_int.c \
../Core/Src/socket.c \
../Core/Src/ssd1306.c \
../Core/Src/ssd1306_fonts.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/w5500.c \
../Core/Src/webpages.c \
../Core/Src/wizchip_conf.c 

C_DEPS += \
./Core/Src/AD5245.d \
./Core/Src/dhcp.d \
./Core/Src/main.d \
./Core/Src/network_int.d \
./Core/Src/socket.d \
./Core/Src/ssd1306.d \
./Core/Src/ssd1306_fonts.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/w5500.d \
./Core/Src/webpages.d \
./Core/Src/wizchip_conf.d 

OBJS += \
./Core/Src/AD5245.o \
./Core/Src/dhcp.o \
./Core/Src/main.o \
./Core/Src/network_int.o \
./Core/Src/socket.o \
./Core/Src/ssd1306.o \
./Core/Src/ssd1306_fonts.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/w5500.o \
./Core/Src/webpages.o \
./Core/Src/wizchip_conf.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G473xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AD5245.cyclo ./Core/Src/AD5245.d ./Core/Src/AD5245.o ./Core/Src/AD5245.su ./Core/Src/dhcp.cyclo ./Core/Src/dhcp.d ./Core/Src/dhcp.o ./Core/Src/dhcp.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/network_int.cyclo ./Core/Src/network_int.d ./Core/Src/network_int.o ./Core/Src/network_int.su ./Core/Src/socket.cyclo ./Core/Src/socket.d ./Core/Src/socket.o ./Core/Src/socket.su ./Core/Src/ssd1306.cyclo ./Core/Src/ssd1306.d ./Core/Src/ssd1306.o ./Core/Src/ssd1306.su ./Core/Src/ssd1306_fonts.cyclo ./Core/Src/ssd1306_fonts.d ./Core/Src/ssd1306_fonts.o ./Core/Src/ssd1306_fonts.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/w5500.cyclo ./Core/Src/w5500.d ./Core/Src/w5500.o ./Core/Src/w5500.su ./Core/Src/webpages.cyclo ./Core/Src/webpages.d ./Core/Src/webpages.o ./Core/Src/webpages.su ./Core/Src/wizchip_conf.cyclo ./Core/Src/wizchip_conf.d ./Core/Src/wizchip_conf.o ./Core/Src/wizchip_conf.su

.PHONY: clean-Core-2f-Src

