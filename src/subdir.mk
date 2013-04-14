################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./src/faclinux.c \
./src/facwin32.c \
./src/hash.c \
./src/hashtable.c \
./src/list.c 

OBJS += \
./src/faclinux.o \
./src/facwin32.o \
./src/hash.o \
./src/hashtable.o \
./src/list.o 

C_DEPS += \
./src/faclinux.d \
./src/facwin32.d \
./src/hash.d \
./src/hashtable.d \
./src/list.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


