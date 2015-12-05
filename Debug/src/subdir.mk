################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RDT.cpp \
../src/ReliableDataTransfer.cpp \
../src/StopAndWait.cpp 

OBJS += \
./src/RDT.o \
./src/ReliableDataTransfer.o \
./src/StopAndWait.o 

CPP_DEPS += \
./src/RDT.d \
./src/ReliableDataTransfer.d \
./src/StopAndWait.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


