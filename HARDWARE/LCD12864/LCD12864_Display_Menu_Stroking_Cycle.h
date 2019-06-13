/*
 * LCD12864_Display_Menu_Set_Time.h
 *
 *  Created on: 2018年10月22日
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_STROKING_CYCLE_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_STROKING_CYCLE_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "LCD12864.h"
#include "stdint.h"
#include "relay.h"

#define STORKING_CYCLE_SET		1	//设置位
#define STORKING_CYCLE_INDEX	0	//个位

extern osThreadId Start_LCD12864_Stroking_Cycle_TaskHandle;

void Start_LCD12864_Stroking_Cycle_Task(void const * argument);
void LCD12864_Display_Stroking_Cycle_Page(uint8_t key);
void LCD12864_Display_Stroking_Cycle_Interior(int8_t* p_S_Place,uint8_t* p_S_Stroking_Cycle);
void LCD12864_Display_Stroking_Cycle_Refresh(int8_t* p_S_Place,uint8_t* p_S_Stroking_Cycle);


#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_SET_TIME_H_ */
