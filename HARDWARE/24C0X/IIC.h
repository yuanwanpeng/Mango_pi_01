/*
 * 24c0x.h
 *
 *  Created on: 2018年10月16日
 *      Author: YuanWP
 */

#ifndef HARDWARE_24C0X_IIC_H_
#define HARDWARE_24C0X_IIC_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "delay.h"

#define AT24 		0
#define AM2320 	1

#define IIC_AT24_SDA_H (HAL_GPIO_WritePin(SDA_24C0X_GPIO_Port, SDA_24C0X_Pin, GPIO_PIN_SET))
#define IIC_AT24_SDA_L (HAL_GPIO_WritePin(SDA_24C0X_GPIO_Port, SDA_24C0X_Pin, GPIO_PIN_RESET))
#define IIC_AT24_SCL_H (HAL_GPIO_WritePin(SCL_24C0X_GPIO_Port, SCL_24C0X_Pin, GPIO_PIN_SET))
#define IIC_AT24_SCL_L (HAL_GPIO_WritePin(SCL_24C0X_GPIO_Port, SCL_24C0X_Pin, GPIO_PIN_RESET))

#define IIC_AM2320_SDA_H (HAL_GPIO_WritePin(SDA_AM2320_GPIO_Port, SDA_AM2320_Pin, GPIO_PIN_SET))
#define IIC_AM2320_SDA_L (HAL_GPIO_WritePin(SDA_AM2320_GPIO_Port, SDA_AM2320_Pin, GPIO_PIN_RESET))
#define IIC_AM2320_SCL_H (HAL_GPIO_WritePin(SCL_AM2320_GPIO_Port, SCL_AM2320_Pin, GPIO_PIN_SET))
#define IIC_AM2320_SCL_L (HAL_GPIO_WritePin(SCL_AM2320_GPIO_Port, SCL_AM2320_Pin, GPIO_PIN_RESET))

#define IIC_AM2320_SDA_STATE	(HAL_GPIO_ReadPin(SDA_AM2320_GPIO_Port, SDA_AM2320_Pin))

//IIC所有操作函数

void SDA_IN(uint8_t hardware);
void SDA_OUT(uint8_t hardware);


void IIC_Init(uint8_t hardware);                //初始化IIC的IO口
void IIC_Start(uint8_t hardware);			//发送IIC开始信号
void IIC_Stop(uint8_t hardware);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t hardware,uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(uint8_t hardware,unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(uint8_t hardware); 				//IIC等待ACK信号
void IIC_Ack(uint8_t hardware);					//IIC发送ACK信号
void IIC_NAck(uint8_t hardware);				//IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);

#endif /* HARDWARE_24C0X_IIC_H_ */
