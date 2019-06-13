/*
 * LCD12864_Display_Menu_Set_Strategy.h
 *
 *  Created on: 2018年10月16日
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_STRATEGY_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_STRATEGY_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "LCD12864.h"
#include "stdint.h"
#include "LCD12864_Display_Menu_Set_Strategy_Info.h"

#define Strategy_1	0x00
#define Strategy_2	0x10
#define Strategy_3	0x20
#define Strategy_4	0x30
#define Strategy_5	0x40
#define Strategy_6	0x50
#define Strategy_7	0x60
#define Strategy_8	0x70

#define Strategy_Type_Route	1
#define Strategy_Type_Temp	2
#define Strategy_Open	1
#define Strategy_Close	0




typedef struct
{
	uint8_t Strategy_Num;
	uint8_t Strategy_Type;
	uint8_t Temp_h;
	uint8_t Temp_l;
	uint8_t Strategy_State;
	uint16_t Route;
	RTC_TimeTypeDef Time;
}Strategy_struct;




extern osThreadId Start_LCD12864_Set_Strategy_TaskHandle;
extern Strategy_struct G_Strategy[8];	//8条策略



void Start_LCD12864_Set_Strategy_Task(void const * argument);
void LCD12864_Set_Strategy_Menu(uint8_t key);
void LCD12864_Display_Set_Strategy_Refresh(Line_Num* p_Strategy_this_line,uint8_t opt);

void LCD12864_Set_Strategy_Line(uint8_t row,uint8_t list_num,Line_Num *p_Strategy_this_line,Line_Num This_Line_Num);
void LCD12864_Set_Strategy_Line1(Line_Num *p_Strategy_this_line);
void LCD12864_Set_Strategy_Line2(Line_Num *p_Strategy_this_line);
void LCD12864_Set_Strategy_Line3(Line_Num *p_Strategy_this_line);
void LCD12864_Set_Strategy_Line4(Line_Num *p_Strategy_this_line);
void LCD12864_Set_Strategy_Line5(Line_Num *p_Strategy_this_line);
void LCD12864_Set_Strategy_Line6(Line_Num *p_Strategy_this_line);
void LCD12864_Set_Strategy_Line7(Line_Num *p_Strategy_this_line);
void LCD12864_Set_Strategy_Line8(Line_Num *p_Strategy_this_line);
#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_STRATEGY_H_ */
