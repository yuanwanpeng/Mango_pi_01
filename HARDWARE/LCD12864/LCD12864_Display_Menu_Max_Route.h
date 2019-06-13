/*
 * LCD12864_Display_Menu_Max_Route.h
 *
 *  Created on: 2018Äê10ÔÂ19ÈÕ
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_MAX_ROUTE_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_MAX_ROUTE_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "LCD12864.h"
#include "stdint.h"
#include "relay.h"

extern osThreadId Start_LCD12864_Max_Route_TaskHandle;

void Start_LCD12864_Max_Route_Task(void const * argument);
void LCD12864_Display_Max_Route_Page(uint8_t key);
void LCD12864_Display_Max_Route_Interior(uint16_t* p_S_Max_Route,uint8_t* p_Coordinate);
void LCD12864_Display_Max_Route_Refresh(uint16_t* p_S_Max_Route,uint8_t* p_Coordinate);


#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_MAX_ROUTE_H_ */
