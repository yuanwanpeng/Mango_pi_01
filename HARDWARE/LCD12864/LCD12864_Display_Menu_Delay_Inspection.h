/*
 * LCD12864_Display_Menu_Delay_Inspection.h
 *
 *  Created on: 2018年10月22日
 *      Author: YuanWP
 */

#ifndef HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_DELAY_INSPECTION_H_
#define HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_DELAY_INSPECTION_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "LCD12864.h"
#include "stdint.h"
#include "relay.h"

#define DELAY_INSPECTION_0 		0	//百位
#define DELAY_INSPECTION_1 		1	//十位
#define DELAY_INSPECTION_2 		2	//个位
#define DELAY_INSPECTION_SET 	3	//设置位


extern osThreadId Start_LCD12864_Delay_Inspection_TaskHandle;

void Start_LCD12864_Delay_Inspection_Task(void const * argument);
void LCD12864_Display_Delay_Inspection_Page(uint8_t key);
void LCD12864_Display_Delay_Inspection_Interior(uint16_t* p_S_Delay_Inspection,uint8_t* p_Coordinate);
void LCD12864_Display_Delay_Inspection_Refresh(uint16_t* p_S_Delay_Inspection,uint8_t* p_Coordinate);


#endif /* HARDWARE_LCD12864_LCD12864_DISPLAY_MENU_DELAY_INSPECTION_H_ */
