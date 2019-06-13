/*
 * SIM800C_Send_State_data.c
 *
 *  Created on: 2019年5月1日
 *      Author: Administrator
 */
#include "SIM800C.h"
#include "SIM800C_Send_State_data.h"
#include "SIM800C_Scheduler_data.h"
#include "main.h"
#include "HUMI.h"
#include "relay.h"
#include "Mqtt.h"
#include "GPRS.h"
extern uint8_t g_send_Onenet_buf[];
extern osThreadId Start_Reset_Sim800c_Task_TaskHandle;
uint8_t g_init_send = ERROR;
void Start_Send_State_data_Task(void const * argument)
{
	xSemaphoreTake(Sim800c_Semaphore,portMAX_DELAY);//获取互斥信号量
	Connect_OneNet();	//SIM800C重新上电后需要连接ONENET的MQTT服务器
	xSemaphoreGive(Sim800c_Semaphore);
	//连接上开机连接上ONENET之后要发送设备信息Version_Information
	if(g_init_send==ERROR)//确定只有第一次开机时候发送设备信息
	{
		xSemaphoreTake(Sim800c_Semaphore,portMAX_DELAY);//获取互斥信号量
		g_init_send = Send_Version_Information();//发送设备信息
		xSemaphoreGive(Sim800c_Semaphore);
	}
	printf("CONNECT_onenet Send_Version_Information\r\n");
	xSemaphoreGive(Sim800c_Semaphore);
	while(1)
	{
		xSemaphoreTake(Sim800c_Semaphore,portMAX_DELAY);//获取互斥信号量
		Send_Temp_Humi_F_R();
		xSemaphoreGive(Sim800c_Semaphore);
		osDelay(5000);
	}
}
/*
 * 发送到ONENET温湿度和当前正反转状态
 */
void Send_Temp_Humi_F_R(void)
{
	uint16_t Pack_Len,i = 0;
	BaseType_t err = 1;
	static uint8_t cnt = 0;
	uint32_t NotifyValue;
	uint8_t cmp;

	while(GPRS_AT_CIPSEND(AT_CIPSEND,">")==ERROR)
	{
		printf("Send_Temp_Humi_F_R CIPSEND reset\r\n");
		xSemaphoreGive(Sim800c_Semaphore);
		taskENTER_CRITICAL();				//进入临界区
		osThreadDef(Reset_Sim800c_Task, Start_Reset_Sim800c_Task, osPriorityNormal, 0, 128);	//开启重启Sim800C初始化调度函数
		Start_Reset_Sim800c_Task_TaskHandle = osThreadCreate(osThread(Reset_Sim800c_Task), NULL);
		taskEXIT_CRITICAL();				//退出临界区
	}
	Pack_Len = init_OneNet_Temp_Humi_F_R_Pack();
	Send_To_Uart2_Str((int8_t*)g_send_Onenet_buf,Pack_Len);
	//当我等待这个SEND OK的时候不能让查询信号发出数据
	err = xTaskNotifyWait((uint32_t)0x00,
			(uint32_t)0xffffffff,
			(uint32_t*)&NotifyValue,
			(TickType_t)15000);//持续15秒钟时间
	cmp = strcmp(Send_data_recv,"SEND OK");//对比是不是这个数据
	if(err == pdFALSE){
		cnt++;
		if(cnt == 3){
			cnt = 0;
			printf("Send_Temp_Humi_F_R = err = %d\r\n",err);
			xSemaphoreGive(Sim800c_Semaphore);
			taskENTER_CRITICAL();				//进入临界区
			osThreadDef(Reset_Sim800c_Task, Start_Reset_Sim800c_Task, osPriorityNormal, 0, 128);	//开启重启Sim800C初始化调度函数
			Start_Reset_Sim800c_Task_TaskHandle = osThreadCreate(osThread(Reset_Sim800c_Task), NULL);
			taskEXIT_CRITICAL();	
		}
		//return ERROR;
	}else if((err == pdTRUE)&&(NotifyValue == RCV_SEND_OK)&&(cmp == 0)){
		cnt = 0;
	//	return SUCCESS;
	}
}

/*
 * 温湿度和正反转状态返回过去
 */
uint16_t init_OneNet_Temp_Humi_F_R_Pack(void)
{
	uint8_t index = 0;
	uint8_t JsonStr[128];
	G_Send_Pack.Pack_type = 0x30;
	G_Send_Pack.TopicNameLen = 0x0003;
	memcpy(G_Send_Pack.TopicName,"$dp",strlen("$dp"));
	G_Send_Pack.DpType = 0x03;

	sprintf(JsonStr,"{\"Temp\":\"%02d.%d\",\"Humi\":\"%d\"}",(uint16_t)temp/10,(uint16_t)temp%10,(uint16_t)g_rh);

	G_Send_Pack.JsonStr = cJSON_Parse(JsonStr);
	if(!G_Send_Pack.JsonStr){
		printf("get root faild !\r\n");
	}

	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"State","0");

	char* s;
	s = cJSON_PrintUnformatted(G_Send_Pack.JsonStr);
	cJSON_Delete(G_Send_Pack.JsonStr);
	if (G_Send_Pack.JsonStr)
		cJSON_Delete(G_Send_Pack.JsonStr);
	index = Create_Send_Pack(s);
	return index;
}




