/*
 * SysTick.c
 *
 *  Created on: Mar 22, 2023
 *      Author: Wels
 */
#include "SysTick.h"

static __IO uint32_t msTick = 0;

void SysTickConfig(uint32_t systick_valor)
{
	// Reinicio
	SysTick->CTRL = 0;
	// Cargamos el valor
	SysTick->LOAD = systick_valor - 1;
	// Reinicio contador
	SysTick->VAL = 0;
	// Prioridad
	NVIC_SetPriority(SysTick_IRQn,0);
	// Fuente de Reloj
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	// Generación de excepción
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	// Enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	msIncTicks();
}

void msIncTicks(void)
{
	msTick++;
}

uint32_t msGetTicks(void)
{
	return msTick;
}

void Delay_ms(uint32_t delay)//100
{
	uint32_t startTime = msGetTicks();//100
	while((msGetTicks() - startTime) < delay);
}


/*****/

void Delay_ms_NB(delay_t * delay, uint32_t duration)
{
	delay->duration = duration/1;
	delay->running = 0;
}

bool Delay_Read(delay_t * delay)
{
	bool TimeArrived = 0;
	if(!delay->running)
	{
		delay->startTime = msGetTicks();
		delay->running = 1;
	}
	else
	{
		if((msGetTicks() - delay->startTime) >= delay->duration)
		{
			TimeArrived = 1;
			delay->running = 0;
		}
	}

	return TimeArrived;
}


