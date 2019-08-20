/*
 * SIM800C_Send_State_data.h
 *
 *  Created on: 2019Äê5ÔÂ1ÈÕ
 *      Author: Administrator
 */

#ifndef HARDWARE_SIM800C_SIM800C_SEND_STATE_DATA_H_
#define HARDWARE_SIM800C_SIM800C_SEND_STATE_DATA_H_

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "SIM800C.h"

void Start_Send_State_data_Task(void const * argument);
void Send_Temp_Humi_F_R(void);
char* init_Temp_Humi_F_R_Pack(void);
#endif /* HARDWARE_SIM800C_SIM800C_SEND_STATE_DATA_H_ */
