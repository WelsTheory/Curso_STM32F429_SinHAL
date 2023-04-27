/*
 * USART.c
 *
 *  Created on: Apr 13, 2023
 *      Author: Wels
 */
#include "USART.h"

void GPIO_USART(void)
{
	//PD8 -> TX  | PD9 -> RX
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	// AF
	GPIOD->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
	GPIOD->MODER |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
	// PUPDR
	GPIOD->OTYPER &= ~(GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	// PUPDR
	GPIOD->PUPDR &= ~(GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
	// SPEED
	GPIOD->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9);
	// AF7 -> TX Y RX
	// AFRH AFRL
	GPIOD->AFR[1] &= ~(GPIO_AFRH_AFSEL8 | GPIO_AFRH_AFSEL9);
	GPIOD->AFR[1] |= (GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL8_1 | GPIO_AFRH_AFSEL8_0);
	GPIOD->AFR[1] |= (GPIO_AFRH_AFSEL9_2 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_0);

}

void USART_Init(uint32_t baud)
{
	GPIO_USART();
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	// BRR = (2*clock + baudios)/2*baudios
	USART3->BRR = (2*CLOCK_45MHZ+baud)/(2*baud);
	//USART3->BRR = 390;
	//USART3->BRR |= 0x18<<4;
	//USART3->BRR |= 0x7;
	// 8 BITS
	USART3->CR1 &= ~(USART_CR1_M);
	// PARIDAD DESHABILITADA
	USART3->CR1 &= ~(USART_CR1_PCE);
	//
	USART3->CR1 &= ~(USART_CR1_PS);
	// STOP
	USART3->CR2 &= ~(USART_CR2_STOP);
	// FLOW CONTROL DESAHBILITADO
	USART3->CR3 &= ~(USART_CR3_CTSE | USART_CR3_RTSE);
	// FULL DUPLEX
	USART3->CR3 &= ~(USART_CR3_HDSEL);
	// HABILITAR TX Y RX
	USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	// USART ENABLE
	USART3->CR1 |= USART_CR1_UE;
}

void USART_Tx(char data){
	while((USART3->SR & USART_SR_TC) == 0);
	USART3->DR = data;//1FF
}

char USART_Rx(void)
{
	while((USART3->SR & USART_SR_RXNE) == 0);
	return (USART3->DR & 0xFF);
}

/* INTERRUPCION USART */
//USART3_IRQn
void USART_EnableNVIC_TX(void)
{
	USART3->CR1 |= USART_CR1_TXEIE;
	NVIC_SetPriority(USART3_IRQn,6);
	NVIC_EnableIRQ(USART3_IRQn);
}

void USART_DisableNVIC_TX(void)
{
	USART3->CR1 &= ~USART_CR1_TXEIE;
	NVIC_DisableIRQ(USART3_IRQn);
}

void USART_EnableNVIC_Rx(void)
{
	USART3->CR1 |= USART_CR1_RXNEIE;
	NVIC_SetPriority(USART3_IRQn,6);
	NVIC_EnableIRQ(USART3_IRQn);
}

void USART_DisableNVIC_Rx(void)
{
	USART3->CR1 &= ~USART_CR1_RXNEIE;
	NVIC_DisableIRQ(USART3_IRQn);
}




