#ifndef __LED_H__
#define __LED_H__
//#include "stdint.h"
/*
 *   设备灯运行启动管理函数
 */
void Start_LED_Task(void const * argument);
/*
 *  运行闪烁灯
 */
void Start_RUN_LED_Task(void const * argument);
//void Printf_Task_Size(int8_t* name);
#endif
