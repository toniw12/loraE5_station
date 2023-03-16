################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include lora/subdir.mk
-include libraries/Wire/src/utility/subdir.mk
-include libraries/Wire/src/subdir.mk
-include libraries/Time/subdir.mk
-include libraries/SrcWrapper/src/stm32/subdir.mk
-include libraries/SrcWrapper/src/LL/subdir.mk
-include libraries/SrcWrapper/src/HAL/subdir.mk
-include libraries/SrcWrapper/src/subdir.mk
-include libraries/SecuredLinkedList/subdir.mk
-include libraries/SdFat/src/iostream/subdir.mk
-include libraries/SdFat/src/common/subdir.mk
-include libraries/SdFat/src/SpiDriver/subdir.mk
-include libraries/SdFat/src/SdCard/subdir.mk
-include libraries/SdFat/src/FsLib/subdir.mk
-include libraries/SdFat/src/FatLib/subdir.mk
-include libraries/SdFat/src/ExFatLib/subdir.mk
-include libraries/SdFat/src/subdir.mk
-include libraries/SPI/src/utility/subdir.mk
-include libraries/SPI/src/subdir.mk
-include libraries/FileLogger/src/subdir.mk
-include libraries/DS3232RTC/src/subdir.mk
-include libraries/BMx280MI/src/subdir.mk
-include libraries/ArduinoRS485/src/subdir.mk
-include libraries/ArduinoModbus/src/libmodbus/subdir.mk
-include libraries/ArduinoModbus/src/subdir.mk
-include libraries/Adafruit_SSD1306/subdir.mk
-include libraries/Adafruit_GFX_Library/subdir.mk
-include libraries/Adafruit_BusIO/subdir.mk
-include core/variant/subdir.mk
-include core/core/stm32/usb/hid/subdir.mk
-include core/core/stm32/usb/cdc/subdir.mk
-include core/core/stm32/usb/subdir.mk
-include core/core/stm32/OpenAMP/virtio/subdir.mk
-include core/core/stm32/OpenAMP/open-amp/rpmsg/subdir.mk
-include core/core/stm32/OpenAMP/open-amp/remoteproc/subdir.mk
-include core/core/stm32/OpenAMP/libmetal/generic/cortexm/subdir.mk
-include core/core/stm32/OpenAMP/libmetal/generic/subdir.mk
-include core/core/stm32/OpenAMP/libmetal/subdir.mk
-include core/core/stm32/OpenAMP/subdir.mk
-include core/core/stm32/subdir.mk
-include core/core/avr/subdir.mk
-include core/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
/home/antoine/eclipse-workspace/loraE5_station/Release/arduino.ar \

ELF += \
loraE5_station.elf \

HEX += \
loraE5_station.hex \



#bootloaderTest
BurnBootLoader: 
	@echo trying to burn bootloader ${bootloader.tool}
	${tools.${bootloader.tool}.erase.pattern}
	${tools.${bootloader.tool}.bootloader.pattern}

uploadWithBuild: all
	@echo trying to build and upload with upload tool ${upload.tool}
	${tools.${upload.tool}.upload.pattern}

uploadWithoutBuild: 
	@echo trying to upload without build with upload tool ${upload.tool}
	${tools.${upload.tool}.upload.pattern}
    
uploadWithProgrammerWithBuild: all
	@echo trying to build and upload with programmer ${program.tool}
	${tools.${program.tool}.program.pattern}

uploadWithProgrammerWithoutBuild: 
	@echo trying to upload with programmer ${program.tool} without build
	${tools.${program.tool}.program.pattern}

# All Target
all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: loraE5_station

# Tool invocations
loraE5_station: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/xpack-arm-none-eabi-gcc/10.3.1-2.3/bin/arm-none-eabi-size" -A "/home/antoine/eclipse-workspace/loraE5_station/Release/loraE5_station.elf"
	@echo 'Finished building target: $@'
	@echo ' '

/home/antoine/eclipse-workspace/loraE5_station/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/xpack-arm-none-eabi-gcc/10.3.1-2.3/bin/arm-none-eabi-gcc-ar" rcs     /home/antoine/eclipse-workspace/loraE5_station/Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

loraE5_station.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/xpack-arm-none-eabi-gcc/10.3.1-2.3/bin/arm-none-eabi-gcc" -mcpu=cortex-m4   -mthumb -Og -g -DNDEBUG --specs=nano.specs -Wl,--defsym=LD_FLASH_OFFSET=0 -Wl,--defsym=LD_MAX_SIZE=262144 -Wl,--defsym=LD_MAX_DATA_SIZE=65536 -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common "-Wl,--default-script=/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/variants/STM32WLxx/WL54JCI_WL55JCI_WLE4J(8-B-C)I_WLE5J(8-B-C)I/ldscript.ld" "-Wl,--script=/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/hardware/stm32/2.3.0/system/ldscript.ld" "-Wl,-Map,/home/antoine/eclipse-workspace/loraE5_station/Release/loraE5_station.map"   "-L/home/antoine/eclipse/cpp-2022-09/eclipse/arduinoPlugin/packages/STMicroelectronics/tools/CMSIS/5.7.0/CMSIS/DSP/Lib/GCC/" -larm_cortexM4l_math -o "/home/antoine/eclipse-workspace/loraE5_station/Release/loraE5_station.elf" "-L/home/antoine/eclipse-workspace/loraE5_station/Release" -Wl,--start-group   $(LINK_OBJ)    /home/antoine/eclipse-workspace/loraE5_station/Release/arduino.ar  -lc -Wl,--end-group -lm -lgcc -lstdc++
	@echo 'Finished building: $@'
	@echo ' '

loraE5_station.hex: $(ELF)
	@echo 'Do all objcopy commands'
	${recipe.objcopy.bin.pattern}
	${recipe.objcopy.hex.pattern}
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

pre-build:
	-${recipe.hooks.prebuild.1.pattern}
	-@echo ' '

.PHONY: all clean dependents main-build pre-build

-include ../makefile.targets
