/*
 * Check_Stroking_Cycle.h
 *
 *  Created on: 2018年10月25日
 *      Author: YuanWP
 */

#ifndef HARDWARE_RELAY_CHECK_STROKING_CYCLE_H_
#define HARDWARE_RELAY_CHECK_STROKING_CYCLE_H_
#include "stdint.h"
#include "button.h"

extern osTimerId Stroking_Cycle_Timer01Handle;

void Start_Check_Stroking_Cycle_Task(void const * argument);//开始行程周期监测任务

#endif /* HARDWARE_RELAY_CHECK_STROKING_CYCLE_H_ */
