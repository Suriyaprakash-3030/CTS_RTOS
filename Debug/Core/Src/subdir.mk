################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AnalogRead.c \
../Core/Src/CustomerApp.c \
../Core/Src/DemoApp.c \
../Core/Src/DigitalRead.c \
../Core/Src/DigitalWrite.c \
../Core/Src/ESP_UART.c \
../Core/Src/Global.c \
../Core/Src/HardwareLevel.c \
../Core/Src/ISR.c \
../Core/Src/Initialize.c \
../Core/Src/Keypad.c \
../Core/Src/LCD.c \
../Core/Src/LoadCells.c \
../Core/Src/Sd_Card.c \
../Core/Src/SerialHandler.c \
../Core/Src/Servo.c \
../Core/Src/Test.c \
../Core/Src/adc.c \
../Core/Src/dma.c \
../Core/Src/fatfs_sd.c \
../Core/Src/freertos.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/pwm.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/usb_otg.c 

OBJS += \
./Core/Src/AnalogRead.o \
./Core/Src/CustomerApp.o \
./Core/Src/DemoApp.o \
./Core/Src/DigitalRead.o \
./Core/Src/DigitalWrite.o \
./Core/Src/ESP_UART.o \
./Core/Src/Global.o \
./Core/Src/HardwareLevel.o \
./Core/Src/ISR.o \
./Core/Src/Initialize.o \
./Core/Src/Keypad.o \
./Core/Src/LCD.o \
./Core/Src/LoadCells.o \
./Core/Src/Sd_Card.o \
./Core/Src/SerialHandler.o \
./Core/Src/Servo.o \
./Core/Src/Test.o \
./Core/Src/adc.o \
./Core/Src/dma.o \
./Core/Src/fatfs_sd.o \
./Core/Src/freertos.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/pwm.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/usb_otg.o 

C_DEPS += \
./Core/Src/AnalogRead.d \
./Core/Src/CustomerApp.d \
./Core/Src/DemoApp.d \
./Core/Src/DigitalRead.d \
./Core/Src/DigitalWrite.d \
./Core/Src/ESP_UART.d \
./Core/Src/Global.d \
./Core/Src/HardwareLevel.d \
./Core/Src/ISR.d \
./Core/Src/Initialize.d \
./Core/Src/Keypad.d \
./Core/Src/LCD.d \
./Core/Src/LoadCells.d \
./Core/Src/Sd_Card.d \
./Core/Src/SerialHandler.d \
./Core/Src/Servo.d \
./Core/Src/Test.d \
./Core/Src/adc.d \
./Core/Src/dma.d \
./Core/Src/fatfs_sd.d \
./Core/Src/freertos.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/pwm.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/usb_otg.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AnalogRead.cyclo ./Core/Src/AnalogRead.d ./Core/Src/AnalogRead.o ./Core/Src/AnalogRead.su ./Core/Src/CustomerApp.cyclo ./Core/Src/CustomerApp.d ./Core/Src/CustomerApp.o ./Core/Src/CustomerApp.su ./Core/Src/DemoApp.cyclo ./Core/Src/DemoApp.d ./Core/Src/DemoApp.o ./Core/Src/DemoApp.su ./Core/Src/DigitalRead.cyclo ./Core/Src/DigitalRead.d ./Core/Src/DigitalRead.o ./Core/Src/DigitalRead.su ./Core/Src/DigitalWrite.cyclo ./Core/Src/DigitalWrite.d ./Core/Src/DigitalWrite.o ./Core/Src/DigitalWrite.su ./Core/Src/ESP_UART.cyclo ./Core/Src/ESP_UART.d ./Core/Src/ESP_UART.o ./Core/Src/ESP_UART.su ./Core/Src/Global.cyclo ./Core/Src/Global.d ./Core/Src/Global.o ./Core/Src/Global.su ./Core/Src/HardwareLevel.cyclo ./Core/Src/HardwareLevel.d ./Core/Src/HardwareLevel.o ./Core/Src/HardwareLevel.su ./Core/Src/ISR.cyclo ./Core/Src/ISR.d ./Core/Src/ISR.o ./Core/Src/ISR.su ./Core/Src/Initialize.cyclo ./Core/Src/Initialize.d ./Core/Src/Initialize.o ./Core/Src/Initialize.su ./Core/Src/Keypad.cyclo ./Core/Src/Keypad.d ./Core/Src/Keypad.o ./Core/Src/Keypad.su ./Core/Src/LCD.cyclo ./Core/Src/LCD.d ./Core/Src/LCD.o ./Core/Src/LCD.su ./Core/Src/LoadCells.cyclo ./Core/Src/LoadCells.d ./Core/Src/LoadCells.o ./Core/Src/LoadCells.su ./Core/Src/Sd_Card.cyclo ./Core/Src/Sd_Card.d ./Core/Src/Sd_Card.o ./Core/Src/Sd_Card.su ./Core/Src/SerialHandler.cyclo ./Core/Src/SerialHandler.d ./Core/Src/SerialHandler.o ./Core/Src/SerialHandler.su ./Core/Src/Servo.cyclo ./Core/Src/Servo.d ./Core/Src/Servo.o ./Core/Src/Servo.su ./Core/Src/Test.cyclo ./Core/Src/Test.d ./Core/Src/Test.o ./Core/Src/Test.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/fatfs_sd.cyclo ./Core/Src/fatfs_sd.d ./Core/Src/fatfs_sd.o ./Core/Src/fatfs_sd.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/pwm.cyclo ./Core/Src/pwm.d ./Core/Src/pwm.o ./Core/Src/pwm.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/usb_otg.cyclo ./Core/Src/usb_otg.d ./Core/Src/usb_otg.o ./Core/Src/usb_otg.su

.PHONY: clean-Core-2f-Src

