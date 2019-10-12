################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/gcc/startup_psoc6_02_cm4.S 

OBJS += \
./psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/gcc/startup_psoc6_02_cm4.o 

S_UPPER_DEPS += \
./psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/gcc/startup_psoc6_02_cm4.d 


# Each subdirectory must supply rules for building sources it contributes
psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/gcc/%.o: ../psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/gcc/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -Og -ffunction-sections -fdata-sections -Wall  -g3 -x assembler-with-cpp -DCY8C624ABZI_D44 -DDEBUG -DAPP_NAME='"CypressTest_cm4"' -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6mw/csdadc" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/include" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/cmsis/include" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/drivers/include" -I"E:\Programming\Cypress\PSoC6\CypressTest_mainapp/GeneratedSource" -I"E:\Programming\Cypress\PSoC6\CypressTest_mainapp/Source" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/drivers/source" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/cm4_low" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/startup/cm4_high" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/linker/cm4" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/utilities/retarget_io/user" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/utilities/retarget_io" -I"C:/Users/Oleh/ModusToolbox_1.1/libraries/psoc6sw-1.1/components/psoc6pdl/devices/psoc6/cm0p/prebuilt" -mfpu=fpv4-sp-d16 -ffat-lto-objects -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


