/*
 * SysTick.h
 *
 *  Created on: Mar 22, 2023
 *      Author: Wels
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include "main.h"

void SysTickConfig(uint32_t systick_valor);

void msIncTicks(void);

uint32_t msGetTicks(void);

void Delay_ms(uint32_t delay);

/* DELAY NO BLOQUEANTE */

typedef struct{
	uint32_t startTime;
	uint32_t duration;
	bool running;
}delay_t;

void Delay_ms_NB(delay_t * delay, uint32_t duration);

bool Delay_Read(delay_t * delay);


#endif /* INC_SYSTICK_H_ */
