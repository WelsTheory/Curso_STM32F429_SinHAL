/*
 * GPIO.h
 *
 *  Created on: Mar 14, 2023
 *      Author: Wels
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

typedef enum
{
	LOW,// = 0
	HIGH// = 1
}Gpio_State;

void GPIO_LEDS_Init(void);

void GPIO_LED1_Toggle(void);

void GPIO_LED1_Write(Gpio_State state);

/* PULSADOR PC13*/
void GPIO_BOT_Init(void);

Gpio_State GPIO_BOT_Read(void);

#endif /* INC_GPIO_H_ */
