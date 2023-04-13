/*
 * main.c
 *
 *  Created on: Mar 14, 2023
 *      Author: Wels
 */
#include "main.h"
#include "RCC.h"
#include "GPIO.h"

int main(void)
{
	SystemClock();
	GPIO_LEDS_Init();
	GPIO_BOT_Init();
	while(1)
	{
		//GPIO_LED1_Toggle();
		GPIO_LED1_Write(GPIO_BOT_Read());
	}
	return 0;
}
