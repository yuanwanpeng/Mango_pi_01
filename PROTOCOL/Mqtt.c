/*
 * Mqtt.c
 *
 *  Created on: 2019年3月29日
 *      Author: Administrator
 */
#include "Mqtt.h"
#include "MQTTPacket.h"
#include "main.h"
#include "GPRS.h"
#include "SIM800C_Scheduler_data.h"
#include "transport.h"
#define DEVICE_ID 	"518898092"
#define PRODUCT_ID 	"217490"
#define API_KEY		"eC8obJLJaS2PEA0SSrF3gcGbDQ8="

extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
Connect_Pack G_Connect_Pack;
MQTT_Send_Pack	G_Send_Pack;   //Onenet_Pack G_Send_Pack;
uint8_t g_send_Server_buf[PDATA_SIZE];
int		MsgID = 1;
extern float temp;
extern osThreadId Start_Reset_Sim800c_Task_TaskHandle;
extern Version_Information_Struct G_Version_Information;
extern GPRS_TypeDef *p_GPRS,GPRS;



uint16_t MQTT_Connect(void)
{
	int len = 0;
	MQTTPacket_connectData ConnectData = MQTTPacket_connectData_initializer;

	ConnectData.clientID.cstring = "Mango_pi_00001";
	ConnectData.keepAliveInterval = 360;
	ConnectData.cleansession = 1;
	ConnectData.username.cstring = NULL;
	ConnectData.password.cstring = NULL;

	len = MQTTSerialize_connect(g_send_Server_buf, PDATA_SIZE, &ConnectData);

	if (transport_sendPacketBuffer(g_send_Server_buf,len,ACK) == SUCCESS)
	{
		p_GPRS->DOWN_LEN = 0x0000;
		Get_MQTT_Dat_From_Uart(p_GPRS);
		
		if (MQTTPacket_read(g_send_Server_buf, PDATA_SIZE, transport_getdata) == CONNACK){
			unsigned char sessionPresent, connack_rc;
			if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, g_send_Server_buf, PDATA_SIZE) != 1 || connack_rc != 0)
				return ERROR;
			else return SUCCESS;
		}else return ERROR;
	}else return ERROR;
}
/*
	
*/
uint8_t MQTT_Ping(void)
{
	int len = 0;
		
	len = MQTTSerialize_pingreq(g_send_Server_buf, PDATA_SIZE);
	
	if (transport_sendPacketBuffer(g_send_Server_buf,len,ACK))
	{
		p_GPRS->DOWN_LEN = 0x0000;
		Get_MQTT_Dat_From_Uart(p_GPRS);
		if (MQTTPacket_read(g_send_Server_buf, PDATA_SIZE, transport_getdata) == PINGRESP)
			return SUCCESS;
		else return ERROR;
	}
	else return ERROR;
}


uint8_t MQTT_Subtopic(char * topic_path)
{
	MQTTString topicString = MQTTString_initializer;
	int dup = 0;
	int req_qos = 1;
	int len = 0;
	char	topic[64]="\0";
	
	strcpy(topic,TOPIC_PRODUCTKEY);
	//strcat(topic,p_GPRS->IMEI);
	strcat(topic,topic_path);
	
	//topicString.cstring = (char *)&TOPIC_GET;
	topicString.cstring = (char *)&topic;
			
	len = MQTTSerialize_subscribe(g_send_Server_buf, PDATA_SIZE, dup, MsgID++, 1, &topicString, &req_qos);
	//printf("len = %d\r\n",len);
	if (transport_sendPacketBuffer(g_send_Server_buf,len,ACK))
	{	
		p_GPRS->DOWN_LEN = 0x0000;
		Get_MQTT_Dat_From_Uart(p_GPRS);
		if (MQTTPacket_read(g_send_Server_buf, PDATA_SIZE, transport_getdata) == SUBACK) 	/* wait for suback */
		{
			unsigned short submsgid;
			int subcount;
			int granted_qos;
			
			MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, g_send_Server_buf, PDATA_SIZE);//90 03 00 01 80
			if ((granted_qos == 0) || (granted_qos == 0x80) )
				return ERROR;
			else if (granted_qos != 0)
				return SUCCESS;
			else return ERROR;
		}
		else return ERROR;
	}
	else return ERROR;
}

/*
 * 连接server
 */
uint16_t Connect_Server(void)
{
	uint8_t flag = 0;

	flag += MQTT_Connect();
	flag += MQTT_Ping();
	flag += MQTT_Subtopic(TOPIC_ReceiveCmd);
	printf("flag:3ok flag = %d\r\n",flag);
	
}


uint8_t MQTT_Pubtopic(char * topic_path, char * payload)
{
	MQTTString topicString = MQTTString_initializer;
	int payloadlen = strlen((const char *)payload);
	int dup = 0;
	int qos = 1;
	int len = 0;
	char	topic[64]="\0";
//	printf("payload = %s\r\n",payload);
//	printf("payloadlen = %d\r\n",payloadlen);
	strcpy(topic,TOPIC_PRODUCTKEY);
//	strcat(topic,p_GPRS->IMEI);
	strcat(topic,topic_path);
	
	topicString.cstring = (char *)&topic;
	
	len = MQTTSerialize_publish(g_send_Server_buf, PDATA_SIZE, dup, qos, 0, MsgID++, topicString, payload, payloadlen);
	//myfree(payload);
	
	if (transport_sendPacketBuffer(g_send_Server_buf,len,ACK))
	{
		p_GPRS->DOWN_LEN = 0x0000;
		if (MQTTPacket_read(g_send_Server_buf, PDATA_SIZE, transport_getdata) == PUBACK) 	/* wait for puback */
		{
			Reset_Uart_DMA(p_GPRS);
			return SUCCESS;
		}
		else return ERROR;
	}
	else return ERROR;
}


/*
 * 返回当前信号值
 */
uint8_t Get_Sim800C_Signal(void)
{
	char* str;
	int n;
//	GPRS_AT_CSQ(AT_CSQ);
//	osDelay(500);
//	str = strstr((const char*)pData,"+CSQ:");
//	str = strtok(str,",");
//	n = atoi(str+5);
	//printf("str+5 = [%s],n = %d\r\n",str+5,n);
	return n;
}
