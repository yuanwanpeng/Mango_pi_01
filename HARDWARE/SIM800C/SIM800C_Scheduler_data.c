/*
 * SIM800C_Scheduler_data.c
 *
 *  Created on: 2019年5月5日
 *      Author: Administrator
 */

#include "SIM800C_Scheduler_data.h"
#include <string.h>
#include "SIM800C.h"

#define CMP_AT 							(strstr(_GPRS->P_CMD, "AT\r\n"))
#define CMP_ATE0						(strstr(_GPRS->P_CMD, "ATE0\r\n"))
#define CMP_AT_CIPSRIP			(strstr(_GPRS->P_CMD, "AT+CIPSRIP=1\r\n"))
#define CMP_AT_CSQ					(strstr(_GPRS->P_CMD, "AT+CSQ\r\n"))
#define CMP_AT_CIPCLOSE			(strstr(_GPRS->P_CMD, "AT+CIPCLOSE\r\n"))	
#define CMP_AT_CIPSTART			(strstr(_GPRS->P_CMD, "AT+CIPSTART"))	
#define CMP_AT_CIPSTATUS		(strstr(_GPRS->P_CMD, "AT+CIPSTATUS\r\n"))
#define CMP_AT_CIPSHUT			(strstr(_GPRS->P_CMD, "AT+CIPSHUT\r\n"))
#define CMP_AT_CGDCONT			(strstr(_GPRS->P_CMD, "AT+CGDCONT="))

#define CMP_AT_CIPSEND			(strstr(_GPRS->P_CMD, "AT+CIPSEND="))	
#define CMP_AT_CSMINS				(strstr(_GPRS->P_CMD, "AT+CSMINS?"))

#define DAT_MQTT_DAT				(strstr(_GPRS->P_CMD, "MQTT_DAT"))
#define RECV_MQTT_DAT				(strstr(_GPRS->P_CMD, "RECV"))



extern char * GPRS_Rx_Dat;
//extern uint8_t G_Sim800C_Signal;
extern uint8_t Recv_cmd_data[];
extern SemaphoreHandle_t uart2_Idle_xSemaphore;
extern osThreadId Start_Scheduler_data_Task_TaskHandle;
extern osThreadId Start_Send_State_data_TaskHandle;	//发送数据到上位机句柄
extern osThreadId Start_Recv_Onenet_data_TaskHandle;	//接收数据到上位机句柄
extern GPRS_TypeDef *p_GPRS,GPRS;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern SemaphoreHandle_t recv_gprs_cmd_xSemaphore;//二值信号量
extern SemaphoreHandle_t send_gprs_dat_xSemaphore;//二值信号量

/*
 * 解析sim800C返回数据的task
 */
void Start_Scheduler_data_Task(void const * argument)
{
	
	while(1)
	{
		if( xSemaphoreTake( uart2_Idle_xSemaphore, portMAX_DELAY ) == pdTRUE )
		{
			Process_USART2_Dat(p_GPRS);//确认一帧数据接收完成需要判断数据是什么		
		}
	}
}

uint8_t Auto_Return_Data(GPRS_TypeDef	* _GPRS)
{
	uint8_t clear = 0;
	if(strstr(GPRS_Rx_Dat,"+CFUN: 1"))
	{
		clear = 1;
		_GPRS->CFUN = 1;
	}
	if(strstr(GPRS_Rx_Dat,"+CPIN: READY"))
	{
		clear = 2;
		_GPRS->CPIN = 1;
	}
	if(strstr(GPRS_Rx_Dat,"Call Ready"))
	{
		clear = 3;
		_GPRS->CALL = 1;
	}
	if(strstr(GPRS_Rx_Dat,"SMS Ready"))
	{
		clear = 4;
		_GPRS->SMS = 1;
	}
	if(_GPRS->P_CMD == NULL)//当没有发送命令
	{
		if(strstr(GPRS_Rx_Dat,"CLOSED"))//自动返回了个CLOSED
		{
			printf("close\r\n");
			clear = 5;
			_GPRS->CIPSTATUS = 8;	//设置当前状态为CLOSED
		}
	}
	if(clear != 0)
	{
		Clear_Recv_Data();
	}
	return clear;
}

