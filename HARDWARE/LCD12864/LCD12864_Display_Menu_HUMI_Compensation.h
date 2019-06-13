/*
 * LCD12864_Display_Menu_HUMI_Compensation.h
 *
 *  Created on: 2018Äê10ÔÂ19ÈÕ
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_HUMI_COMPENSATION_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_HUMI_COMPENSATION_H_
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "LCD12864.h"
#include "stdint.h"
#include "relay.h"

extern osThreadId Start_LCD12864_HUMI_Compensation_TaskHandle;

void Start_LCD12864_HUMI_Compensation_Task(void const * argument);
void LCD12864_Display_HUMI_Compensation_Page(uint8_t key);
void LCD12864_Display_HUMI_Compensation_Interior(int8_t* p_S_com,uint8_t* p_Coordinate);
void LCD12864_Display_HUMI_Compensation_Refresh(int8_t* p_S_comp,uint8_t* p_Coordinate);

#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_HUMI_COMPENSATION_H_ */
