/*
 * LCD12864.h
 *
 *  Created on: 2018年7月25日
 *      Author: Administrator
 */

#ifndef HARDWARE_LCD12864_LCD12864_H_
#define HARDWARE_LCD12864_LCD12864_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "delay.h"
#include "LCD12864_Display_Menu.h"
#include "TypeFace.h"
#include "24c0x.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"
#include "LCD12864_Display_Menu_Route_Reset.h"
#include "LCD12864_Display_Menu_Max_Route.h"
#include "LCD12864_Display_Menu_HUMI_Compensation.h"
#include "LCD12864_Display_Menu_Delay_Inspection.h"
#include "LCD12864_Display_Menu_Set_Time.h"
#include "LCD12864_Display_Menu_Stroking_Cycle.h"

#define CS_L HAL_GPIO_WritePin(CS_LCD_GPIO_Port, CS_LCD_Pin, GPIO_PIN_RESET)
#define RS_L HAL_GPIO_WritePin(RST_LCD_GPIO_Port, RST_LCD_Pin, GPIO_PIN_RESET)
#define A0_L HAL_GPIO_WritePin(A0_LCD_GPIO_Port, A0_LCD_Pin, GPIO_PIN_RESET)
#define SCL_L HAL_GPIO_WritePin(SCL_LCD_GPIO_Port, SCL_LCD_Pin, GPIO_PIN_RESET)
#define SDA_L HAL_GPIO_WritePin(SDA_LCD_GPIO_Port, SDA_LCD_Pin, GPIO_PIN_RESET)

#define CS_H HAL_GPIO_WritePin(CS_LCD_GPIO_Port, CS_LCD_Pin, GPIO_PIN_SET)
#define RS_H HAL_GPIO_WritePin(RST_LCD_GPIO_Port, RST_LCD_Pin, GPIO_PIN_SET)
#define A0_H HAL_GPIO_WritePin(A0_LCD_GPIO_Port, A0_LCD_Pin, GPIO_PIN_SET)
#define SCL_H HAL_GPIO_WritePin(SCL_LCD_GPIO_Port, SCL_LCD_Pin, GPIO_PIN_SET)
#define SDA_H HAL_GPIO_WritePin(SDA_LCD_GPIO_Port, SDA_LCD_Pin, GPIO_PIN_SET)

#define STALLING 	0
#define FOREWARD 	1
#define REVERSAL 	2
#define PLACE_1x16 (0xB0)

#define BLACK_BASE 1
#define WHITE_BASE 0

typedef enum{
	Humi_Page = 1,
	Menu_Page,
	Strategy_Menu,
	Route_Reset_page,
	Max_Route_page,
	HUMI_Compensation_page,
	Delay_Inspection_page,
	Set_Time_page,
	Stroking_Cycle_page,
	Strategy_Menu_Info_page,
}New_page_enum;

typedef struct{
	uint8_t sign;//标志为0不删除，标志位1，就给我删除我艹
	osThreadId D_Task;
}Delete_Task_struct;
extern Delete_Task_struct G_Delete_Task_struct;
extern uint8_t G_New_Mode;
extern volatile New_page_enum Now_page;
extern float g_rh;
void LCD12864_Display(uint8_t dat1,uint8_t dat2);
void LCD12864_Put_16_16_Char(uint8_t row_addr,uint8_t column_addr,uint8_t chr,uint8_t Base_Color);
void LCD12864_Put_8_16_Char(uint8_t row_addr,uint8_t column_addr,uint8_t chr,uint8_t Base_Color);
void Start_LCD12864_Task(void const * argument);

void LCD12864_Refresh_Route(void);
void LCD12864_Refresh_Time(void);
void LCD12864_Put_Symbol_RH(void);
void Start_LCD12864_Display_HUMI_Task(void const * argument);
void LCD12864_Put_Open_Mouth_And_Time(void);
void LCD12864_Put_RH(float RH);

void WriteCommand(uint8_t CommandByte);
void Write_Data(uint8_t dat);
void LCD12864_Init(void);
void LCD12864_Put_Signel_Sim(uint8_t signal);
void LCD12864_Put_Signel_Wifi(void);
void LCD12864_DisplayChar(uint8_t col,uint8_t page,uint8_t Order);
void LCD12864_Put_HUMI(float humi);
void LCD12864_Put_Motor_Situation(uint8_t motor_situation);
void LCD12864_Put_Str_At_Present_Mode(uint8_t mode_num);
void LCD12864_Put_Str_Welcome(void);
void LCD12864_Display_Clear_Success(void);
void LCD12864_Display_Success(void);
#endif /* HARDWARE_LCD12864_LCD12864_H_ */
