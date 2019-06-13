/*
 * board_info.h
 *
 *  Created on: 2019年4月27日
 *      Author: Administrator
 */

#ifndef HARDWARE_BOARD_INFO_BOARD_INFO_H_
#define HARDWARE_BOARD_INFO_BOARD_INFO_H_

#include "main.h"
#include "cJSON.h"
#include "Mqtt.h"

#define STRATEGY_ADDR 				0x10	//这个是乘数	i*0x10 一共8租数据0-7
#define HUMI_COMP					0x80
#define MAX_ROUTE					0x82
#define NOW_ROUTE					0x84
#define STORKING_CYCLE				0x87
#define DELAY_INSPECTION			0x8A
#define VERSION_INFORMATION_ADDR	0x90					//从90开始存储设备信息

//typedef struct{
//	uint8_t	Pack_type; 		// MQTT类型=3  DupFlag=0 QosLevel=0 Retain=0
//	uint8_t RemainedLength[4];	//remainedLength=包长度，注意【不计算这一位和前一位】
//	uint16_t  TopicNameLen;	//主题名长度
//	uint8_t TopicName[10];	 //TopicName=$dp
//	uint8_t DpType;			// DpType=3 TimeSet=0
//	uint16_t JsonStrLen;  	 // JsonStrLen
//	cJSON* p_Version_Information;
//}Version_Information_Pack;

int8_t Check_Board_Info(void);
int8_t Check_Master_Control(uint8_t* Master_Control);
int8_t Check_Soft_Versions(uint8_t* p_Soft_Versions);
int8_t Check_Device_Type(uint8_t* p_Device_Type);
int8_t Check_Hardware_Versions(uint8_t* p_Soft_Versions);
int8_t Check_Device_ID(uint8_t* p_Device_ID);
int8_t Check_Str_Number(uint8_t* p_Str_Number,uint8_t type);
int8_t Check_Serial_Number(uint8_t* p_str_Serial_Number);
int8_t Check_Date_Of_Manufacture(uint8_t* p_str_Date_Of_Manufacture);
uint8_t Send_Version_Information(void);
uint16_t init_OneNet_Version_Information_Pack(void);
#endif /* HARDWARE_BOARD_INFO_BOARD_INFO_H_ */
