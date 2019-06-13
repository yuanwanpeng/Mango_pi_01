/*
 * LCD12864_Display_Menu_Set_Strategy_Info.h
 *
 *  Created on: 2018年11月7日
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_STRATEGY_INFO_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_STRATEGY_INFO_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "LCD12864_Display_Menu.h"
#include "TypeFace.h"
#include "24c0x.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"

#define STRATEGY_INFO_SWITCH 		0
#define STRATEGY_INFO_STANDARD 		1
#define STRATEGY_INFO_TIME_HOURS 	2
#define STRATEGY_INFO_TIME_MINUTES	3
#define STRATEGY_INFO_ROUTE_4 		4//千位
#define STRATEGY_INFO_ROUTE_3 		5//百位
#define STRATEGY_INFO_ROUTE_2 		6//十位
#define STRATEGY_INFO_ROUTE_1 		7//个位
#define STRATEGY_INFO_MIN_TEMP		8
#define STRATEGY_INFO_MAX_TEMP		9
#define STRATEGY_INFO_SET			10
#define STRATEGY_INFO_TEMP_SECTION	3

extern osThreadId Start_LCD12864_Set_Strategy_Info_TaskHandle;
extern uint16_t G_Delay_Inspection;
void Start_LCD12864_Set_Strategy_Info_Task(void const * argument);
void LCD12864_Set_Strategy_Info_Up_Key_Opt(uint8_t* p_Place,Line_Num* p_Line_Num);
void LCD12864_Set_Strategy_Info_Down_Key_Opt(uint8_t* p_Place,Line_Num* p_Line_Num);
void LCD12864_Set_Strategy_Info_Line1(uint8_t* p_Place,Line_Num* p_Line_Num);
void LCD12864_Set_Strategy_Info_Line2(uint8_t* p_Place,Line_Num* p_Line_Num);
void LCD12864_Set_Strategy_Info_Line3(uint8_t* p_Place,Line_Num* p_Line_Num);
void LCD12864_Set_Strategy_Info_Line4(uint8_t* p_Place,Line_Num* p_Line_Num);

#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_STRATEGY_INFO_H_ */
