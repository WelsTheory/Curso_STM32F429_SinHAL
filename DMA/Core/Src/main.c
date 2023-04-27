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

#define ARRAY_SIZE 3

uint16_t data_array[ARRAY_SIZE] = {0};

volatile uint32_t flag = 0;


int main(void)
{
	SystemClock();
	SysTickConfig(180000);
	USART_Init(115200);
	printf("Bienvenido Clase DMA \r\n");
	ADC_Multicanal();
	ADC_DMAConfig(data_array, ARRAY_SIZE, (uint32_t)&ADC1->DR);
	ADC_Start();
	while(1)
	{
		if(flag == 1)
		{
			for(uint16_t i = 0; i< ARRAY_SIZE; i++)
			{
				printf("ADC BUFFER[%d]: %d\r\n",i, data_array[i]);
			}
			printf("-------------------\r\n");
			Delay_ms(500);
			flag = 0;
			ADC_Start();
		}
	}
	return 0;
}

void DMA2_Stream0_IRQHandler(void)
{
	if(DMA2->LISR & DMA_LISR_TCIF0)
	{
		NVIC_ClearPendingIRQ(DMA2_Stream0_IRQn);
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
		flag = 1;
		ADC_Stop();
	}
	if(DMA2->LISR & DMA_LISR_HTIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CHTIF0;
	}
}


