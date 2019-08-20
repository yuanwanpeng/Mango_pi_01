#ifndef __ALIOT_H__
#define __ALIOT_H__
#include "GPRS.h"
#include <string.h>
#include "stdint.h"
//#include "utils_sha1.h"

#define	Host					"iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define	Port					"1883"
#define	SignMethod		"signmethod=hmacsha1"
#define	SecureMode		"securemode=3"
#define	Timestamp			"timestamp"
#define	Gw						"gw=0"
extern	GPRS_TypeDef						*p_GPRS;

typedef struct 
{
	uint32_t total[2];          /*!< number of bytes processed  */
	uint32_t state[5];          /*!< intermediate digest state  */
	unsigned char buffer[64];   /*!< data block being processed */
} iot_sha1_context;
typedef struct
{
	char IOT_ProductKey[32];
	char IOT_DeviceName[32];
	char IOT_DeviceID[64];
	char IOT_DeviceSecret[64];
	
	char IOT_Host[64];
	char IOT_Port[8];
	
	char IOT_Content[128];
	
	char IOT_ClientID[128];
	char IOT_UserName[64];
	char IOT_Guider_Sign[64];
}Aliot_TypeDef;



void Aliot_Make_Dat(Aliot_TypeDef *	p, uint32_t time);

#endif
