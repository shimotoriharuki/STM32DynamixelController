/*
 * HAL_RS485.hpp
 *
 *  Created on: Mar 28, 2022
 *      Author: Haruki Shimotori
 */

#ifndef INC_HAL_RS485_HPP_
#define INC_HAL_RS485_HPP_

#include "stm32f4xx_hal.h"
#include "BufData.hpp"

class HAL_RS485{
private:

public:
	HAL_RS485();
	HAL_RS485(UART_HandleTypeDef *, GPIO_TypeDef *, uint16_t gpio_num);
	void send(uint8_t *, uint8_t);
	void read(uint8_t *, uint8_t);

	/*
	void setSendBuf(uint8_t *, uint16_t);
	void setReadDataPointer(uint8_t *, uint16_t);
	void setNoReceiveFlag(bool);
	*/
	void setBufs(uint8_t *, uint16_t, uint8_t *, uint16_t, bool);
	void startSendIT();
	bool isCommunicating();
};


#endif /* INC_HAL_RS485_HPP_ */
