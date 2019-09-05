#ifndef HARDWARE_SIM800C_SIM800C_H_
#define HARDWARE_SIM800C_SIM800C_H_
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include <string.h>
#include "GPRS.h"
#include "SIM800C_Send_State_data.h"
#include "SIM800C_Recv_Mqtt_data.h"

#define SIM800C_PWKEY_CTL_L 		HAL_GPIO_WritePin(PWKEY_CTL_GPIO_Port, PWKEY_CTL_Pin, GPIO_PIN_RESET);
#define SIM800C_PWKEY_CTL_H			HAL_GPIO_WritePin(PWKEY_CTL_GPIO_Port, PWKEY_CTL_Pin, GPIO_PIN_SET);

#define 	CHECK_0 	"IP INITIAL"
#define 	CHECK_1 	"IP START"
#define 	CHECK_2 	"IP CONFIG"
#define 	CHECK_3 	"IP GPRSACT"
#define 	CHECK_4 	"IP STATUS"
#define 	CHECK_5 	"CONNECTING"		//TCP CONNECTING/UDP CONNECTING/SERVER LISTENING
#define 	CHECK_6 	"CONNECT OK"
#define 	CHECK_7 	"CLOSING"				//TCP CLOSING/UDP CLOSING
#define 	CHECK_8 	"CLOSED"				//TCP CLOSED/UDP CLOSED
#define 	CHECK_9 	"PDP DEACT"			//

#define	CHECK_NUM_0	(strstr(GPRS_Rx_Dat,CHECK_0))
#define	CHECK_NUM_1	(strstr(GPRS_Rx_Dat,CHECK_1))
#define	CHECK_NUM_2	(strstr(GPRS_Rx_Dat,CHECK_2))
#define	CHECK_NUM_3	(strstr(GPRS_Rx_Dat,CHECK_3))
#define	CHECK_NUM_4	(strstr(GPRS_Rx_Dat,CHECK_4))
#define	CHECK_NUM_5	(strstr(GPRS_Rx_Dat,CHECK_5))
#define	CHECK_NUM_6	(strstr(GPRS_Rx_Dat,CHECK_6))
#define	CHECK_NUM_7	(strstr(GPRS_Rx_Dat,CHECK_7))
#define	CHECK_NUM_8	(strstr(GPRS_Rx_Dat,CHECK_8))
#define	CHECK_NUM_9	(strstr(GPRS_Rx_Dat,CHECK_9))


extern SemaphoreHandle_t Sim800c_Semaphore;//这是一个互斥信号量
void Start_Sim800c_Task(void const * argument);
void Start_Sim800c_Rcv_Task(void const * argument);

void Start_Reset_Sim800c_Task(void const * argument);
void reset_Sim800C(void);
#endif
