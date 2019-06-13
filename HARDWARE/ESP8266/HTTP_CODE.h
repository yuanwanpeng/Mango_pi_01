/*
 * HTTP.h
 *
 *  Created on: 2018Äê11ÔÂ19ÈÕ
 *      Author: YuanWP
 */

#ifndef HARDWARE_ESP8266_HTTP_CODE_H_
#define HARDWARE_ESP8266_HTTP_CODE_H_
#include "HUMI.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "LCD12864.h"
#include <math.h>
#include <string.h>
uint32_t HTTP_PostPkt(int8_t *pkt, int8_t *key, int8_t *devid, int8_t *dsid, int8_t *val);

#endif /* HARDWARE_ESP8266_HTTP_CODE_H_ */
