/*
 * Aliot.c
 *
 *  Created on: 2019Äê7ÔÂ13ÈÕ
 *      Author: Administrator
 */
#include "ALIOT.h"

void Aliot_Make_Dat(Aliot_TypeDef *	p, uint32_t time)
{
	char a[16] = "\0";
	memset (p, 0 ,sizeof(Aliot_TypeDef));

	strcpy(p->IOT_ProductKey,	ProductKey);

	strcpy(p->IOT_DeviceName,	p_GPRS->IMEI);

	strcpy(p->IOT_DeviceID,		ProductKey);
	strcat(p->IOT_DeviceID,		".");
	strcat(p->IOT_DeviceID,		p_GPRS->IMEI);

	//strcpy(p->IOT_DeviceSecret, DeviceSecret);
	strcpy(p->IOT_DeviceSecret, p_GPRS->DS);

	strcpy(p->IOT_Host,				ProductKey);
	strcat(p->IOT_Host,				".");
	strcat(p->IOT_Host,				Host);

	strcpy(p->IOT_Port,				Port);

	strcpy(p->IOT_ClientID,		p->IOT_DeviceID);
	strcat(p->IOT_ClientID,		"|");
	strcat(p->IOT_ClientID,		SecureMode);
	strcat(p->IOT_ClientID,		",");
	strcat(p->IOT_ClientID,		Timestamp);
	strcat(p->IOT_ClientID,		"=");
	strcat(p->IOT_ClientID,		itoa(a,time,10));
	//strcat(p->IOT_ClientID,		"2524608000000");
	strcat(p->IOT_ClientID,		",");
	strcat(p->IOT_ClientID,		SignMethod);
	strcat(p->IOT_ClientID,		",");
	strcat(p->IOT_ClientID,		Gw);
	strcat(p->IOT_ClientID,		"|");

	strcpy(p->IOT_UserName,		p_GPRS->IMEI);
	strcat(p->IOT_UserName,		"&");
	strcat(p->IOT_UserName,		ProductKey);

	sprintf(p->IOT_Content,	"clientId%s" "deviceName%s" "productKey%s" "timestamp%s",p->IOT_DeviceID, p->IOT_DeviceName,p->IOT_ProductKey,a);

	utils_hmac_sha1(p->IOT_Content, p->IOT_DeviceSecret, p->IOT_Guider_Sign);

}




