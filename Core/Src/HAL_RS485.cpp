/*
 * HAL_RS485.cpp
 *
 *  Created on: Mar 28, 2022
 *      Author: Haruki Shimotori
 */

#include "HAL_RS485.hpp"
#include <string.h>
#include <vector>
#include <algorithm>

#define BUF_SIZE 256

uint16_t mon_tx_cnt = 0;
uint16_t mon_rx_cnt = 0;

BufData g_buf_data(BUF_SIZE);
bool g_communicating_flag = false;

UART_HandleTypeDef *g_huart_num_;
GPIO_TypeDef *g_gpio_type;
uint16_t g_gpio_num;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(g_gpio_type, g_gpio_num, GPIO_PIN_RESET); //RS485 Send mode disable
	mon_tx_cnt++;

	uint8_t *p_temp_data;
	uint16_t temp_data_length;
	p_temp_data = g_buf_data.getFirstReadPointerBuf(&temp_data_length);

	bool no_receive_flag = g_buf_data.getFirstNoReceiveFlagBuf();

	if(no_receive_flag == false){ //If there is data received from the device. Start receiveing.
		HAL_UART_Receive_IT(g_huart_num_, (uint8_t*) p_temp_data, temp_data_length);
	}
	else{ //If there is no data received from the device. Send next data.
		HAL_GPIO_WritePin(g_gpio_type, g_gpio_num, GPIO_PIN_SET); //RS485 Send mode enable

		g_buf_data.deleteFirstBuf();
		uint16_t length = g_buf_data.getFirstSendBufLength();
		if(length != 0){ //If there is a message in the buffer
			uint8_t temp_data[length];
			g_buf_data.getFirstSendBuf(temp_data, length);

			HAL_UART_Transmit_IT(g_huart_num_, (uint8_t*) temp_data, length);
		}
		else{
			g_communicating_flag = false;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(g_gpio_type, g_gpio_num, GPIO_PIN_SET); //RS485 Send mode enable
	mon_rx_cnt++;

	// Preapare next data.
	g_buf_data.deleteFirstBuf();

	uint16_t length = g_buf_data.getFirstSendBufLength();
	if(length != 0){ //If there is a message in the buffer
		uint8_t temp_data[length];
		g_buf_data.getFirstSendBuf(temp_data, length);

		HAL_UART_Transmit_IT(g_huart_num_, (uint8_t*) temp_data, length);
	}
	else{
		g_communicating_flag = false;
	}

}

/* ------- private ------ */

/* ------- public ------- */
HAL_RS485::HAL_RS485(){}
HAL_RS485::HAL_RS485(UART_HandleTypeDef *huart_num, GPIO_TypeDef *gpio_type, uint16_t gpio_num)
{
	g_huart_num_ = huart_num;
	g_gpio_type = gpio_type;
	g_gpio_num = gpio_num;
}

void HAL_RS485::send(uint8_t *s, uint8_t size)
{
	HAL_GPIO_WritePin(g_gpio_type, g_gpio_num, GPIO_PIN_SET);

	HAL_UART_Transmit(g_huart_num_, (uint8_t*) s, size, 0xFFFF);
	HAL_Delay(0.2);

	HAL_GPIO_WritePin(g_gpio_type, g_gpio_num, GPIO_PIN_RESET);
}

void HAL_RS485::read(uint8_t *s, uint8_t size)
{
	HAL_UART_Receive(g_huart_num_, (uint8_t*) s, size, 0xFFFF);
	HAL_Delay(0.2);
}

/*
void HAL_RS485::setSendBuf(uint8_t *data, uint16_t send_size)
{
	g_buf_data.setSendBuf(data, send_size);
}

void HAL_RS485::setReadDataPointer(uint8_t *read_data_pointer, uint16_t length)
{
	g_buf_data.setReadPointerBuf(read_data_pointer, length);
}

void HAL_RS485::setNoReceiveFlag(bool flag)
{
	g_buf_data.setNoReceiveFlagBuf(flag);
}
*/

void HAL_RS485::setBufs(uint8_t *send_data, uint16_t send_data_length, uint8_t *read_data_pointer, uint16_t read_data_pointer_length, bool flag)
{
	g_buf_data.setSendBuf(send_data, send_data_length);
	g_buf_data.setReadPointerBuf(read_data_pointer, read_data_pointer_length);
	g_buf_data.setNoReceiveFlagBuf(flag);

}

void HAL_RS485::startSendIT()
{
	HAL_GPIO_WritePin(g_gpio_type, g_gpio_num, GPIO_PIN_SET); //RS485 Send mode enable

	uint16_t length = g_buf_data.getFirstSendBufLength();
	uint8_t temp_data[length];
	g_buf_data.getFirstSendBuf(temp_data, length);

	g_communicating_flag = true;
	HAL_UART_Transmit_IT(g_huart_num_, (uint8_t*) temp_data, length);
	HAL_Delay(1);
}

bool HAL_RS485::isCommunicating()
{
	return g_communicating_flag;
}

