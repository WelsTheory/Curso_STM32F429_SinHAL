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
#include "I2C.h"
#include "MPU_Libreria.h"

#define MPU_I2C	0xD0

int main(void)
{
	SystemClock();
	SysTickConfig(180000);
	USART_Init(115200);
	printf("Bienvenido Clase I2C \r\n");
	I2C_Init();
	if(I2C_SlaveExist(MPU_I2C, 5)){
		printf("Existe Sensor\r\n");
	}
	else
	{
		printf("No Existe Sensor\r\n");
		while(1);
	}
	MPU_ConfigTypeDef myMPU;
	myMPU.Accel_Full_Scale = AFS_SEL_4g;
	myMPU.ClockSource = Internal_8MHz;
	myMPU.CONFIG_DLPF = DLPF_184A_188G_Hz;
	myMPU.Sleep_Mode_Bit = 0;
	myMPU.Gyro_Full_Scale = FS_SEL_500;
	MPU6050_Config(&myMPU);
	ScaledData_Def Accel_Data;
	ScaledData_Def Gyro_Data;
	RawData_Def Accel_Raw;
	while(1)
	{
		MPU6050_Get_Accel_Scale(&Accel_Data);
		MPU6050_Get_Accel_RawData(&Accel_Raw);
		MPU6050_Get_Gyro_Scale(&Gyro_Data);
		printf("Gyro: X=%.2f, Y=%.2f, Z=%.2f\r\n", Gyro_Data.x,Gyro_Data.y, Gyro_Data.z);
		printf("Accel: X=%.2f, Y=%.2f, Z=%.2f\r\n", Accel_Data.x,Accel_Data.y, Accel_Data.z);
		printf("RAW: X=%d, Y=%d, Z=%d\r\n", Accel_Raw.x,Accel_Raw.y, Accel_Raw.z);
		printf("---------------------------\r\n");
		Delay_ms(500);
	}
	return 0;
}


