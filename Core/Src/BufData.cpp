/*
 * BufData.cpp
 *
 *  Created on: Apr 2, 2022
 *      Author: Haruki Shimotori
 */

#include "BufData.hpp"

uint8_t mon_send_buf[256];
uint16_t mon_send_buf_idx;
uint8_t mon_read_buf[256];
uint16_t mon_read_buf_idx;

bool mon_bool[256];

uint8_t *mon_read_pointer[256];

/* ----private----*/


/* ----public-----*/
BufData::BufData(uint16_t buf_length) : buf_length_(buf_length), send_idx_(0), read_pointer_idx_(0), no_receive_flag_idx_(0)
{
	send_buf_.resize(buf_length_);
	read_pointer_buf_.resize(buf_length_);
	read_pointer_length_buf_.resize(buf_length_);
	no_receive_flag_buf_.resize(buf_length_);

}

void BufData::setSendBuf(uint8_t *data, uint16_t send_length)
{
	send_buf_[send_idx_].resize(send_length);
	for(uint16_t i = 0; i < send_length; i++){
		send_buf_[send_idx_][i] = data[i];
		mon_send_buf[mon_send_buf_idx] = data[i];
		mon_send_buf_idx++;
		if(mon_send_buf_idx >= 256) mon_send_buf_idx = 0;
	}

	send_idx_++;
	if(send_idx_ >= buf_length_) send_idx_ = 0;

}

uint16_t BufData::getFirstSendBufLength()
{
	return send_buf_[0].size();
}

void BufData::getFirstSendBuf(uint8_t *data, uint16_t length)
{
	for(uint16_t i = 0; i < length; i++){
		data[i] = send_buf_[0][i];
	}

}

void BufData::deleteFirstBuf()
{
	std::rotate(send_buf_.begin(), send_buf_.begin() + 1, send_buf_.end());
	std::rotate(read_pointer_buf_.begin(), read_pointer_buf_.begin() + 1, read_pointer_buf_.end());
	std::rotate(read_pointer_length_buf_.begin(), read_pointer_length_buf_.begin() + 1, read_pointer_length_buf_.end());
	std::rotate(no_receive_flag_buf_.begin(), no_receive_flag_buf_.begin() + 1, no_receive_flag_buf_.end());
	send_idx_--;
	read_pointer_idx_--;
	no_receive_flag_idx_--;

	send_buf_[buf_length_ - 1].resize(0);
	read_pointer_buf_[buf_length_ - 1] = 0;
	read_pointer_length_buf_[buf_length_ - 1] = 0;
	no_receive_flag_buf_[buf_length_ - 1] = 0;
}

void BufData::setReadPointerBuf(uint8_t *pointer, uint16_t length)
{
	read_pointer_buf_[read_pointer_idx_] = pointer;
	read_pointer_length_buf_[read_pointer_idx_] = length;

	read_pointer_idx_++;
	if(read_pointer_idx_ >= buf_length_) read_pointer_idx_ = 0;
}

uint8_t *BufData::getFirstReadPointerBuf(uint16_t *length)
{
	*length = read_pointer_length_buf_[0];

	return read_pointer_buf_[0];
}

void BufData::setNoReceiveFlagBuf(bool flag)
{
	no_receive_flag_buf_[no_receive_flag_idx_] = flag;

	no_receive_flag_idx_++;
	if(no_receive_flag_idx_>= buf_length_) no_receive_flag_idx_ = 0;

	for(uint16_t i = 0; i < 256; i++){
		mon_bool[i] = no_receive_flag_buf_[i];
	}
}

bool BufData::getFirstNoReceiveFlagBuf()
{
	return no_receive_flag_buf_[0];
}
