/*
 * Dynamixel.hpp
 *
 *  Created on: Mar 29, 2022
 *      Author: Haruki Shimotori
 */

#ifndef INC_DYNAMIXEL_HPP_
#define INC_DYNAMIXEL_HPP_

#include "stm32f4xx_hal.h"
#include "HAL_RS485.hpp"

#define INSTRUCTION_PING 0x01
#define INSTRUCTION_READ 0x02
#define INSTRUCTION_WRITE 0x03

#define ADDRESS_OPERATING_MODE 0x0B
#define ADDRESS_TORQUE_ENABLE 0x40
#define ADDRESS_LED 0x41
#define ADDRESS_GOAL_POSITION 0x74
#define ADDRESS_PURESENT_INPUT_VOLTAGE 0x90
#define ADDRESS_PRESENT_TEMPERATURE 0x92

#define STATUS_PACKET_PING 6
#define STATUS_PACKET_OPERATING_MODE 6
#define STATUS_PACKET_TORQUE_ENABLE_LEN 6
#define STATUS_PACKET_LED_LEN 6
#define STATUS_PACKET_GOAL_POSITION_LEN 6
#define STATUS_PACKET_INPUT_VOLTAGE_LEN 8
#define STATUS_PACKET_PRESENT_TEMPERATURE_LEN 7

class Dynamixel{
private:
	HAL_RS485 rs485_;

	uint8_t status_packet_ping_[STATUS_PACKET_PING];
	uint8_t status_packet_operating_mode_[STATUS_PACKET_OPERATING_MODE];
	uint8_t status_packet_torque_enable_[STATUS_PACKET_TORQUE_ENABLE_LEN];
	uint8_t status_packet_goal_position_[STATUS_PACKET_GOAL_POSITION_LEN];
	uint8_t status_packet_led_[STATUS_PACKET_LED_LEN];
	uint8_t status_packet_tempareture_[STATUS_PACKET_PRESENT_TEMPERATURE_LEN];
	uint8_t status_packet_inputvoltage_[STATUS_PACKET_INPUT_VOLTAGE_LEN];

	uint8_t writeData_(uint8_t, uint8_t, uint8_t *);
	uint8_t readData_(uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t *);
	void setPacket(uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t, uint8_t *);
	void communicationStart();
	uint16_t getCheckSum_(uint8_t *);

public:
	Dynamixel(UART_HandleTypeDef *, GPIO_TypeDef *, uint16_t);

	/* Ping */
	uint8_t ping(uint8_t);

	/* Write */
	void LED(uint8_t, uint8_t);
	void getLEDError(uint8_t *error);

	void torque(uint8_t, uint8_t);
	void getTorqueError(uint8_t *error);

	void setGoalPosition(uint8_t, float);
	void getGoalPositionError(uint8_t *error);

	void setOperatingMode(uint8_t, uint8_t);
	void getOperatingModeError(uint8_t *error);

	/* Read */
	void requestTemperature(uint8_t);
	uint8_t getTemperature(uint8_t *error = 0);

	void requestInputVoltage(uint8_t);
	float getInputVoltage(uint8_t *error = 0);


};

#endif /* INC_DYNAMIXEL_HPP_ */
