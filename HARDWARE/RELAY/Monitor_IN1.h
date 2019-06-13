/*
 * Monitor_IN1.h
 *
 *  Created on: 2018年10月25日
 *      Author: YuanWP
 */

#ifndef HARDWARE_RELAY_MONITOR_IN1_H_
#define HARDWARE_RELAY_MONITOR_IN1_H_
#include "Check_Stroking_Cycle.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "relay.h"
#include "24c0x.h"

#define IN1_STATE 	HAL_GPIO_ReadPin(IN1_GPIO_Port, IN1_Pin);

void Start_Monitor_IN1_Task(void const * argument);//开始行程周期监测任务
void Save_G_Now_Route(int16_t Now_Route);                     


#endif /* HARDWARE_RELAY_MONITOR_IN1_H_ */
