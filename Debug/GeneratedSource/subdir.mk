################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GeneratedSource/Thermistor.c \
../GeneratedSource/cycfg.c \
../GeneratedSource/cycfg_clocks.c \
../GeneratedSource/cycfg_peripherals.c \
../GeneratedSource/cycfg_pins.c \
../GeneratedSource/cycfg_platform.c \
../GeneratedSource/cycfg_routing.c 

OBJS += \
./GeneratedSource/Thermistor.o \
./GeneratedSource/cycfg.o \
./GeneratedSource/cycfg_clocks.o \
./GeneratedSource/cycfg_peripherals.o \
./GeneratedSource/cycfg_pins.o \
./GeneratedSource/cycfg_platform.o \
./GeneratedSource/cycfg_routing.o 

C_DEPS += \
./GeneratedSource/Thermistor.d \
./GeneratedSource/cycfg.d \
./GeneratedSource/cycfg_clocks.d \
./GeneratedSource/cycfg_peripherals.d \
./GeneratedSource/cycfg_pins.d \
./GeneratedSource/cycfg_platform.d \
./GeneratedSource/cycfg_routing.d 


# Each subdirectory must supply rules for building sources it contributes
GeneratedSource/%.o: ../GeneratedSource/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -Og -ffunction-sections -fdata-sections -Wall  -g3 -DCY8C624ABZI_D44 -DDEBUG -DAPP_NAME='"CypressTest_cm4"' -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/include" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/cmsis/include" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/drivers/include" -I"E:\Programming\Cypress\PSoC6\CypressTest_mainapp/GeneratedSource" -I"E:\Programming\Cypress\PSoC6\CypressTest_mainapp/Source" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/drivers/source" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/cm4_low" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/cm4_high" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/linker/cm4" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/utilities/retarget_io/user" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/utilities/retarget_io" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/cm0p/prebuilt" -std=gnu11 -mfpu=fpv4-sp-d16 -ffat-lto-objects -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


