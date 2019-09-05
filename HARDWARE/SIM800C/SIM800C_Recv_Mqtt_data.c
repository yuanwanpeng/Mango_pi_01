/*
 * SIM800C_Recv_Onenet_data.c
 *
 *  Created on: 2019年5月5日
 *      Author: Administrator
 */


#include "SIM800C_Recv_Mqtt_data.h"
#include "main.h"
#include "HUMI.h"
#include "relay.h"
#include "Mqtt.h"
#include "GPRS.h"
#include <string.h>
uint8_t Recv_cmd_data[512];

/*
 * 解析接收数据的函数
 */
void Start_Recv_Mqtt_data_Task(void const * argument)
{
	BaseType_t err = 1;
	uint32_t NotifyValue;
	cJSON* item_cmd;
	cJSON* root;
	int8_t* p_data;
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,
				(uint32_t)0xffffffff,
				(uint32_t*)&NotifyValue,
				(TickType_t)portMAX_DELAY);
		root = cJSON_Parse((const char*)Recv_cmd_data);
		item_cmd = cJSON_GetObjectItem(root, "cmd");//{"cmd":"set_cmd","max_route":"100"}TopicNameLen = 42
		if(item_cmd != NULL){
			p_data = cJSON_Print(item_cmd);//p_data是命令的类型
			printf("pdata = %s\r\n",p_data);

			if(!strcmp(p_data,"\"set_cmd\"")){//匹配是设置命令类型
				free(p_data);p_data=NULL;
				item_cmd = cJSON_GetObjectItem(root, "max_route");//获取指令类型
				p_data = cJSON_Print(item_cmd);//p_data是命令的类型
				printf("指令 = %s\r\n",p_data);
				free(p_data);p_data=NULL;
			}
			if(!strcmp(p_data,"\"exe_cmd\"")){

			}
		}
	}
}











