/*
 * ADC.h
 *
 *  Created on: Apr 13, 2023
 *      Author: Wels
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"

#define CONV_DONE 1

void ADC_Init(void);

void ADC_Start(void);

void ADC_Stop(void);

uint16_t ADC_Read(void);

bool ADC_PollforEndConversion(uint32_t timeout);

/** ADC INTERRUPCIONES **/
void ADC_EnableNVIC(void);

void ADC_DisableNVIC(void);


#endif /* INC_ADC_H_ */
