/*
 * USART.h
 *
 *  Created on: Apr 13, 2023
 *      Author: Wels
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include "main.h"

#define CLOCK_45MHZ	45000000UL

void GPIO_USART(void);

void USART_Init(uint32_t baud);

void USART_Tx(char data);

char USART_Rx(void);

/**/
void USART_EnableNVIC_Tx(void);

void USART_DisableNVIC_Tx(void);

void USART_EnableNVIC_Rx(void);

void USART_DisableNVIC_Rx(void);

#endif /* INC_USART_H_ */
