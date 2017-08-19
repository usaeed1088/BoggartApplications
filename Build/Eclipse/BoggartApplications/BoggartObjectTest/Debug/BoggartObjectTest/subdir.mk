################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/developer/Clones/Boggart/linux-makefiles/BoggartApplications/Applications/BoggartObjectTest/BoggartObjectTest.cpp 

OBJS += \
./BoggartObjectTest/BoggartObjectTest.o 

CPP_DEPS += \
./BoggartObjectTest/BoggartObjectTest.d 


# Each subdirectory must supply rules for building sources it contributes
BoggartObjectTest/BoggartObjectTest.o: /home/developer/Clones/Boggart/linux-makefiles/BoggartApplications/Applications/BoggartObjectTest/BoggartObjectTest.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I../../../../../PAL -I../../../../../Boggart -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


