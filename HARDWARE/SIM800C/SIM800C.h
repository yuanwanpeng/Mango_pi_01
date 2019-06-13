#ifndef HARDWARE_SIM800C_SIM800C_H_
#define HARDWARE_SIM800C_SIM800C_H_
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include <string.h>
#include "SIM800C_Send_State_data.h"
#include "SIM800C_Recv_Onenet_data.h"

#define SIM800C_PWKEY_CTL_L 		HAL_GPIO_WritePin(PWKEY_CTL_GPIO_Port, PWKEY_CTL_Pin, GPIO_PIN_RESET);
#define SIM800C_PWKEY_CTL_H			HAL_GPIO_WritePin(PWKEY_CTL_GPIO_Port, PWKEY_CTL_Pin, GPIO_PIN_SET);

extern SemaphoreHandle_t Sim800c_Semaphore;//这是一个互斥信号量
void Start_Sim800c_Task(void const * argument);
void Start_Sim800c_Rcv_Task(void const * argument);
void init_Sim800C(void);
void Start_Reset_Sim800c_Task(void const * argument);
void Reset_Uart_DMA(void);
#endif
