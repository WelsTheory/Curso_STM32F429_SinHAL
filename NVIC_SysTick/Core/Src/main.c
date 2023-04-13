/*
 * main.c
 *
 *  Created on: Mar 14, 2023
 *      Author: Wels
 */
#include "main.h"
#include "RCC.h"
#include "GPIO.h"
#include "INT.h"
#include "SysTick.h"

delay_t Tiempo_100;

int main(void)
{
	SystemClock();
	SysTickConfig(180000);
	GPIO_LEDS_Init();
	//GPIO_BOT_Init();
	//INT_Init();
	Delay_ms_NB(&Tiempo_100, 100);
	while(1)
	{
		if(Delay_Read(&Tiempo_100))
		{
			GPIO_LED1_Toggle();
		}
		//Delay_ms(100);

	}
	return 0;
}
