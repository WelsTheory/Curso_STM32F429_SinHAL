/*
 * PWM.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Wels
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "main.h"

void PWM_Gpio(void);

void PWM_Init(void);

void Change_PWM(uint8_t duty_value);

/** SERVO MOTOR **/
void PWM_ServoMotor(void);

/** 2 CANALES **/
void PWM_2Gpio(void);

void PWM_2CH_Init(void);

#endif /* INC_PWM_H_ */
