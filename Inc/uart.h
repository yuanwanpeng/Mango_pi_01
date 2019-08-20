/*
 * uart.h
 *
 *  Created on: 2018Äê7ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include "stdint.h"
#include <string.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "SIM800c.h"
#include "GPRS.h"
//#include "dma.h"
#define MAX_RECV_LEN 256



extern SemaphoreHandle_t xSemaphore;

void Send_To_Uart2_Str(int8_t* str,uint32_t len);

char *itoa(char *string, int32_t value, int radix);
//void Reset_Uart_DMA(GPRS_TypeDef *_GPRS);



#endif /* INC_UART_H_ */


