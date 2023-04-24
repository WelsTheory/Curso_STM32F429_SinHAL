/*
 * INT.c
 *
 *  Created on: Mar 22, 2023
 *      Author: Wels
 */
#include "INT.h"
#include "GPIO.h"

void INT_Init(void)
{
	// Habilitamos el reloj
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	// Seleccionamos Puerto
	SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13);
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	// No enmascarar
	EXTI->IMR |= EXTI_IMR_IM13;
	EXTI->EMR |= EXTI_EMR_EM13;
	// DESCENDENTE
	EXTI->FTSR |= EXTI_FTSR_TR13;
	// ASCENDENTE NO
	EXTI->RTSR &= ~(EXTI_RTSR_TR13);
	// PRIORIDAD
	NVIC_SetPriority(EXTI15_10_IRQn,5);
	// ENABLE
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Funcion Interrupcion
void EXTI15_10_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	EXTI->PR |= EXTI_PR_PR13;
	GPIO_LED1_Toggle();
}

