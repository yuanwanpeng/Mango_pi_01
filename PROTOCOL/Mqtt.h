/*
 * Mqtt.h
 *
 *  Created on: 2019年3月29日
 *      Author: Administrator
 */

#ifndef INC_MQTT_H_
#define INC_MQTT_H_
#include "stdint.h"
#include "cJSON.h"
typedef struct{
	uint8_t	Pack_type;
	uint8_t RemainedLength;
	uint16_t ProtocolNameLength;
	uint8_t ProtocolName[10];
	uint8_t ProtocolLevel;
	uint8_t Pack_Flag;
	uint16_t	KeepAlive;
	uint16_t 	ClientIdentifierLen;
	uint8_t 	ClientIdentifier[20];//=518720857
	uint16_t 	UserNameLen;
	uint8_t		UserName[10];		  // userName=217490
	uint16_t	UserPasswordLen;
	uint8_t		UserPassword[64];		//api-key
}Connect_Pack;

//typedef struct{
//	uint8_t	Pack_type; 		// MQTT类型=3  DupFlag=0 QosLevel=0 Retain=0
//	uint8_t RemainedLength;	//remainedLength=包长度，注意【不计算这一位和前一位】
//	uint16_t  TopicNameLen;	//主题名长度
//	uint8_t TopicName[10];	 //TopicName=$dp
//	uint8_t DpType;			// DpType=3 TimeSet=0
//	uint16_t JsonStrLen;  	 // JsonStrLen=21
//	uint8_t JsonStr[64]; // JsonStr={"Temp":10,"Humi":95}
////	cJSON* p_Temp_Humi_Pack;
//}Temp_Humi_Pack;

//typedef struct{
//	uint8_t	Pack_type; 		// MQTT类型=3  DupFlag=0 QosLevel=0 Retain=0
//	uint8_t RemainedLength[4];	//remainedLength=包长度，注意【不计算这一位和前一位】
//	uint16_t  TopicNameLen;	//主题名长度
//	uint8_t TopicName[10];	 //TopicName=$dp
//	uint8_t DpType;			// DpType=3 TimeSet=0
//	uint16_t JsonStrLen;  	 // JsonStrLen
//	cJSON* p_Version_Information;
//}Version_Information_Pack;

typedef struct{
	uint8_t	Pack_type; 		// MQTT类型=3  DupFlag=0 QosLevel=0 Retain=0
	uint8_t RemainedLength[4];	//remainedLength=包长度，注意【不计算这一位和前一位】
	uint16_t  TopicNameLen;	//主题名长度
	uint8_t TopicName[10];	 //TopicName=$dp
	uint8_t DpType;			// DpType=3 TimeSet=0
	uint16_t JsonStrLen;  	 // JsonStrLen
	cJSON*  JsonStr;
}Onenet_Pack;
extern float g_rh;
extern uint8_t pData[];
extern float temp;
extern Onenet_Pack G_Send_Pack;
uint16_t Connect_Server(void);

uint8_t Get_Sim800C_Signal(void);
uint16_t Create_Send_Pack(uint8_t* str);
#endif /* INC_MQTT_H_ */