void Process_USART2_Dat(GPRS_TypeDef	* _GPRS)
{
	BaseType_t xHigherPriorityTaskWoken;


	HAL_UART_DMAStop(&huart2);		//关闭制定的DMA通道x,以免打扰
	GPRS_Rx_Dat = &_GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->Bufuse][0];
	_GPRS->GPRS_BUF->Len[_GPRS->GPRS_BUF->Bufuse] = PDATA_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);//获取接收到的数据的长度
	_GPRS->GPRS_BUF->Bufuse = !_GPRS->GPRS_BUF->Bufuse;					//切换缓冲区BUFF
	HAL_UART_Receive_DMA(&huart2, _GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->Bufuse], PDATA_SIZE);//DMA接收开启

	if(Auto_Return_Data(_GPRS) != 0)	//判断自动返回的数据不等于0 表示为自动返回数据不必下面code处理
	{
		return;
	}

	if(CMP_AT_CIPSEND)		//等待发送数据命令
	{
		//printf("[%s]",GPRS_Rx_Dat);
		if(strstr(GPRS_Rx_Dat,_GPRS->P_CMD_CHECK))	//>\r\n
		{
			_GPRS->ATCIPSEND = 1;
		//	xSemaphoreGiveFromISR(recv_gprs_cmd_xSemaphore,&xHigherPriorityTaskWoken);
		//	xSemaphoreGiveFromISR(send_gprs_dat_xSemaphore,&xHigherPriorityTaskWoken);
			return;
		}
	}
	if(CMP_AT_CSMINS)		//命令
	{
		printf("[%s]",GPRS_Rx_Dat);
		if(strstr(GPRS_Rx_Dat,_GPRS->P_CMD_CHECK))	//+CSMINS:
		{
			_GPRS->ATCSMINS = 1;
		//	xSemaphoreGiveFromISR(recv_gprs_cmd_xSemaphore,&xHigherPriorityTaskWoken);
		//	xSemaphoreGiveFromISR(send_gprs_dat_xSemaphore,&xHigherPriorityTaskWoken);
			return;
		}
	}
	if(DAT_MQTT_DAT)
	{
		if(strstr(GPRS_Rx_Dat,_GPRS->P_CMD_CHECK))	//SEND OK\r\n
		{
			_GPRS->SEND_OK = 1;
			//xSemaphoreGiveFromISR(send_gprs_dat_xSemaphore,&xHigherPriorityTaskWoken);
			return;
		}
	}
	if (CMP_AT_CIPSTATUS)//查询TCP状态
	{
		if(CHECK_NUM_0)		//表示可以连接数据
			_GPRS->CIPSTATUS = 0;
		if(CHECK_NUM_1)		//表示可以连接数据
			_GPRS->CIPSTATUS = 1;
		if(CHECK_NUM_2)		//表示可以连接数据
			_GPRS->CIPSTATUS = 2;
		if(CHECK_NUM_3)		//表示可以连接数据
			_GPRS->CIPSTATUS = 3;
		if(CHECK_NUM_4)		//表示可以连接数据
			_GPRS->CIPSTATUS = 4;
		if(CHECK_NUM_5)		//表示可以连接数据
			_GPRS->CIPSTATUS = 5;
		if(CHECK_NUM_6)		//表示可以连接数据
			_GPRS->CIPSTATUS = 6;
		if(CHECK_NUM_7)		//表示可以连接数据
			_GPRS->CIPSTATUS = 7;
		if(CHECK_NUM_8)		//表示可以连接数据
			_GPRS->CIPSTATUS = 8;
		if(CHECK_NUM_9)		//表示可以连接数据
			_GPRS->CIPSTATUS = 9;
		_GPRS->ATCIPSTATUS = 1;
		//xSemaphoreGiveFromISR(recv_gprs_cmd_xSemaphore,&xHigherPriorityTaskWoken);
		return;
	}
	if (CMP_AT_CIPSHUT||CMP_AT_CGDCONT)
	{
		if(strstr(GPRS_Rx_Dat,"OK\r\n"))
		{
			xSemaphoreGiveFromISR(recv_gprs_cmd_xSemaphore,&xHigherPriorityTaskWoken);
//			taskEXIT_CRITICAL();			//退出临界区
			return;
		}
	}
	

	if(CMP_AT_CIPSTART)	//等待CONNECT OK
	{
		if(strstr(GPRS_Rx_Dat,"CONNECT OK\r\n"))
		{
			_GPRS->CIPSTATUS = 8;
			//xSemaphoreGiveFromISR(recv_gprs_cmd_xSemaphore,&xHigherPriorityTaskWoken);
			return;
		}
	}
	
	
	if(RECV_MQTT_DAT)	//RECV
	{
		if(strstr(GPRS_Rx_Dat,_GPRS->P_CMD_CHECK))	//RECV FROM
		{
			xSemaphoreGiveFromISR(send_gprs_dat_xSemaphore,&xHigherPriorityTaskWoken);
			return;
		}
	}	
	if(CMP_AT)
	{
		if(strstr(GPRS_Rx_Dat,"OK\r\n"))
		{
			_GPRS->AT = 1;
			return;
		}
	}
	if(CMP_ATE0)
	{
		if(strstr(GPRS_Rx_Dat,"OK\r\n"))
		{
			_GPRS->ATE = 1;
			return;
		}
	}
	if(CMP_AT_CIPSRIP)
	{
		if(strstr(GPRS_Rx_Dat,"OK\r\n"))
		{
			_GPRS->CIPSRIP = 1;
			return;
		}
	}
	if (CMP_AT_CSQ)
	{
		if(strstr(GPRS_Rx_Dat,"OK\r\n"))
		{
			_GPRS->ATCSQ = 1;
			return;
		}
	}
	if(CMP_AT_CIPCLOSE)//CIPCLOSE
	{
		if(strstr(GPRS_Rx_Dat,"ERROR\r\n"))
		{
			_GPRS->CIPCLOSE = 1;
			return;
		}
	}
	
	//printf("GPRS_Rx_Dat = [%s]",GPRS_Rx_Dat);//打印出没有能力解析的数据
	//	if (strstr(GPRS_Rx_Dat,"SMS Ready\r\n"))
//		_GPRS->READY++;
	Clear_Recv_Data();
}

void Clear_Recv_Data(void)
{
	memcpy(p_GPRS->DOWN_BUF,p_GPRS->GPRS_BUF->Buf[p_GPRS->GPRS_BUF->RX_Dispose],p_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose]);
	(p_GPRS->DOWN_BUF[p_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose] + 1])  = '\0';//将最后一个数据赋值为0
	memset(p_GPRS->GPRS_BUF->Buf[p_GPRS->GPRS_BUF->RX_Dispose],'\0',strlen(p_GPRS->GPRS_BUF->Buf[p_GPRS->GPRS_BUF->RX_Dispose]));//清空数据
	//p_GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->RX_Dispose]这里面的数据处理完毕
	p_GPRS->GPRS_BUF->RX_Dispose = !p_GPRS->GPRS_BUF->RX_Dispose;//切换待处理的缓冲区指针
}








