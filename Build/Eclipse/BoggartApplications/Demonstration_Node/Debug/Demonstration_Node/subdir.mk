################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/developer/Clones/boggart/BoggartApplications/Applications/Demonstration_Node/Demonstration_Node.cpp 

OBJS += \
./Demonstration_Node/Demonstration_Node.o 

CPP_DEPS += \
./Demonstration_Node/Demonstration_Node.d 


# Each subdirectory must supply rules for building sources it contributes
Demonstration_Node/Demonstration_Node.o: /home/developer/Clones/boggart/BoggartApplications/Applications/Demonstration_Node/Demonstration_Node.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../../Boggart -I../../../../../../Boggart/Lib -I../../../../../../PAL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


