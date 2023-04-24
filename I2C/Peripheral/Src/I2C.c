/*
 * I2C.c
 *
 *  Created on: Apr 21, 2023
 *      Author: Wels
 */
#include "I2C.h"
#include "SysTick.h"

static void I2C_Gpio(void)
{
	// I2C1 -> PB8 Y PB9
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// MODO ALTERNO
	GPIOB->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
	GPIOB->MODER |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
	// AF4 -> I2C
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8 | GPIO_AFRH_AFSEL9);
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_2);
	// PULL UP
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0);
	// OPEN DRAIN
	GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;
	// VERY HIGH SPEED
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9;
}

void I2C_Init(void)
{
	I2C_Gpio();
	// CLOCK
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	// RESET
	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;
	// STANDARD MODE
	I2C1->CCR &= ~I2C_CCR_FS;
	// TH = 5000nS
	I2C1->CCR |= 225<<0;
	// Tsubida = 1000ns
	I2C1->TRISE = 46;
	// CLOCK DEL I2C
	I2C1->CR2 |= 45<<I2C_CR2_FREQ_Pos;
	// HABILITAR PERIFERICO
	I2C1->CR1 |= I2C_CR1_PE;
}

bool I2C_SlaveExist(uint8_t address, uint32_t timeout)
{
	uint32_t startTime = msGetTicks();
	while(I2C1->SR2 & I2C_SR2_BUSY)
	{
		// BUS OCUPADO
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	// BUS ESTÁ LIBRE
	I2C1->CR1 &= ~I2C_CR1_POS;
	I2C1->CR1 |= I2C_CR1_START;// START
	while((I2C1->SR1 & I2C_SR1_SB) == 0)
	{
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	// DIRECCION
	I2C1->DR = address;
	while((I2C1->SR1 & I2C_SR1_ADDR) == 0)
	{
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	// STOP
	I2C1->CR1 |= I2C_CR1_STOP;
	uint32_t temp = I2C1->SR1 | I2C1->SR2;
	(void)temp;
	while(I2C1->SR2 & I2C_SR2_BUSY)
	{
		// BUS OCUPADO
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	return true;
}

bool I2C_masterTransmit(uint8_t address, uint8_t* pDato, uint32_t len, uint32_t timeout)
{
	uint32_t startTime = msGetTicks();
	while(I2C1->SR2 & I2C_SR2_BUSY)
	{
		// BUS OCUPADO
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	// BUS ESTÁ LIBRE
	I2C1->CR1 &= ~I2C_CR1_POS;
	I2C1->CR1 |= I2C_CR1_START;// START
	while((I2C1->SR1 & I2C_SR1_SB) == 0)
	{
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	// DIRECCION
	I2C1->DR = address;// address + r/w
	while((I2C1->SR1 & I2C_SR1_ADDR) == 0)
	{
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	uint32_t temp = I2C1->SR1 | I2C1->SR2;
	(void)temp;
	uint32_t dataIdx = 0;
	uint32_t size = len;//10
	while(size > 0)
	{
		// NO ESTÁ VACÍO
		while((I2C1->SR1 & I2C_SR1_TXE) == 0)
		{
			if((msGetTicks() - startTime) > timeout)
			{
				return false;
			}
		}
		// VACIO
		I2C1->DR = pDato[dataIdx];
		while((I2C1->SR1 & I2C_SR1_BTF) == 0)
		{
			if((msGetTicks() - startTime) > timeout)
			{
				return false;
			}
		}
		size--;
		dataIdx++;
	}
	// STOP
	I2C1->CR1 |= I2C_CR1_STOP;
	temp = I2C1->SR1 | I2C1->SR2;
	(void)temp;
	while(I2C1->SR2 & I2C_SR2_BUSY)
	{
		// BUS OCUPADO
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	return true;
}

bool I2C_masterReceive(uint8_t address, uint8_t* pData, uint32_t len, uint32_t timeout)
{
	uint32_t startTime = msGetTicks();
	while(I2C1->SR2 & I2C_SR2_BUSY)
	{
		// BUS OCUPADO
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	// BUS ESTÁ LIBRE
	I2C1->CR1 &= ~I2C_CR1_POS;
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;// START
	while((I2C1->SR1 & I2C_SR1_SB) == 0)
	{
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	// DIRECCION
	I2C1->DR = address | 0x01;// address + r/w
	while((I2C1->SR1 & I2C_SR1_ADDR) == 0)
	{
		if((msGetTicks() - startTime) > timeout)
		{
			return false;
		}
	}
	uint32_t temp = I2C1->SR1 | I2C1->SR2;
	(void)temp;
	if(len == 0)
	{
		temp = I2C1->SR1 | I2C1->SR2;
		(void)temp;
		I2C1->CR1 |= I2C_CR1_STOP;
		return false;
	}
	else if(len == 1)
	{
		I2C1->CR1 &= ~I2C_CR1_ACK;
		temp = I2C1->SR1 | I2C1->SR2;
		(void)temp;
		I2C1->CR1 |= I2C_CR1_STOP;
	}
	else if(len == 2)
	{
		I2C1->CR1 &= ~I2C_CR1_ACK;
		I2C1->CR1 |= I2C_CR1_POS;
		temp = I2C1->SR1 | I2C1->SR2;
		(void)temp;
	}
	else
	{
		I2C1->CR1 &= ~I2C_CR1_ACK;
		temp = I2C1->SR1 | I2C1->SR2;
		(void)temp;
		I2C1->CR1 |= I2C_CR1_STOP;
	}
	uint32_t dataIdx = 0;
	uint32_t size = len;//10
	while(size > 0)
	{
		if(size <= 3)
		{
			if(size == 1)
			{
				// NO ESTÁ VACÍO
				while((I2C1->SR1 & I2C_SR1_RXNE) == 0)
				{
					if((msGetTicks() - startTime) > timeout)
					{
						return false;
					}
				}
				// VACIO
				pData[dataIdx] = I2C1->DR;
				dataIdx++;
				size--;
			}
			else if(size == 2)
			{
				while((I2C1->SR1 & I2C_SR1_BTF) == 0)
				{
					if((msGetTicks() - startTime) > timeout)
					{
						return false;
					}
				}
				I2C1->CR1 |= I2C_CR1_STOP;
				// DATO 0
				pData[dataIdx] = I2C1->DR;
				dataIdx++;
				size--;
				// DATO 1
				pData[dataIdx] = I2C1->DR;
				dataIdx++;
				size--;
			}
			else
			{
				while((I2C1->SR1 & I2C_SR1_BTF) == 0)
				{
					if((msGetTicks() - startTime) > timeout)
					{
						return false;
					}
				}
				// DATO 2
				I2C1->CR1 &= ~I2C_CR1_ACK;
				pData[dataIdx] = I2C1->DR;
				dataIdx++;
				size--;
				while((I2C1->SR1 & I2C_SR1_BTF) == 0)
				{
					if((msGetTicks() - startTime) > timeout)
					{
						return false;
					}
				}
				I2C1->CR1 |= I2C_CR1_STOP;
				// DATO 1
				pData[dataIdx] = I2C1->DR;
				dataIdx++;
				size--;
				// DATO 0
				pData[dataIdx] = I2C1->DR;
				dataIdx++;
				size--;
			}
		}
		else
		{
			// NO ESTÁ VACÍO
			while((I2C1->SR1 & I2C_SR1_RXNE) == 0)
			{
				if((msGetTicks() - startTime) > timeout)
				{
					return false;
				}
			}
			// DATO N
			pData[dataIdx] = I2C1->DR;
			dataIdx++;
			size--;
			while((I2C1->SR1 & I2C_SR1_BTF) == 0)
			{
				if((msGetTicks() - startTime) > timeout)
				{
					return false;
				}
			}
		}

	}
	return true;
}







