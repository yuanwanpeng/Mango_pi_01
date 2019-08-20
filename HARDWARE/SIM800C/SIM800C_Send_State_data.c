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
#include "LCD12864.h"
extern uint8_t g_send_Server_buf[];
extern osThreadId Start_Reset_Sim800c_Task_TaskHandle;
extern osThreadId Start_Send_State_data_TaskHandle;
extern Delete_Task_struct G_Delete_Task_struct;//删除任务的结构体
uint8_t g_init_send = ERROR;
void Start_Send_State_data_Task(void const * argument)
{
	uint8_t flag;
	Connect_Server();	//SIM800C重新上电后需要连接MQTT服务器
	printf("Connect_Server ok\r\n");
	
	//推送主题
	//连接上开机连接上Server之后要发送设备信息Version_Information
	if(g_init_send==ERROR)//确定只有第一次开机时候发送设备信息
	{
		g_init_send = Send_Version_Information();//发送设备信息
	}
	while(1)
	{
		flag = MQTT_Ping();
		printf("flag:1ok flag = %d\r\n",flag);
		Send_Temp_Humi_F_R();
		osDelay(15000);
	}
}
/*
 * 发送到Mqtt温湿度和当前正反转状态
 */
void Send_Temp_Humi_F_R(void)
{
	uint8_t flag;
	char* s;

	s = init_Temp_Humi_F_R_Pack();
	printf("%s\r\n",s);
	flag = MQTT_Pubtopic(TOPIC_SendData,s);//发送JSON数据
	free(s);s = NULL;
}

/*
 * 温湿度和正反转状态返回过去
 */
char* init_Temp_Humi_F_R_Pack(void)
{
	uint8_t JsonStr[128];

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
	
	
	return s;
}




