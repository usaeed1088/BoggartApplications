################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/developer/Clones/boggart/BoggartApplications/Applications/Demonstration_Node/Options/Options.cpp 

OBJS += \
./Demonstration_Node/Options/Options.o 

CPP_DEPS += \
./Demonstration_Node/Options/Options.d 


# Each subdirectory must supply rules for building sources it contributes
Demonstration_Node/Options/Options.o: /home/developer/Clones/boggart/BoggartApplications/Applications/Demonstration_Node/Options/Options.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../../Boggart -I../../../../../../Boggart/Lib -I../../../../../../PAL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


