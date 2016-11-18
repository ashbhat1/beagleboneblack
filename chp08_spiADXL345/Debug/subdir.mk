################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ADXL345.c \
../SPIDevice.c \
../simple_spiADXL345_TEST.c 

OBJS += \
./ADXL345.o \
./SPIDevice.o \
./simple_spiADXL345_TEST.o 

C_DEPS += \
./ADXL345.d \
./SPIDevice.d \
./simple_spiADXL345_TEST.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


