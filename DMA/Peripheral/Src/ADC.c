/*
 * ADC.c
 *
 *  Created on: Apr 13, 2023
 *      Author: Wels
 */
#include "ADC.h"
#include "SysTick.h"

void ADC_Init(void)
{
	// PA3 -> ENTRADA
	// ENABLE CLOCK
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	//
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// MODO ANALOGICO
	GPIOA->MODER |= GPIO_MODER_MODE3;
	// ALINEACION DERECHA
	ADC1->CR2 &= ~ADC_CR2_ALIGN;
	// SINGLE
	ADC1->CR2 &= ~ADC_CR2_CONT;
	// PRESCALER F/2
	ADC->CCR &= ~ADC_CCR_ADCPRE;
	// RESOLUCION 12 BITS
	ADC1->CR1 &= ~(ADC_CR1_RES);
	// MUESTREO 3 CYCLES
	ADC1->SMPR2 &= ~(ADC_SMPR2_SMP0);
	// N° CONVERSION
	ADC1->SQR1 &= ~(ADC_SQR1_L);
	// SECUENCIA 0 => CANAL 3 PA3
	ADC1->SQR3 = 3;
	// SWSTART
	ADC1->CR2 |= ADC_CR2_SWSTART;
	// ADC ON
	ADC1->CR2 |= ADC_CR2_ADON;
	// DELAY 10 MS
	Delay_ms(10);
}

void ADC_Start(void)
{
	ADC1->SR = 0;
	// SWSTART
	ADC1->CR2 |= ADC_CR2_SWSTART;
	// ADC ON
	ADC1->CR2 |= ADC_CR2_ADON;
}

void ADC_Stop(void)
{
	ADC1->SR = 0;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint16_t ADC_Read(void)
{
	return (ADC1->DR & 0x0FFF);
}

bool ADC_PollforEndConversion(uint32_t timeout)
{
	uint32_t starTime = msGetTicks();
	while((ADC1->SR & ADC_SR_EOC) == 0)
	{
		if(msGetTicks() - starTime > timeout)
		{
			return false;
		}
	}
	return true;
}

void ADC_EnableNVIC(void)
{
	ADC1->CR1 |= ADC_CR1_EOCIE;
	NVIC_SetPriority(ADC_IRQn,4);
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_DisableNVIC(void)
{
	ADC1->CR1 &= ~ADC_CR1_EOCIE;
	NVIC_DisableIRQ(ADC_IRQn);
}




