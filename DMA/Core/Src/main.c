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

int main(void)
{
	SystemClock();
	SysTickConfig(180000);
	USART_Init(115200);
	printf("Bienvenido Clase DMA \r\n");
	while(1)
	{

	}
	return 0;
}


