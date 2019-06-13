/*
 * LCD12864_Display_Menu_Set_Time.h
 *
 *  Created on: 2018Äê10ÔÂ22ÈÕ
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_TIME_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_TIME_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "LCD12864.h"
#include "stdint.h"
#include "relay.h"

#define SET		3
#define HOURS 	2
#define MINUTES 1
#define SECONDS	0
extern osThreadId Start_LCD12864_Set_Time_TaskHandle;
extern RTC_HandleTypeDef hrtc;
void Start_LCD12864_Set_Time_Task(void const * argument);
void LCD12864_Display_Set_Time_Page(uint8_t key);
void LCD12864_Display_Set_Time_Interior(int8_t* p_S_Place,RTC_TimeTypeDef* p_Time);
void LCD12864_Display_Set_Time_Refresh(int8_t* p_S_Place,RTC_TimeTypeDef* p_Time);


#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_TIME_H_ */
