/*
 * dma.c
 *
 *  Created on: 2018年10月30日
 *      Author: YuanWP
 */
#include "dma.h"

extern DMA_HandleTypeDef hdma_usart2_tx;
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//chx:DMA通道选择,DMA1_Channel1~DMA1_Channel7
void MYDMA_Config(DMA_Channel_TypeDef *chx)
{
	__HAL_RCC_DMA1_CLK_ENABLE();			//DMA1时钟使能

    __HAL_LINKDMA(&huart2,hdmatx,hdma_usart2_tx);    		//将DMA与USART1联系起来(发送DMA)

    //Rx DMA配置
    hdma_usart2_tx.Instance=chx;                            		//通道选择
    hdma_usart2_tx.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
    hdma_usart2_tx.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式
    hdma_usart2_tx.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式
    hdma_usart2_tx.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
    hdma_usart2_tx.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
    hdma_usart2_tx.Init.Mode=DMA_NORMAL;                            //外设普通模式
    hdma_usart2_tx.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级

    HAL_DMA_DeInit(&hdma_usart2_tx);
    HAL_DMA_Init(&hdma_usart2_tx);
}
//开启一次DMA传输
//huart:串口句柄
//pData：传输的数据指针
//Size:传输的数据量
void MYDMA_USART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    HAL_DMA_Start(huart->hdmatx, (uint32_t)pData, (uint32_t)&huart->Instance->DR, Size);//开启DMA传输

    huart->Instance->CR3 |= USART_CR3_DMAT;//使能串口DMA发送
}

