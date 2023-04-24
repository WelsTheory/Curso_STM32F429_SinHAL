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
#include "USART.h"
#include "ADC.h"
#include "PWM.h"

int main(void)
{
	char duty = 0;
	SystemClock();
	SysTickConfig(180000);
	USART_Init(115200);
	printf("Bienvenido Clase PWM \r\n");
	//PWM_Init();
	//Change_PWM(10);
	/* SERVO */
	//PWM_ServoMotor();
	//Change_PWM(1);
	/* 2 PWM */
	PWM_2CH_Init();
	Change_PWM(25);
	while(1)
	{
//		Change_PWM(duty++);
//		if(duty> 100)
//		{
//			duty = 0;
//		}
//		Delay_ms(500);
	}
	return 0;
}


