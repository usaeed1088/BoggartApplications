################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/developer/Clones/boggart/BoggartApplications/Applications/Demonstration_Common/Messages/UserIO/UserIOMessage.cpp 

OBJS += \
./Demonstration_Node/Messages/UserIO/UserIOMessage.o 

CPP_DEPS += \
./Demonstration_Node/Messages/UserIO/UserIOMessage.d 


# Each subdirectory must supply rules for building sources it contributes
Demonstration_Node/Messages/UserIO/UserIOMessage.o: /home/developer/Clones/boggart/BoggartApplications/Applications/Demonstration_Common/Messages/UserIO/UserIOMessage.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../../Boggart -I../../../../../../Boggart/Lib -I../../../../../../PAL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


