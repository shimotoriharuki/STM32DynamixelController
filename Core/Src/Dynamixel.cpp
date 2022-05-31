/*
 * Dynamixe.cpp
 *
 *  Created on: Mar 29, 2022
 *      Author: Haruki Shimotori
 */

#include <Dynamixel.hpp>

uint8_t mon_res_led[10];
uint8_t mon_res_temperature[10];
uint8_t mon_res_voltage[10];
uint16_t mon_res_idx;
uint8_t *mon_pointer[256];

/* -----private-----*/
uint8_t Dynamixel::writeData_(uint8_t id, uint8_t write_param_len, uint8_t *write_param) //return error
{
	/* ---Send Instruction packet--- */
	uint8_t send_buf_length = write_param_len + 6;
	uint8_t send_buf[send_buf_length];

	send_buf[0] = 0xff;
	send_buf[1] = 0xff;
	send_buf[2] = id;
	send_buf[3] = write_param_len + 2; //param_len + (Instruction + Checksum)
	send_buf[4] = INSTRUCTION_WRITE;

	for(uint8_t i = 0; i < write_param_len; i++){ //Set parameters;
		send_buf[5 + i] = write_param[i];
	}

	send_buf[5 + write_param_len] = getCheckSum_(send_buf);

	rs485_.send(send_buf, send_buf_length);


	/* ---Read status packet--- */
	uint8_t read_buf[6];
	rs485_.read(read_buf, 6);

	return read_buf[4]; //return Error status
}

uint8_t Dynamixel::readData_(uint8_t id, uint8_t write_param_len, uint8_t read_param_len, uint8_t *write_param, uint8_t *read_param)
{
	/* ---Send Instruction packet--- */
	uint8_t send_buf_length = write_param_len + 6;
	uint8_t send_buf[send_buf_length];

	send_buf[0] = 0xff;
	send_buf[1] = 0xff;
	send_buf[2] = id;
	send_buf[3] = write_param_len + 2; //param_len + (Instruction + Checksum)
	send_buf[4] = INSTRUCTION_READ;

	for(uint8_t i = 0; i < write_param_len; i++){ //Set parameters;
		send_buf[5 + i] = write_param[i];
	}

	send_buf[5 + write_param_len] = getCheckSum_(send_buf);

	rs485_.send(send_buf, send_buf_length);


	/* ---Read status packet--- */
	uint8_t read_buf_length = read_param_len + 6;
	uint8_t read_buf[read_buf_length];
	rs485_.read(read_buf, read_buf_length);

	for(uint16_t i = 0; i < read_param_len; i++){
		read_param[i] = read_buf[5 + i];
	}

	return read_buf[4]; //return Error status
}


void Dynamixel::setPacket(uint8_t id, uint8_t instruction, uint8_t write_param_len, uint8_t *write_param, uint8_t read_param_len, uint8_t *read_param)
{
	/* ---Send Instruction packet--- */
	uint8_t send_buf_length = write_param_len + 6;
	uint8_t send_buf[send_buf_length];

	send_buf[0] = 0xff;
	send_buf[1] = 0xff;
	send_buf[2] = id;
	send_buf[3] = write_param_len + 2; //param_len + (Instruction + Checksum)
	send_buf[4] = instruction;

	for(uint8_t i = 0; i < write_param_len; i++){ //Set parameters;
		send_buf[5 + i] = write_param[i];
	}

	send_buf[5 + write_param_len] = getCheckSum_(send_buf);

	uint8_t read_buf_length = read_param_len + 6;

	if(id == 254) rs485_.setBufs(send_buf, send_buf_length, read_param, read_buf_length, true);
	else rs485_.setBufs(send_buf, send_buf_length, read_param, read_buf_length, false);
	/*
	rs485_.setSendBuf(send_buf, send_buf_length);
	rs485_.setReadDataPointer(read_param, read_buf_length);
	if(id == 254)	rs485_.setNoReceiveFlag(true);
	else rs485_.setNoReceiveFlag(false);
	*/
}

void Dynamixel::communicationStart()
{
	rs485_.startSendIT();
}

uint16_t Dynamixel::getCheckSum_(uint8_t *buf)
{
	uint8_t sum = 0;
	for(uint8_t i = 2; i < buf[3] + 3; i++){ //buf[3](param_size) + (id + length : instruction)
		sum += buf[i];
	}

	return 0xff - (sum & 0xff); //If the calculated value exceeds 255, the lower 8 bits are set as SUM.
}

