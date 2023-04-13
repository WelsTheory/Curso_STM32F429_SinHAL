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

const uint8_t msg[] = "\nSuscribete para mas!!!\r\n";
volatile uint8_t tx_pos = 0;

volatile uint8_t rxbuf = 0;
char msg2[10];
volatile uint8_t done = 0;

int main(void)
{
	SystemClock();
	SysTickConfig(180000);
	USART_Init(115200);
	//USART_EnableNVIC_Tx();
	USART_EnableNVIC_Rx();
	while(1)
	{
		//printf("Hola mundo: %d"data++);
		//Delay_ms(100);
		if(done == 1){
			printf("%s \n",msg2);
			done = 0;
		}
	}
	return 0;
}

void USART3_IRQHandler(void)
{
	//    if (USART3->SR & USART_SR_TXE) {
	//
	//        if (tx_pos == sizeof(msg)) {
	//            tx_pos = 0;
	//            USART3->CR1 &= ~(USART_CR1_TXEIE);
	//        }
	//        else {
	//        	USART3->DR = msg[tx_pos++];
	//        }
	//    }
	if(USART3->SR & USART_SR_RXNE){
		msg2[rxbuf++] = USART3->DR;
		if(rxbuf == sizeof(msg2)){
			rxbuf = 0;
			done = 1;
			USART3->SR &= ~(USART_SR_RXNE);
		}
	}
}
