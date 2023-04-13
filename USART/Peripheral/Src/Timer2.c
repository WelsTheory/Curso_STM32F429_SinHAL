/*
 * Timer2.c
 *
 *  Created on: Mar 29, 2023
 *      Author: Wels
 */
#include "Timer2.h"

__IO uint32_t flag_timer = 0;

void Timer2_Delay_Init(uint32_t arr_load)
{
	// Habilito el Clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// Modo Ascendente
	TIM2->CR1 &= ~(TIM_CR1_DIR);
	// One pulse mode
	TIM2->CR1 |= TIM_CR1_OPM;
	// Modo Reset
	TIM2->CR2 &= ~(TIM_CR2_MMS);
	// Prescaler = 0
	TIM2->PSC = 0;
	// Tiempo 0.1ms
	TIM2->ARR = arr_load - 1;
}

void Timer2_Delay_ms(uint32_t delay)
{
	// 100ms
	// 0.1ms -> 1000
	TIM2->PSC = (delay*10)-1;
	// Habilitar Timer
	TIM2->CR1 |= TIM_CR1_CEN;
	// Esperar evento
	while((TIM2->SR & TIM_SR_UIF) == 0);
	// UIF ->1
	TIM2->SR &= ~(TIM_SR_UIF);
}

/* PART 2  -> TIM INT */
void Timer2_Period_Init(uint32_t arr_load, uint32_t delay)
{
	// Habilito el Clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// Modo Ascendente
	TIM2->CR1 &= ~(TIM_CR1_DIR);
	// Periodo
	TIM2->CR1 &= ~TIM_CR1_OPM;
	// Modo Reset
	TIM2->CR2 &= ~(TIM_CR2_MMS);
	// Prescaler = 0
	TIM2->PSC = (delay*10)-1;
	// Tiempo 0.1ms
	TIM2->ARR = arr_load - 1;
	// UIF ->1
	TIM2->SR &= ~(TIM_SR_UIF);
	// Int Timer Habilitada
	TIM2->DIER |= TIM_DIER_UIE;
	// NVIC
	NVIC_SetPriority(TIM2_IRQn,2);
	// NVIC ENABLE
	NVIC_EnableIRQ(TIM2_IRQn);
	// Habilitar Timer
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	TIM2->SR &= ~(TIM_SR_UIF);
	flag_timer = 1;
}


