/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stdint.h"
#include <stdlib.h> 
#include "board_info.h"
#define SIM800C_VERSION
#ifdef SIM800C_VERSION
#include "SIM800C.h"
#endif
#include "uart.h"
#include "cJSON.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BACKLIGHT_Pin GPIO_PIN_13
#define BACKLIGHT_GPIO_Port GPIOC
#define SDA_AM2320_Pin GPIO_PIN_0
#define SDA_AM2320_GPIO_Port GPIOB
#define SCL_AM2320_Pin GPIO_PIN_1
#define SCL_AM2320_GPIO_Port GPIOB
#define PWKEY_CTL_Pin GPIO_PIN_14
#define PWKEY_CTL_GPIO_Port GPIOB
#define SCL_24C0X_Pin GPIO_PIN_15
#define SCL_24C0X_GPIO_Port GPIOB
#define SDA_24C0X_Pin GPIO_PIN_6
#define SDA_24C0X_GPIO_Port GPIOC
#define OUTC3_Pin GPIO_PIN_7
#define OUTC3_GPIO_Port GPIOC
#define OUTC2_Pin GPIO_PIN_8
#define OUTC2_GPIO_Port GPIOC
#define OUTC1_Pin GPIO_PIN_9
#define OUTC1_GPIO_Port GPIOC
#define IN1_Pin GPIO_PIN_8
#define IN1_GPIO_Port GPIOA
#define B_IN4_Pin GPIO_PIN_11
#define B_IN4_GPIO_Port GPIOA
#define B_IN3_Pin GPIO_PIN_12
#define B_IN3_GPIO_Port GPIOA
#define SDA_LCD_Pin GPIO_PIN_15
#define SDA_LCD_GPIO_Port GPIOA
#define SCL_LCD_Pin GPIO_PIN_10
#define SCL_LCD_GPIO_Port GPIOC
#define A0_LCD_Pin GPIO_PIN_11
#define A0_LCD_GPIO_Port GPIOC
#define RST_LCD_Pin GPIO_PIN_12
#define RST_LCD_GPIO_Port GPIOC
#define CS_LCD_Pin GPIO_PIN_2
#define CS_LCD_GPIO_Port GPIOD
#define Buzzer_Pin GPIO_PIN_3
#define Buzzer_GPIO_Port GPIOB
#define LED_REV_Pin GPIO_PIN_4
#define LED_REV_GPIO_Port GPIOB
#define LED_FWD_Pin GPIO_PIN_5
#define LED_FWD_GPIO_Port GPIOB
#define MCU_RUN_LED_Pin GPIO_PIN_6
#define MCU_RUN_LED_GPIO_Port GPIOB
#define IN2_Pin GPIO_PIN_7
#define IN2_GPIO_Port GPIOB
#define B_IN2_Pin GPIO_PIN_8
#define B_IN2_GPIO_Port GPIOB
#define B_IN1_Pin GPIO_PIN_9
#define B_IN1_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

typedef enum{
	Line1 = 0,
	Line2 = 1,
	Line3 = 2,
	Line4 = 3,
	Line5 = 4,
	Line6 = 5,
	Line7 = 6,
	Line8 = 7,
}Line_Num;

typedef struct{
	uint8_t Master_Control;			//主控类型 	交流卷膜，直流卷膜，但接触器（风机），双接触器（棉被），数据采集器
	uint8_t Device_Type;			//硬件的类型   单机版，gprs版本，wifi版本
	uint8_t Soft_Versions[10];		//软件版本
	uint8_t Hardware_Versions[10];	//硬件版本
	uint8_t Device_Id[10];			//设备ID 	供ONENET使用 实际使用9个字节
	uint8_t Date_Of_Manufacture[8];	//生产日期
	uint8_t Serial_Number[10];		//流水号4个字节
}Version_Information_Struct;

void Init_Strategy(uint8_t num);
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
