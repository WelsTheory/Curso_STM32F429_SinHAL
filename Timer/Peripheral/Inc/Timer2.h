/*
 * Timer2.h
 *
 *  Created on: Mar 29, 2023
 *      Author: Wels
 */

#ifndef INC_TIMER2_H_
#define INC_TIMER2_H_

#include "main.h"

void Timer2_Delay_Init(uint32_t arr_load);

void Timer2_Delay_ms(uint32_t delay);

/**/
void Timer2_Period_Init(uint32_t arr_load, uint32_t delay);

#endif /* INC_TIMER2_H_ */
