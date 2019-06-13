/*
 * GPRS.h
 *
 *  Created on: 2018年11月28日
 *      Author: YuanWP
 */

#include "GPRS.h"
#include "uart.h"
#include "SIM800C_Scheduler_data.h"


extern uint8_t pData[];
char Send_data_recv[128];
uint8_t GPRS_AT(uint8_t* cmd,uint8_t* check)
{
	Send_To_Uart2_Str(AT,strlen(AT));

	if(pData == NULL)
		return ERROR;

	else if (strstr((const char *)pData,(const char *)check))
		return SUCCESS;
	else return ERROR;
}

uint8_t GPRS_AT_CGDCONT(uint8_t* cmd,uint8_t* check)
{
	Send_To_Uart2_Str(AT_CGDCONT,strlen(AT_CGDCONT));
	if(pData == NULL)
		return ERROR;

	else if (strstr((const char *)pData,(const char *)check))
		return SUCCESS;
	else return ERROR;
}

uint8_t GPRS_AT_CGATT(uint8_t* cmd,uint8_t* check)
{
	Send_To_Uart2_Str(AT_CGATT,strlen(AT_CGATT));
	osDelay(1000);
	if(pData == NULL)
		return ERROR;

	else if (strstr((const char *)pData,(const char *)check))
		return SUCCESS;
	else return ERROR;
}

uint8_t GPRS_AT_CIPCSGP(uint8_t* cmd,uint8_t* check)
{
	Send_To_Uart2_Str(AT_CIPCSGP,strlen(AT_CIPCSGP));
	if(pData == NULL)
		return ERROR;

	else if (strstr((const char *)pData,(const char *)check))
		return SUCCESS;
	else return ERROR;
}

uint8_t GPRS_AT_CIPSTATUS(uint8_t* cmd,uint8_t* check)
{
	Send_To_Uart2_Str(AT_CIPSTATUS,strlen(AT_CIPSTATUS));
	if(pData == NULL)
		return ERROR;

	else if (strstr((const char *)pData,(const char *)check))
		return SUCCESS;
	else return ERROR;
}

uint8_t GPRS_AT_CIPSTART(uint8_t* cmd,uint8_t* check)
{
	Send_To_Uart2_Str(AT_CIPSTART,strlen(AT_CIPSTART));
	osDelay(1000);
	while(strstr((const char *)pData,check) == NULL)//没有数据返回
	{
		if((strstr((const char *)pData,"PDP ") != NULL)||(strstr((const char *)pData,"ERROR") != NULL)){//返回PDP或者ERROR了
//			printf("2AT_CIPSTART pData = %s\r\n", pData);
			return ERROR;
		}
//		printf("2AT_CIPSTART pData = %s\r\n", pData);
		osDelay(5000);
	}
//	printf("3AT_CIPSTART pData = %s\r\n", pData);
	return SUCCESS;
}
uint8_t GPRS_AT_CSQ(uint8_t* cmd)
{
	Send_To_Uart2_Str(AT_CSQ,strlen(AT_CSQ));
	return 0;
}

uint8_t GPRS_ATE0(uint8_t* cmd)
{
	Send_To_Uart2_Str(ATE0,strlen(ATE0));
	return 0;
}

uint8_t GPRS_AT_CIPSEND(uint8_t* cmd,uint8_t* check)
{
	BaseType_t err = 1;
	uint32_t NotifyValue;
	Send_To_Uart2_Str(AT_CIPSEND,strlen(AT_CIPSEND));
	err = xTaskNotifyWait((uint32_t)0x00,
			(uint32_t)0xffffffff,
			(uint32_t*)&NotifyValue,
			(TickType_t)10000);
	if(err == pdFALSE){
		return ERROR;
	}else if((err == pdTRUE)&&(NotifyValue == RCV_WRITE_SYMBEL)){
		return SUCCESS;
	}
}


