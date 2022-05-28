/*
 * wrapper.hpp
 *
 *  Created on: Oct 16, 2021
 *      Author: under
 */

#ifndef INC_WRAPPER_HPP_
#define INC_WRAPPER_HPP_

#ifdef __cplusplus
extern "C" {
#endif

void cppInit(void);
void cppLoop(void);
void cppFlip1ms(void);

#ifdef __cplusplus
};
#endif

#endif /* INC_WRAPPER_HPP_ */
