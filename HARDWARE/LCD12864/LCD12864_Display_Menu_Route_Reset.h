/*
 * LCD12864_Display_Menu_Route_Reset.h
 *
 *  Created on: 2018Äê10ÔÂ18ÈÕ
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_ROUTE_RESET_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_ROUTE_RESET_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "LCD12864.h"
#include "stdint.h"
extern osThreadId Start_LCD12864_Route_Reset_TaskHandle;

void Start_LCD12864_Route_Reset_Task(void const * argument);
void LCD12864_Display_Route_Reset_Page(uint8_t key);
void LCD12864_Display_Now_Route(void);
void LCD12864_Display_Now_Route_Refresh(void);




#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_ROUTE_RESET_H_ */
