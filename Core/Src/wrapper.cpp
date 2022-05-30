/*
 * wrapper.cpp
 *
 *  Created on: Oct 16, 2021
 *      Author: Haruki Shimotori
 */
#include "wrapper.hpp"
#include "Dynamixel.hpp"
#include <stdio.h>

#define DYNAMIXEL_ID 1

UART_HandleTypeDef huart5;

Dynamixel mx64(&huart5, GPIOC, GPIO_PIN_13);

void cppInit(void){
	uint8_t check_ping = mx64.ping(DYNAMIXEL_ID); //Confirm communication with Dynamixel
	printf("Ping: %d\r\n", check_ping); //0 is OK

	mx64.setOperatingMode(DYNAMIXEL_ID, 3); //Set operating mode
	mx64.torque(DYNAMIXEL_ID, 1); //Enalbe torque

	mx64.requestTemperature(DYNAMIXEL_ID); //Request to get temperature information
	mx64.requestInputVoltage(DYNAMIXEL_ID); //Request to get input voltage information
}

void cppLoop(void){
	static float angle;

	angle += 1;
	if(angle >= 90) angle = 0;

	mx64.setGoalPosition(DYNAMIXEL_ID, angle); //Set Angle for Dynamixel

	uint8_t temperature = mx64.getTemperature(); //Get temperature for Dynamixel
	printf("Temperature: %d\r\n", temperature);

	float voltage = mx64.getInputVoltage(); //Get input voltage for Dynamixel
	printf("Voltage: %f\r\n", voltage);

	HAL_Delay(100);
}
