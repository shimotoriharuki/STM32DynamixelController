/*
 * bufData.hpp
 *
 *  Created on: Apr 2, 2022
 *      Author: Haruki Shimotori
 */

#ifndef INC_BUFDATA_HPP_
#define INC_BUFDATA_HPP_

#include "stm32f4xx_hal.h"
#include <vector>
#include <algorithm>

class BufData{
private:
	const uint16_t buf_length_;

	std::vector<std::vector<uint8_t>> send_buf_;
	uint16_t send_idx_;

	std::vector<uint8_t *> read_pointer_buf_;
	std::vector<uint16_t> read_pointer_length_buf_;
	uint16_t read_pointer_idx_;

	std::vector<bool> no_receive_flag_buf_;
	uint16_t no_receive_flag_idx_;

public:
	BufData(uint16_t);
	void setSendBuf(uint8_t *, uint16_t);
	uint16_t getFirstSendBufLength();
	void getFirstSendBuf(uint8_t *, uint16_t);
	void deleteFirstBuf();

	void setReadPointerBuf(uint8_t *, uint16_t);
	uint8_t *getFirstReadPointerBuf(uint16_t *);

	void setNoReceiveFlagBuf(bool);
	bool getFirstNoReceiveFlagBuf();
};

#endif /* INC_BUFDATA_HPP_ */
