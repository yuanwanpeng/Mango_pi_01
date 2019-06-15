/*
 * Mqtt.c
 *
 *  Created on: 2019年3月29日
 *      Author: Administrator
 */
#include "Mqtt.h"
#include "main.h"
#include "GPRS.h"
#include "SIM800C_Scheduler_data.h"
#define DEVICE_ID 	"518898092"
#define PRODUCT_ID 	"217490"
#define API_KEY		"eC8obJLJaS2PEA0SSrF3gcGbDQ8="
extern uint8_t pData[];
Connect_Pack G_Connect_Pack;
Onenet_Pack G_Send_Pack;
uint8_t g_send_Onenet_buf[1024];
extern float temp;
extern osThreadId Start_Reset_Sim800c_Task_TaskHandle;
extern Version_Information_Struct G_Version_Information;
uint16_t init_OneNet_Pack(void)
{
	uint8_t index = 0;

	G_Connect_Pack.Pack_type = 0x10;
	G_Connect_Pack.RemainedLength = 59;		//包长，不算这一位和前一位
	G_Connect_Pack.ProtocolNameLength = 0x0004;		//协议名长度
	memcpy(G_Connect_Pack.ProtocolName,"MQTT",strlen("MQTT"));	// protocolName=MQTT
	G_Connect_Pack.ProtocolLevel = 0x04; 	// protocolLevel=4
	G_Connect_Pack.Pack_Flag = 0xC0;		// userFlag=1 passwordFlag=1 willFlag=0 willRetainFlag=0 willQosFlag=0 clenSessionFlag=0 clenSessionFlag=0
	G_Connect_Pack.KeepAlive = 120; 		// keepAlive=120
	G_Connect_Pack.ClientIdentifierLen = 0x0009;
	memcpy(G_Connect_Pack.ClientIdentifier,DEVICE_ID,strlen(DEVICE_ID));
	G_Connect_Pack.UserNameLen = 0x0006;
	memcpy(G_Connect_Pack.UserName,PRODUCT_ID,strlen(PRODUCT_ID));
	G_Connect_Pack.UserPasswordLen = 0x001C;
	memcpy(G_Connect_Pack.UserPassword,API_KEY,strlen(API_KEY));

	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.Pack_type),sizeof(G_Connect_Pack.Pack_type));
	index += sizeof(G_Connect_Pack.Pack_type);
	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.RemainedLength),sizeof(G_Connect_Pack.RemainedLength));
	index += sizeof(G_Connect_Pack.RemainedLength);
	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.ProtocolNameLength),sizeof(G_Connect_Pack.ProtocolNameLength));

	g_send_Onenet_buf[index++] = G_Connect_Pack.ProtocolNameLength>>8;
	g_send_Onenet_buf[index++] = G_Connect_Pack.ProtocolNameLength;

	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.ProtocolName),strlen((const char*)G_Connect_Pack.ProtocolName));
	index += strlen((const char*)G_Connect_Pack.ProtocolName);
	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.ProtocolLevel),sizeof(G_Connect_Pack.ProtocolLevel));
	index += sizeof(G_Connect_Pack.ProtocolLevel);
	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.Pack_Flag),sizeof(G_Connect_Pack.Pack_Flag));
	index += sizeof(G_Connect_Pack.Pack_Flag);

	g_send_Onenet_buf[index++] = G_Connect_Pack.KeepAlive>>8;
	g_send_Onenet_buf[index++] = G_Connect_Pack.KeepAlive;

	g_send_Onenet_buf[index++] = G_Connect_Pack.ClientIdentifierLen>>8;
	g_send_Onenet_buf[index++] = G_Connect_Pack.ClientIdentifierLen;

	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.ClientIdentifier),strlen((const char*)G_Connect_Pack.ClientIdentifier));
	index += strlen((const char*)G_Connect_Pack.ClientIdentifier);

	g_send_Onenet_buf[index++] = G_Connect_Pack.UserNameLen>>8;
	g_send_Onenet_buf[index++] = G_Connect_Pack.UserNameLen;

	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.UserName),strlen((const char*)G_Connect_Pack.UserName));
	index += strlen((const char*)G_Connect_Pack.UserName);

	g_send_Onenet_buf[index++] = G_Connect_Pack.UserPasswordLen>>8;
	g_send_Onenet_buf[index++] = G_Connect_Pack.UserPasswordLen;
	memcpy(g_send_Onenet_buf+index,&(G_Connect_Pack.UserPassword),strlen((const char*)G_Connect_Pack.UserPassword));
	index += strlen((const char*)G_Connect_Pack.UserPassword);

	g_send_Onenet_buf[index++] = 0x1A;

	return index;//返回编辑的长度
}

