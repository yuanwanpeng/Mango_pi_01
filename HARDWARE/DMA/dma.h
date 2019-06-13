/*
 * dma.h
 *
 *  Created on: 2018Äê10ÔÂ30ÈÕ
 *      Author: YuanWP
 */

#ifndef HARDWARE_DMA_DMA_H_
#define HARDWARE_DMA_DMA_H_
#include "stdint.h"
#include <string.h>
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "uart.h"

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef  UART2RxDMA_Handler;      //DMA¾ä±ú
void MYDMA_Config(DMA_Channel_TypeDef *chx);
void MYDMA_USART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

#endif /* HARDWARE_DMA_DMA_H_ */
