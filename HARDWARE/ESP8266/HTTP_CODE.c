/*
 * HTTP.c
 *
 *  Created on: 2018年11月19日
 *      Author: YuanWP
 */
#include "HTTP_CODE.h"
/**
  * @brief   组HTTP POST报文
  * @param   pkt   报文缓存指针
  * @param   key   API_KEY定义在Main.c文件中，需要根据自己的设备修改
  *	@param 	 devid 设备ID，定义在main.c文件中，需要根据自己的设备修改
  *	@param 	 dsid  数据流ID
  *	@param 	 val   字符串形式的数据点的值
  * @retval  整个包的长度
  */
uint32_t HTTP_PostPkt(int8_t *pkt, int8_t *key, int8_t *devid, int8_t *dsid, int8_t *val)
{
    int8_t dataBuf[100] = {0};
    int8_t lenBuf[10] = {0};
    *pkt = 0;

    sprintf((char*)dataBuf, ",;%s,%s", dsid, val);     //采用分割字符串格式:type = 5
    sprintf((char*)lenBuf, "%d", strlen((const char*)dataBuf));

    strcat((char*)pkt, "POST /devices/");
    strcat((char*)pkt, (const char*)devid);
    strcat((char*)pkt, "/datapoints?type=5 HTTP/1.1\r\n");

    strcat((char*)pkt, "api-key:");
    strcat((char*)pkt, (const char*)key);
    strcat((char*)pkt, "\r\n");

    strcat((char*)pkt, "Host:api.heclouds.com\r\n");

    strcat((char*)pkt, "Content-Length:");
    strcat((char*)pkt, (const char*)lenBuf);
    strcat((char*)pkt, "\r\n\r\n");

    strcat((char*)pkt, (const char*)dataBuf);

    return strlen((char*)pkt);
}