/* -----public------*/
Dynamixel::Dynamixel(UART_HandleTypeDef *huart_num, GPIO_TypeDef *gpio_type, uint16_t gpio_num) : rs485_(huart_num, gpio_type, gpio_num){}

/* ping */
uint8_t Dynamixel::ping(uint8_t id)
{
	/* ---Send Instruction packet--- */
	uint8_t send_buf[6];
	send_buf[0] = 0xff;
	send_buf[1] = 0xff;
	send_buf[2] = id;
	send_buf[3] = 2; //(Instruction + Checksum)
	send_buf[4] = INSTRUCTION_PING;
	send_buf[5] = getCheckSum_(send_buf);

	rs485_.send(send_buf, 6);

	/* ---Read status packet--- */
	uint8_t read_buf[6];
	rs485_.read(read_buf, 6);

	return read_buf[4]; //return Error status
}

/* write */
void Dynamixel::LED(uint8_t id, uint8_t status)
{
	uint8_t write_param[2];
	write_param[0] = ADDRESS_LED;
	write_param[1] = status;

	setPacket(id, INSTRUCTION_WRITE, 2, write_param, 0, status_packet_led_);

	if(rs485_.isCommunicating() == false) communicationStart();
}

void Dynamixel::getLEDError(uint8_t *error)
{
	*error = status_packet_led_[4];
}

void Dynamixel::torque(uint8_t id, uint8_t status)
{
	uint8_t write_param[2];
	write_param[0] = ADDRESS_TORQUE_ENABLE;
	write_param[1] = status;

	setPacket(id, INSTRUCTION_WRITE, 2, write_param, 0, status_packet_torque_enable_);

	if(rs485_.isCommunicating() == false) communicationStart();
}

void Dynamixel::getTorqueError(uint8_t *error)
{
	*error = status_packet_torque_enable_[4];
}

void Dynamixel::setGoalPosition(uint8_t id, float position)
{
	uint8_t write_param[5];
	int32_t value = (int32_t)(position * 4095 / 359.9);
	write_param[0] = ADDRESS_GOAL_POSITION;
	write_param[1] = value & 0xff;
	write_param[2] = (value >> 8) & 0xff;
	write_param[3] = (value >> 16) & 0xff;
	write_param[4] = (value >> 24) & 0xff;

	setPacket(id, INSTRUCTION_WRITE, 5, write_param, 0, status_packet_goal_position_);

	if(rs485_.isCommunicating() == false) communicationStart();
}

void Dynamixel::getGoalPositionError(uint8_t *error)
{
	*error = status_packet_goal_position_[4];
}

void Dynamixel::setOperatingMode(uint8_t id, uint8_t mode)
{
	uint8_t write_param[2];
	write_param[0] = ADDRESS_OPERATING_MODE;
	write_param[1] = mode;

	setPacket(id, INSTRUCTION_WRITE, 2, write_param, 0, status_packet_operating_mode_);

	if(rs485_.isCommunicating() == false) communicationStart();
}

void Dynamixel::getOperatingModeError(uint8_t *error)
{
	*error = status_packet_operating_mode_[4];
}

/* read */
void Dynamixel::requestTemperature(uint8_t id)
{
	uint8_t write_param[2];
	uint8_t byte_size = 0x01;
	write_param[0] = ADDRESS_PRESENT_TEMPERATURE;
	write_param[1] = byte_size; //byte size

	setPacket(id, INSTRUCTION_READ, 2, write_param, byte_size, status_packet_tempareture_);

	if(rs485_.isCommunicating() == false) communicationStart();

}

uint8_t Dynamixel::getTemperature(uint8_t *error)
{
	for(uint16_t i = 0; i < STATUS_PACKET_PRESENT_TEMPERATURE_LEN; i++){
		mon_res_temperature[i] = status_packet_tempareture_[i];
	}

	*error= status_packet_tempareture_[4];

	return status_packet_tempareture_[5];
}

void Dynamixel::requestInputVoltage(uint8_t id)
{
	uint8_t write_param[2];
	uint8_t byte_size = 0x02;
	write_param[0] = ADDRESS_PURESENT_INPUT_VOLTAGE;
	write_param[1] = byte_size; //byte size

	setPacket(id, INSTRUCTION_READ, 2, write_param, byte_size, status_packet_inputvoltage_);

	if(rs485_.isCommunicating() == false) communicationStart();

}

float Dynamixel::getInputVoltage(uint8_t *error)
{
	*error= status_packet_inputvoltage_[4];

	return float(status_packet_inputvoltage_[5]) / 10;
}
