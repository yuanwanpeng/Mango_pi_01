/*
 * GPRS.h
 *
 *  Created on: 2018Äê11ÔÂ28ÈÕ
 *      Author: YuanWP
 */

#ifndef INC_GPRS_H_
#define INC_GPRS_H_
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include <string.h>
#include "SIM800C.h"
#define AT 				"AT\r\n"
#define AT_CGDCONT 		"AT+CGDCONT=1,\"IP\",\"CMLG\"\r\n"
#define AT_CGATT 		"AT+CGATT=1\r\n"
#define AT_CIPCSGP 		"AT+CIPCSGP=1,\"CMLG\"\r\n"
#define AT_CIPSTATUS 	"AT+CIPSTATUS\r\n"
#define AT_CIPSTART		"AT+CIPSTART=\"TCP\",\"183.230.40.39\",\"6002\"\r\n"
#define AT_CSQ			"AT+CSQ\r\n"
#define AT_CIPSEND		"AT+CIPSEND\r\n"
#define ATE0			"ATE0\r\n"

extern char Send_data_recv[];

uint8_t GPRS_AT(uint8_t* cmd,uint8_t* check);
uint8_t GPRS_AT_CGDCONT(uint8_t* cmd,uint8_t* check);
uint8_t GPRS_AT_CGATT(uint8_t* cmd,uint8_t* check);
uint8_t GPRS_AT_CIPCSGP(uint8_t* cmd,uint8_t* check);
uint8_t GPRS_AT_CIPSTATUS(uint8_t* cmd,uint8_t* check);
uint8_t GPRS_AT_CIPSTART(uint8_t* cmd,uint8_t* check);
uint8_t GPRS_AT_CSQ(uint8_t* cmd);
uint8_t GPRS_AT_CIPSEND(uint8_t* cmd,uint8_t* check);
uint8_t GPRS_ATE0(uint8_t* cmd);
#endif
