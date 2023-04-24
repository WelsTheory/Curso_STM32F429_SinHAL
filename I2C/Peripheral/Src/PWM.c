/*
 * PWM.c
 *
 *  Created on: Apr 19, 2023
 *      Author: Wels
 */
#include "PWM.h"

void PWM_Gpio(void)
{
	//PE9 -> TIM1 CH1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	// MODO FUNCTION ALTERNA
	GPIOE->MODER &= ~GPIO_MODER_MODE9;
	GPIOE->MODER |= GPIO_MODER_MODE9_1;
	// PUSH PULL
	GPIOE->OTYPER &= ~(GPIO_OTYPER_OT9);
	// LOW SPEED
	GPIOE->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR9);
	// NO PULL UP - PULL DOWN
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD9);
	// AF1 -> TIM1 CH1
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL9;
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL9_0;
}

void PWM_Init(void)
{
	PWM_Gpio();
	// clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	// ASCENDENTE
	TIM1->CR1 &= TIM_CR1_DIR;
	// ALINEACION IZQUIERDA
	TIM1->CR1 &= ~TIM_CR1_CMS;
	// CLOCK DIVISION
	TIM1->CR1 &= ~TIM_CR1_CKD;
	// F = 10KHz -> 18000
	TIM1->ARR = 18000 - 1;
	// PRESCALER = 0
	TIM1->PSC = 0;
	// HABILITAMOS SALIDA
	TIM1->BDTR |= TIM_BDTR_MOE;
	// HABILITANDO CANAL 1
	TIM1->CCER |= TIM_CCER_CC1E;
	// SALIDA
	TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
	// PWM MODO 1 (110)
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	// DUTY CYCLE 50%
	TIM1->CCR1 = 8999;
	// COUNTER ENABLE
	TIM1->CR1 |= TIM_CR1_CEN;
}

void Change_PWM(uint8_t duty_value)
{
	uint32_t period = TIM1->ARR;
	if(duty_value > 100)
	{
		duty_value = 100;
	}
	TIM1->CCR1 = (uint16_t)((duty_value/100.0f)*(period+0.0f));
}

void PWM_ServoMotor(void)
{
	PWM_Gpio();
	// clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	// ASCENDENTE
	TIM1->CR1 &= TIM_CR1_DIR;
	// ALINEACION IZQUIERDA
	TIM1->CR1 &= ~TIM_CR1_CMS;
	// CLOCK DIVISION
	TIM1->CR1 &= ~TIM_CR1_CKD;
	// F = 50Hz -> 20000
	TIM1->ARR = 20000 - 1;
	// PRESCALER = 180
	TIM1->PSC = 180 - 1;
	// HABILITAMOS SALIDA
	TIM1->BDTR |= TIM_BDTR_MOE;
	// HABILITANDO CANAL 1
	TIM1->CCER |= TIM_CCER_CC1E;
	// SALIDA
	TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
	// PWM MODO 1 (110)
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	// DUTY CYCLE 50%
	TIM1->CCR1 = 10000;
	// COUNTER ENABLE
	TIM1->CR1 |= TIM_CR1_CEN;
}

/** 2 CANALES **/
void PWM_2Gpio(void)
{
	//PE9 -> TIM1 CH1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	// MODO FUNCTION ALTERNA
	GPIOE->MODER &= ~GPIO_MODER_MODE9;
	GPIOE->MODER |= GPIO_MODER_MODE9_1;
	// PUSH PULL
	GPIOE->OTYPER &= ~(GPIO_OTYPER_OT9);
	// LOW SPEED
	GPIOE->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR9);
	// NO PULL UP - PULL DOWN
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPD9);
	// AF1 -> TIM1 CH1
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFSEL9;
	GPIOE->AFR[1] |= GPIO_AFRH_AFSEL9_0;
	//PB6 -> TIM4 CH1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// MODO FUNCTION ALTERNA
	GPIOB->MODER &= ~GPIO_MODER_MODE6;
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	// PUSH PULL
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT6);
	// LOW SPEED
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6);
	// NO PULL UP - PULL DOWN
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6);
	// AF2 -> TIM1 CH1
	GPIOB->AFR[0] &= ~GPIO_AFRH_AFRH6;
	GPIOB->AFR[0] |= GPIO_AFRH_AFRH6_1;
}

void PWM_2CH_Init(void)
{
	PWM_2Gpio();
	// clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	// ASCENDENTE
	TIM1->CR1 &= TIM_CR1_DIR;
	TIM4->CR1 &= TIM_CR1_DIR;
	// ALINEACION IZQUIERDA
	TIM1->CR1 &= ~TIM_CR1_CMS;
	TIM4->CR1 &= ~TIM_CR1_CMS;
	// CLOCK DIVISION
	TIM1->CR1 &= ~TIM_CR1_CKD;
	TIM4->CR1 &= ~TIM_CR1_CKD;
	// F = 50Hz -> 20000
	TIM1->ARR = 20000 - 1;
	TIM4->ARR = 10000 - 1; // 50Hz
	// PRESCALER = 180
	TIM1->PSC = 180 - 1;
	TIM4->PSC = 180 - 1;
	// HABILITAMOS SALIDA
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM4->BDTR |= TIM_BDTR_MOE;
	// HABILITANDO CANAL 1
	TIM1->CCER |= TIM_CCER_CC1E;
	TIM4->CCER |= TIM_CCER_CC1E;
	// SALIDA
	TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	// PWM MODO 1 (110)
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM1->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	TIM4->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM4->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	// DUTY CYCLE 50%
	TIM1->CCR1 = 10000;
	TIM4->CCR1 = 5000;
	// COUNTER ENABLE
	TIM1->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;
}


