/*
 * main.c
 *
 *  Created on: Mar 14, 2023
 *      Author: Wels
 */
#include "main.h"
#include "RCC.h"
#include "GPIO.h"
#include "SysTick.h"
#include "Timer2.h"

extern __IO uint32_t flag_timer;

int main(void)
{
	SystemClock();
	SysTickConfig(180000);
	GPIO_LEDS_Init();
	//Timer2_Delay_Init(9000);
	Timer2_Period_Init(9000, 250);
	while(1)
	{
		if(flag_timer == 1)
		{
			GPIO_LED1_Toggle();
			GPIO_PD7_Toggle();
			flag_timer = 0;
		}
		//Timer2_Delay_ms(100);
	}
	return 0;
}
