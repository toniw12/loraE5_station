################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/antoine/Arduino/libraries/SdFat/src/SdCard/SdCardInfo.cpp \
/home/antoine/Arduino/libraries/SdFat/src/SdCard/SdSpiCard.cpp \
/home/antoine/Arduino/libraries/SdFat/src/SdCard/SdioTeensy.cpp 

LINK_OBJ += \
./libraries/SdFat/src/SdCard/SdCardInfo.cpp.o \
./libraries/SdFat/src/SdCard/SdSpiCard.cpp.o \
./libraries/SdFat/src/SdCard/SdioTeensy.cpp.o 

CPP_DEPS += \
./libraries/SdFat/src/SdCard/SdCardInfo.cpp.d \
./libraries/SdFat/src/SdCard/SdSpiCard.cpp.d \
./libraries/SdFat/src/SdCard/SdioTeensy.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/SdFat/src/SdCard/SdCardInfo.cpp.o: /home/antoine/Arduino/libraries/SdFat/src/SdCard/SdCardInfo.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/xpack-arm-none-eabi-gcc/10.3.1-2.3/bin/arm-none-eabi-g++" -mcpu=cortex-m4   -DUSE_FULL_LL_DRIVER -mthumb "@/home/antoine/eclipse-workspace/loraE5_station/Release/sketch/build.opt" -c -Og -g -DNDEBUG -w -std=gnu++14 -ffunction-sections -fdata-sections -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -fno-use-cxa-atexit -MMD "-I/home/antoine/eclipse-workspace/loraE5_station" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/avr" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/LL" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/OpenAMP" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb/hid" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb/cdc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/STM32WLxx_HAL_Driver/Inc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/STM32WLxx_HAL_Driver/Src" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/STM32WLxx" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/ST/STM32_USB_Device_Library/Core/Src" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/open-amp/lib/include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/libmetal/lib/include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/virtual_driver" -DSTM32WLxx -DARDUINO=10812 -DARDUINO_SEEED_LORA_E5_MINI -DARDUINO_ARCH_STM32 "-DBOARD_NAME=\"SEEED_LORA_E5_MINI\"" "-DVARIANT_H=\"variant_SEEED_LORA_E5_MINI.h\""  -DSTM32WLE5xx -DUSE_CM4_STARTUP_FILE -DHAL_UART_MODULE_ENABLED  "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/Core/Include/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/CMSIS/Device/ST/STM32WLxx/Include/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/CMSIS/Device/ST/STM32WLxx/Source/Templates/gcc/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/DSP/Include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/DSP/PrivateInclude"   -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/variants/STM32WLxx/WL54JCI_WL55JCI_WLE4J(8-B-C)I_WLE5J(8-B-C)I" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/SrcWrapper/src" -I"/home/antoine/Arduino/libraries/ArduinoJson/src" -I"/home/antoine/Arduino/libraries/SdFat/src" -I"/home/antoine/Arduino/libraries/ArduinoModbus/src" -I"/home/antoine/Arduino/libraries/ArduinoRS485/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/SPI/src" -I"/home/antoine/Arduino/libraries/Time" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/Wire/src" -I"/home/antoine/Arduino/libraries/BMx280MI/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/libraries/DS3232RTC/2.0.1/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/libraries/CircularBuffer/1.3.3" -I"/home/antoine/Arduino/libraries/Adafruit_SSD1306" -I"/home/antoine/Arduino/libraries/Adafruit_GFX_Library" -I"/home/antoine/Arduino/libraries/Adafruit_BusIO" -I"/home/antoine/Arduino/libraries/base64/src" -I"/home/antoine/Arduino/libraries/FileLogger/src" -I"/home/antoine/Arduino/libraries/SecuredLinkedList/src" -I"/home/antoine/Arduino/libraries/LinkedList" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries/SdFat/src/SdCard/SdSpiCard.cpp.o: /home/antoine/Arduino/libraries/SdFat/src/SdCard/SdSpiCard.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/xpack-arm-none-eabi-gcc/10.3.1-2.3/bin/arm-none-eabi-g++" -mcpu=cortex-m4   -DUSE_FULL_LL_DRIVER -mthumb "@/home/antoine/eclipse-workspace/loraE5_station/Release/sketch/build.opt" -c -Og -g -DNDEBUG -w -std=gnu++14 -ffunction-sections -fdata-sections -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -fno-use-cxa-atexit -MMD "-I/home/antoine/eclipse-workspace/loraE5_station" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/avr" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/LL" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/OpenAMP" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb/hid" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb/cdc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/STM32WLxx_HAL_Driver/Inc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/STM32WLxx_HAL_Driver/Src" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/STM32WLxx" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/ST/STM32_USB_Device_Library/Core/Src" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/open-amp/lib/include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/libmetal/lib/include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/virtual_driver" -DSTM32WLxx -DARDUINO=10812 -DARDUINO_SEEED_LORA_E5_MINI -DARDUINO_ARCH_STM32 "-DBOARD_NAME=\"SEEED_LORA_E5_MINI\"" "-DVARIANT_H=\"variant_SEEED_LORA_E5_MINI.h\""  -DSTM32WLE5xx -DUSE_CM4_STARTUP_FILE -DHAL_UART_MODULE_ENABLED  "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/Core/Include/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/CMSIS/Device/ST/STM32WLxx/Include/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/CMSIS/Device/ST/STM32WLxx/Source/Templates/gcc/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/DSP/Include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/DSP/PrivateInclude"   -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/variants/STM32WLxx/WL54JCI_WL55JCI_WLE4J(8-B-C)I_WLE5J(8-B-C)I" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/SrcWrapper/src" -I"/home/antoine/Arduino/libraries/ArduinoJson/src" -I"/home/antoine/Arduino/libraries/SdFat/src" -I"/home/antoine/Arduino/libraries/ArduinoModbus/src" -I"/home/antoine/Arduino/libraries/ArduinoRS485/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/SPI/src" -I"/home/antoine/Arduino/libraries/Time" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/Wire/src" -I"/home/antoine/Arduino/libraries/BMx280MI/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/libraries/DS3232RTC/2.0.1/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/libraries/CircularBuffer/1.3.3" -I"/home/antoine/Arduino/libraries/Adafruit_SSD1306" -I"/home/antoine/Arduino/libraries/Adafruit_GFX_Library" -I"/home/antoine/Arduino/libraries/Adafruit_BusIO" -I"/home/antoine/Arduino/libraries/base64/src" -I"/home/antoine/Arduino/libraries/FileLogger/src" -I"/home/antoine/Arduino/libraries/SecuredLinkedList/src" -I"/home/antoine/Arduino/libraries/LinkedList" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries/SdFat/src/SdCard/SdioTeensy.cpp.o: /home/antoine/Arduino/libraries/SdFat/src/SdCard/SdioTeensy.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/xpack-arm-none-eabi-gcc/10.3.1-2.3/bin/arm-none-eabi-g++" -mcpu=cortex-m4   -DUSE_FULL_LL_DRIVER -mthumb "@/home/antoine/eclipse-workspace/loraE5_station/Release/sketch/build.opt" -c -Og -g -DNDEBUG -w -std=gnu++14 -ffunction-sections -fdata-sections -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -fno-use-cxa-atexit -MMD "-I/home/antoine/eclipse-workspace/loraE5_station" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/avr" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/LL" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/OpenAMP" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb/hid" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino/stm32/usb/cdc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/STM32WLxx_HAL_Driver/Inc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/STM32WLxx_HAL_Driver/Src" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/STM32WLxx" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/ST/STM32_USB_Device_Library/Core/Src" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/open-amp/lib/include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/libmetal/lib/include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Middlewares/OpenAMP/virtual_driver" -DSTM32WLxx -DARDUINO=10812 -DARDUINO_SEEED_LORA_E5_MINI -DARDUINO_ARCH_STM32 "-DBOARD_NAME=\"SEEED_LORA_E5_MINI\"" "-DVARIANT_H=\"variant_SEEED_LORA_E5_MINI.h\""  -DSTM32WLE5xx -DUSE_CM4_STARTUP_FILE -DHAL_UART_MODULE_ENABLED  "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/Core/Include/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/CMSIS/Device/ST/STM32WLxx/Include/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/Drivers/CMSIS/Device/ST/STM32WLxx/Source/Templates/gcc/" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/DSP/Include" "-I/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/DSP/PrivateInclude"   -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/variants/STM32WLxx/WL54JCI_WL55JCI_WLE4J(8-B-C)I_WLE5J(8-B-C)I" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/cores/arduino" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/SrcWrapper/src" -I"/home/antoine/Arduino/libraries/ArduinoJson/src" -I"/home/antoine/Arduino/libraries/SdFat/src" -I"/home/antoine/Arduino/libraries/ArduinoModbus/src" -I"/home/antoine/Arduino/libraries/ArduinoRS485/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/SPI/src" -I"/home/antoine/Arduino/libraries/Time" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/libraries/Wire/src" -I"/home/antoine/Arduino/libraries/BMx280MI/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/libraries/DS3232RTC/2.0.1/src" -I"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/libraries/CircularBuffer/1.3.3" -I"/home/antoine/Arduino/libraries/Adafruit_SSD1306" -I"/home/antoine/Arduino/libraries/Adafruit_GFX_Library" -I"/home/antoine/Arduino/libraries/Adafruit_BusIO" -I"/home/antoine/Arduino/libraries/base64/src" -I"/home/antoine/Arduino/libraries/FileLogger/src" -I"/home/antoine/Arduino/libraries/SecuredLinkedList/src" -I"/home/antoine/Arduino/libraries/LinkedList" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


