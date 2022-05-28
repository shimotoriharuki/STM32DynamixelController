/*
 * wrapper.cpp
 *
 *  Created on: Oct 16, 2021
 *      Author: Haruki Shimotori
 */
#include "wrapper.hpp"
#include "Dynamixel.hpp"

#define DYNAMIXEL_ID 3

UART_HandleTypeDef huart5;

Dynamixel mx64(&huart5, GPIOC, GPIO_PIN_13);

uint8_t mon_error_led, mon_error_temp;
uint16_t mon_ret;
uint8_t mon_ping;
uint8_t mon_char;

void cppInit(void){
	//HAL_Delay(1000);
	mon_ping = mx64.ping(DYNAMIXEL_ID);

	mx64.setOperatingMode(DYNAMIXEL_ID, 4);
	mx64.requestInputVoltage(DYNAMIXEL_ID);

	mx64.torque(DYNAMIXEL_ID, 1);
	mx64.setGoalPosition(DYNAMIXEL_ID, 0);
}

void cppLoop(void){
	static float angle;

	mx64.setGoalPosition(DYNAMIXEL_ID, angle);

}

void cppFlip1ms(void){

	//encoder.updateCnt();
	//total_cnt = encoder.getAngle();

	//encoder.clearCnt();

}



