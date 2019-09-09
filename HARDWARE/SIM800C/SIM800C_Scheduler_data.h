/*
 * SIM800C_Scheduler_data.h
 *
 *  Created on: 2019Äê5ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef HARDWARE_SIM800C_SIM800C_SCHEDULER_DATA_H_
#define HARDWARE_SIM800C_SIM800C_SCHEDULER_DATA_H_
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "SIM800C.h"

#define RCV_ERROR 0
#define RCV_OK 1
#define RCV_WRITE_SYMBEL 2
#define RCV_SEND_OK 3
#define RCV_CMD_DAT 3
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern osThreadId Start_SIM800C_TaskHandle;	//800c¾ä±ú
void Start_Scheduler_data_Task(void const * argument);
void Process_USART2_Dat(GPRS_TypeDef	* _GPRS);
uint8_t Auto_Return_Data(GPRS_TypeDef	* _GPRS);
uint16_t Clear_Recv_Data(void);
#endif /* HARDWARE_SIM800C_SIM800C_SCHEDULER_DATA_H_ */
