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

uint16_t val_adc = 0;
float resul = 0.0;

volatile uint16_t flag_adc = 0;

int main(void)
{
	SystemClock();
	SysTickConfig(180000);
	USART_Init(115200);
	ADC_Init();
	ADC_EnableNVIC();
	ADC_Start();
	while(1)
	{
//		if(ADC_PollforEndConversion(10) == CONV_DONE)
//		{
//			val_adc = ADC_Read();
//			resul = val_adc*3.3/4095;
//			printf("Valor ADC: %d \n\r",val_adc);
//			printf("Valor Voltaje: %.2f \n\r",resul);
//			ADC_Start();
//		}
//		Delay_ms(100);
		if(flag_adc == 1)
		{
			val_adc = ADC_Read();
			resul = val_adc*3.3/4095;
			printf("Valor ADC: %d \n\r",val_adc);
			printf("Valor Voltaje: %.2f \n\r",resul);
			ADC_Start();
			flag_adc = 0;
		}
	}
	return 0;
}

void ADC_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(ADC_IRQn);
	ADC1->SR &= ~(ADC_SR_EOC);
	flag_adc = 1;
}


