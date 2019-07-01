/*
 * uart.c
 *
 *  Created on: 2018年7月24日
 *      Author: Administrator
 */
#include "uart.h"
#include <stdarg.h>
#include "cmsis_os.h"
#include "semphr.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void USARTx_Printf(UART_HandleTypeDef* huartx, char *Data,...);

uint8_t pData[PDATA_SIZE];
uint8_t aRxBuffer;			//接收中断缓冲
uint8_t Uart1_RxBuff[64];		//接收缓冲
uint8_t Uart1_Rx_Cnt = 0;		//接收缓冲计数
uint8_t	cAlmStr[] = "数据溢出(大于256)\r\n";

uint8_t a2RxBuffer;			//接收中断缓冲
uint8_t Uart2_RxBuff[MAX_RECV_LEN];		//接收缓冲
uint8_t Uart2_Rx_Cnt = 0;		//接收缓冲计数
extern uint8_t *G_p_index_start,*G_p_index_end;

void Reset_Uart_DMA(void)
{
	HAL_UART_DMAStop(&huart2);
	memset(pData,'\0',PDATA_SIZE);
	HAL_UART_Receive_DMA(&huart2, pData, PDATA_SIZE);
	G_p_index_start = pData;
	G_p_index_end = pData;
}


/*
 *     串口2发送
 */
void Send_To_Uart2_Str(int8_t* str,uint32_t len)
{	
	while(HAL_UART_Transmit_DMA(&huart2, (uint8_t*)str, len) != HAL_OK)
	{
		osDelay(100);
	}
	osDelay(100);
}





/********************************************************************************
 *  @brief  HAL_UART_RxCpltCallback  进入中断后HAL库会调用这个函数
* 	@param  串口句柄
*	 @retval NONE
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken;
	if(USART1 == huart->Instance)//调试串口1
	{
		if(Uart1_Rx_Cnt >= (MAX_RECV_LEN+1))  //溢出判断
		{
			Uart1_Rx_Cnt = 0;
			memset(Uart1_RxBuff,0x00,sizeof(Uart1_RxBuff));
			HAL_UART_Transmit(&huart1, (uint8_t *)&cAlmStr, sizeof(cAlmStr),0xFFFF);
		}else{
			Uart1_RxBuff[Uart1_Rx_Cnt++] = aRxBuffer;   //接收数据转存

			if((Uart1_RxBuff[Uart1_Rx_Cnt-1] == 0x0A)&&(Uart1_RxBuff[Uart1_Rx_Cnt-2] == 0x0D)) //判断结束位
			{

				xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
				//HAL_UART_Transmit(&huart1, (uint8_t *)&Uart1_RxBuff, Uart1_Rx_Cnt,0xFFFF); //将收到的信息发送出去
				//Uart1_Rx_Cnt = 0;
				//memset(Uart1_RxBuff,0x00,sizeof(Uart1_RxBuff)); //清空数组
			}
		}
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
	}

	if(USART2 == huart->Instance)//wifi模块串口2
	{
//		if(Uart2_Rx_Cnt >= (MAX_RECV_LEN+1))  //溢出判断
//		{
//			Uart2_Rx_Cnt = 0;
//			memset(Uart2_RxBuff,0x00,sizeof(Uart2_RxBuff));
//			//HAL_UART_Transmit(&huart1, (uint8_t *)&cAlmStr, sizeof(cAlmStr),0xFFFF);//发送给串口一
//		}else{
//			Uart2_RxBuff[Uart2_Rx_Cnt++] = a2RxBuffer;   //接收数据转存**********重点

//			if(a2RxBuffer == 0x0D)
//			{	
//					xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
//			}
//		}
//		HAL_UART_Receive_IT(&huart2, (uint8_t *)&a2RxBuffer, 1);   //再开启接收中断
	}
}



/****************************************************************************
* 函数名	:	itoa
* 描述  :	将整形数据转换成字符串
* 输入  :	-radix =10 表示10进制，其他结果为0
*        	-value 要转换的整形数
*        	-buf 转换后的字符串
* 输出  :	无
* 返回  :	无
* 调用  :	被USARTx_printf()调用
***************************************************************************/
char *itoa(char *string, int32_t value, int radix)
{
    int32_t     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 1000000000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

void USARTx_Send_Dat(UART_HandleTypeDef* huartx, uint16_t dat)
{
	//USART_SendData(USARTx,dat);
	HAL_UART_Transmit(huartx, (uint8_t*)&dat, 1, 0xFFFF);
	while( __HAL_UART_GET_FLAG(huartx, UART_FLAG_TXE) == RESET );
	//while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
}

/****************************************************************************
*	函数名	:	USARTx_Printf
*	描述	:	格式化输出，类似于C库中的printf，但这里没有用到C库
* 输入	:	-USARTx 串口通道，这里只用到了串口2，即USART2
*		     	-Data   要发送到串口的内容的指针
*			  	 -...   其他参数
* 输出	:	无
* 返回	:	无 
* 调用	:	外部调用
*					典型应用	USARTx_printf( USART1, "\r\n this is a demo \r\n" );
*									USARTx_printf( USART2, "\r\n %d \r\n", i );
*									USARTx_printf( USART3, "\r\n %s \r\n", j );
****************************************************************************/
void USARTx_Printf(UART_HandleTypeDef* huartx, char *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5C )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USARTx_Send_Dat(huartx, 0x0D);
					Data ++;
					break;

				case 'n':							          //换行符
					USARTx_Send_Dat(huartx, 0x0A);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{	
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
						USARTx_Send_Dat(huartx,*s);
					Data++;
          break;

        case 'd':										 //十进制
          d = va_arg(ap, int);
          itoa(buf ,d, 10);
          for (s = buf; *s; s++) 
						USARTx_Send_Dat(huartx,*s);
					Data++;
          break;
				
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else 
			USARTx_Send_Dat(huartx, *Data++);
	}
}


