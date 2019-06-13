/*
 * 24c0x.h
 *
 *  Created on: 2018年10月16日
 *      Author: YuanWP
 */

#ifndef HARDWARE_24C0X_24C0X_H_
#define HARDWARE_24C0X_24C0X_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "delay.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02

extern uint16_t G_Delay_Inspection;

void AT24CXX_Init(void);
void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);	//指定地址开始写入指定长度的数据
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   	//从指定地址开始读出指定长度的数据

void Check_Delay_Inspection(void);
void Init_Delay_Inspection(uint16_t Delay_Inspection);
#endif /* HARDWARE_24C0X_24C0X_H_ */