/*
 * 通用函数组织发送函数包
 * */
uint16_t Create_Send_Pack(uint8_t* str)
{
	uint8_t index = 0;
	uint8_t i = 0,j;
	uint32_t pack_len=0;
	unsigned long encodedByte;
//	printf("str = %s\r\n",str);
	memset(g_send_Onenet_buf,'\0',sizeof(g_send_Onenet_buf));

	memcpy(g_send_Onenet_buf+index,&(G_Send_Pack.Pack_type),sizeof(G_Send_Pack.Pack_type));
	index += sizeof(G_Send_Pack.Pack_type);
	pack_len = strlen((const char*)str)+8;
	do{
		 encodedByte = pack_len % 128;
		 pack_len = pack_len / 128;
		 if(pack_len > 0){
			 encodedByte = encodedByte | 128;
		 }
		 G_Send_Pack.RemainedLength[i++] =  (unsigned char)encodedByte;
	}while(pack_len);
	for(j = 0;j < i;j++)
	{
		g_send_Onenet_buf[index++] = G_Send_Pack.RemainedLength[j];
	}
	G_Send_Pack.JsonStrLen = strlen((const char*)str);
	g_send_Onenet_buf[index++] = G_Send_Pack.TopicNameLen>>8;
	g_send_Onenet_buf[index++] = G_Send_Pack.TopicNameLen;
	memcpy(g_send_Onenet_buf+index,&(G_Send_Pack.TopicName),strlen((const char*)G_Send_Pack.TopicName));
	index += strlen((const char*)G_Send_Pack.TopicName);
	memcpy(g_send_Onenet_buf+index,&(G_Send_Pack.DpType),sizeof(G_Send_Pack.DpType));
	index += sizeof(G_Send_Pack.DpType);
	g_send_Onenet_buf[index++] = G_Send_Pack.JsonStrLen>>8;
	g_send_Onenet_buf[index++] = G_Send_Pack.JsonStrLen;

	memcpy(g_send_Onenet_buf+index,str,strlen((const char*)str));
	index += strlen((const char*)str);

	g_send_Onenet_buf[index++] = 0x1A;
	
	cJSON_Delete(G_Send_Pack.JsonStr);
	return index;
}

/*
 * 连接Onenet
 */
uint16_t Connect_OneNet(void)
{
	BaseType_t err = 1;
	uint32_t NotifyValue;
	uint16_t Pack_Len;
	uint8_t cmp;
	while(GPRS_AT_CIPSEND(AT_CIPSEND,">")==ERROR)
	{
		printf("Connect_OneNet CIPSEND reset\r\n");
		xSemaphoreGive(Sim800c_Semaphore);
		taskENTER_CRITICAL();				//进入临界区
		osThreadDef(Reset_Sim800c_Task, Start_Reset_Sim800c_Task, osPriorityNormal, 0, 128);	//开启重启Sim800C初始化调度函数
		Start_Reset_Sim800c_Task_TaskHandle = osThreadCreate(osThread(Reset_Sim800c_Task), NULL);
		taskEXIT_CRITICAL();				//退出临界区
	}
	Pack_Len = init_OneNet_Pack();
	Send_To_Uart2_Str((int8_t*)g_send_Onenet_buf,Pack_Len);
	//需要等待返回数据返回一个SEND OK
	err = xTaskNotifyWait((uint32_t)0x00,
			(uint32_t)0xffffffff,
			(uint32_t*)&NotifyValue,
			(TickType_t)10000);
	cmp = strcmp((const char*)Send_data_recv,"\r\nSEND OK\r\n");//对比是不是这个数据
	osDelay(1000);
	printf("Connect_OneNet ok\r\n");
	Reset_Uart_DMA();
	if(err == pdFALSE){
		return ERROR;
	}else if((err == pdTRUE)&&(NotifyValue == RCV_SEND_OK)&&(cmp == 0)){
		return SUCCESS;
	}
}

/*
 * 返回当前信号值
 */
uint8_t Get_Sim800C_Signal(void)
{
	char* str;
	int n;
	GPRS_AT_CSQ(AT_CSQ);
	osDelay(500);
//	str = strstr((const char*)pData,"+CSQ:");
//	str = strtok(str,",");
//	n = atoi(str+5);
	//printf("str+5 = [%s],n = %d\r\n",str+5,n);
	return n;
}
