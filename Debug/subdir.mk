################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../completer.cpp \
../endpoint.cpp \
../main.cpp \
../port.cpp \
../root_complex.cpp \
../root_complex_simple.cpp \
../switch.cpp 

OBJS += \
./completer.o \
./endpoint.o \
./main.o \
./port.o \
./root_complex.o \
./root_complex_simple.o \
./switch.o 

CPP_DEPS += \
./completer.d \
./endpoint.d \
./main.d \
./port.d \
./root_complex.d \
./root_complex_simple.d \
./switch.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/systemc230/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


