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

/** ADC MULTICANAL - DMA **/
void ADC_Multicanal(void)
{
	// PA3, PC0 Y PC3 -> ENTRADA
	// ENABLE CLOCK
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	// GPIO A GPIO C
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	// MODO ANALOGICO
	GPIOA->MODER |= GPIO_MODER_MODE3;
	GPIOC->MODER |= GPIO_MODER_MODE0 | GPIO_MODER_MODE3;
	// SCAN ENABLE
	ADC1->CR1 |= ADC_CR1_SCAN;
	// MUESTREO 3 CYCLES
	ADC1->SMPR2 &= ~(ADC_SMPR2_SMP0 | ADC_SMPR2_SMP1 |ADC_SMPR2_SMP2);
	// N° CONVERSION 3
	ADC1->SQR1 &= ~(ADC_SQR1_L);
	ADC1->SQR1 |= (ADC_SQR1_L_1);
	// SECUENCIA 0 => CANAL 3 PA3
	/**
	 * PA3 -> CONVERSION 1
	 * PC0 -> CONVERSION 2
	 * PC3 -> CONVERSION 3
	 * */
	ADC1->SQR3 = 3;
	ADC1->SQR3 |= (ADC_SQR3_SQ2_3 | ADC_SQR3_SQ2_1);//AN10
	ADC1->SQR3 |= (ADC_SQR3_SQ3_3 | ADC_SQR3_SQ3_2 |ADC_SQR3_SQ3_0);//AN13
	// ALINEACION DERECHA
	ADC1->CR2 &= ~ADC_CR2_ALIGN;
	// SINGLE
	ADC1->CR2 |= ADC_CR2_CONT;
	// CONVERSION POR GRUPO
	ADC1->CR2 &= ~ADC_CR2_EOCS;
	// PRESCALER F/2
	ADC->CCR &= ~ADC_CCR_ADCPRE;
	// RESOLUCION 12 BITS
	ADC1->CR1 &= ~(ADC_CR1_RES);
	// DMA
	ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_DDS;
	// ADC ON
	ADC1->CR2 |= ADC_CR2_ADON;
	// DELAY 10 MS
	Delay_ms(10);
}

void ADC_DMAConfig(uint32_t *pData, uint32_t length, uint32_t base)
{
	// Clock Enable
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	// DMA2 -> STREAM 0 -> CANAL 0
	DMA2_Stream0->CR = 0;
	// Periferico direccion Origen
	DMA2_Stream0->PAR = base;
	// Memoria Destino
	DMA2_Stream0->M0AR = (uint32_t)pData;
	// Total
	DMA2_Stream0->NDTR = length;
	// PERIFERICO A MEMORIA
	DMA2_Stream0->CR &= ~DMA_SxCR_DIR;
	// 16 BITS
	DMA2_Stream0->CR &= ~DMA_SxCR_PSIZE;
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;
	DMA2_Stream0->CR &= ~DMA_SxCR_MSIZE;
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;
	// NO INCREMENTAR DIRECCION
	DMA2_Stream0->CR &= ~DMA_SxCR_PINC;
	// INCREMENTA DATA
	DMA2_Stream0->CR |= DMA_SxCR_MINC;
	// MODO CIRCULAR
	DMA2_Stream0->CR |= DMA_SxCR_CIRC;
	// TRANSFER COMPLETE
	DMA2_Stream0->CR |= DMA_SxCR_TCIE;
	// HALF COMPLETE
	DMA2_Stream0->CR &= ~DMA_SxCR_HTIE;
	// NVIC
	NVIC_SetPriority(DMA2_Stream0_IRQn,3);
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	// ENABLE
	DMA2_Stream0->CR |= DMA_SxCR_EN;
}



