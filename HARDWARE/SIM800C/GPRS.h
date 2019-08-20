/*
 * GPRS.h
 *
 *  Created on: 2018年11月28日
 *      Author: YuanWP
 */

#ifndef HARDWARE_GPRS_H_
#define HARDWARE_GPRS_H_
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include <string.h>
#include "uart.h"
#include "SIM800C.h"

#define PDATA_SIZE 640


/********my prh.h*********/
#define	TOPIC_PRODUCTKEY			"Mangopiekey"
#define TOPIC_ReceiveCmd			"/ReceiveCmd"			//获取控制指令


#define TOPIC_SendData			"/SendData"			//获取控制指令



#define	Port					"1883"


/********my prh.h  end*********/
#define	GPRS_MAX_Delay		0x1200//max120s
#define Start_Delay			0x1000
#define DAT_VALID					0x8000
#define	ALI_OK						0x4000
#define	ORDER_VALID				0x0400
#define	GPS_INVALID				0x0800
#define	GPS_VALID					0x1000
#define	MOBIL_OK					0x2000



typedef struct
{
//	uint32_t	Addr;
	char			Buf[2][PDATA_SIZE];
	volatile	uint8_t		RX_Dispose;//处理数据
	volatile	uint8_t		Bufuse;//接收到哪个buff的变量
	volatile	uint8_t		RX_Flag;
	volatile 	uint16_t	Len[2];//两个缓冲区的长度
	volatile	uint32_t	RX_Time;
	char			Clear;
}BUF_TypeDef;


typedef struct
{
	BUF_TypeDef		*GPRS_BUF;

	volatile uint8_t		CSQ;
	volatile char		CIPSTATUS;
	char			CSQ_CH[8];
	uint8_t		CREG;
	char			IMEI[16];
	char			DS[64];
	char			PNUM[32];
	
	volatile uint8_t		AT;
	volatile uint8_t		ATE;
	volatile uint8_t 		CIPSRIP;
	volatile uint8_t 		CIPCLOSE;
	volatile uint8_t 		ATCSQ;
	volatile uint8_t 		ATCIPSTATUS;
	volatile uint8_t 		ATCIPSEND;
	volatile uint8_t 		SEND_OK;
	volatile uint8_t 		ATCSMINS;
	uint8_t 	CFUN;
	uint8_t		CPIN;
	uint8_t		CALL;
	uint8_t		SMS;
	uint8_t		SENDING;
	uint16_t	SENDING_TIME;

	char	*		P_CMD;
	char	*		P_CMD_CHECK;
	char			SEND_LEN[8];
	char			RECV_LEN[8];

	uint16_t	DOWN_LEN;
	char			DOWN_BUF[PDATA_SIZE];
}GPRS_TypeDef;



typedef struct
{
	uint16_t	ping_timer;	
}Timer_TypeDef;

void Reset_Uart_DMA(GPRS_TypeDef *_GPRS);
uint8_t GPRS_Init(GPRS_TypeDef		*_GPRS);
uint8_t GPRS_AT(GPRS_TypeDef	* _GPRS);
uint8_t GPRS_CIPSHUT(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_CGDCONT(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_CIPCLOSE_OFF(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_CIPSTART_ON(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_RECONNECT(GPRS_TypeDef		* _GPRS, char * host);
uint8_t GPRS_CIPSTATUS(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_CGSN(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_CNUM(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_CGSN(GPRS_TypeDef		* _GPRS);	
uint8_t GPRS_CMD(GPRS_TypeDef		* _GPRS, char * cmd, char *check,uint8_t delay_time);
uint8_t GPRS_DAT(GPRS_TypeDef  * _GPRS,char * cmd,unsigned char* buf, int buflen);
uint8_t GPRS_CSQ(GPRS_TypeDef		* _GPRS);
uint8_t GPRS_ATE0(GPRS_TypeDef		*_GPRS);
uint16_t GPRS_CIPSEND(GPRS_TypeDef  * _GPRS,unsigned char* buf, int buflen);
uint8_t GPRS_Network(GPRS_TypeDef		* _GPRS);
uint8_t Get_MQTT_Dat_From_Uart(GPRS_TypeDef  * _GPRS);
uint8_t GPRS_CSMINS(GPRS_TypeDef		*_GPRS);
uint8_t RECV_CMD_ZERO_TIME(GPRS_TypeDef		* _GPRS,char * cmd);
#endif
