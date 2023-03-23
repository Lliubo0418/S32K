################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/cJSON.c \
../Sources/cJSON_Utils.c \
../Sources/can.c \
../Sources/delay.c \
../Sources/font.c \
../Sources/key.c \
../Sources/main.c \
../Sources/oled.c \
../Sources/sys_cfg.c \
../Sources/uart.c 

OBJS += \
./Sources/cJSON.o \
./Sources/cJSON_Utils.o \
./Sources/can.o \
./Sources/delay.o \
./Sources/font.o \
./Sources/key.o \
./Sources/main.o \
./Sources/oled.o \
./Sources/sys_cfg.o \
./Sources/uart.o 

C_DEPS += \
./Sources/cJSON.d \
./Sources/cJSON_Utils.d \
./Sources/can.d \
./Sources/delay.d \
./Sources/font.d \
./Sources/key.d \
./Sources/main.d \
./Sources/oled.d \
./Sources/sys_cfg.d \
./Sources/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/cJSON.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


