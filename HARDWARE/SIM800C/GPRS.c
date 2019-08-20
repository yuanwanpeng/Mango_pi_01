/*
 * GPRS.h
 *
 *  Created on: 2018骞�11鏈�28鏃�
 *      Author: YuanWP
 */

#include "GPRS.h"
#include "uart.h"
#include "SIM800C_Scheduler_data.h"
extern osThreadId Start_Scheduler_data_Task_TaskHandle;
char * GPRS_Rx_Dat;
extern	GPRS_TypeDef						*p_GPRS;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern TIM_HandleTypeDef htim2;
Timer_TypeDef							*p_Tap_Timer,Tap_Timer;
extern SemaphoreHandle_t recv_gprs_cmd_xSemaphore;//二值信号量
extern SemaphoreHandle_t send_gprs_dat_xSemaphore;//二值信号量

void Reset_Uart_DMA(GPRS_TypeDef *_GPRS)
{
	HAL_UART_DMAStop(&huart2);
	p_GPRS->GPRS_BUF->RX_Time = 0;
	p_GPRS->GPRS_BUF->RX_Flag = RESET;
	_GPRS->GPRS_BUF->RX_Dispose = 0;//非法数据进行回收处理
	_GPRS->GPRS_BUF->Bufuse = 0;
	memset(_GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->Bufuse],'\0',PDATA_SIZE);
	HAL_UART_Receive_DMA(&huart2, (uint8_t*)_GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->Bufuse], PDATA_SIZE);
}

uint8_t GPRS_AT(GPRS_TypeDef	* _GPRS)
{
	return GPRS_CMD(_GPRS,"AT\r\n","OK\r\n",0);//等待0秒
}

uint8_t RECV_CMD_ZERO_TIME_CLEAR(GPRS_TypeDef		* _GPRS,char* cmd,uint8_t* cnt,uint16_t time)
{
	(*cnt)++;
	osDelay(100);
	if((*cnt) == time)
	{
		(*cnt) = 0;
		printf("[%s] Field cmd\r\n",cmd);
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		return ERROR;
	}
	return SUCCESS;
}
uint8_t RECV_CMD_ZERO_TIME(GPRS_TypeDef		* _GPRS,char * cmd)
{
	uint8_t cnt = 0;

	if(strstr(cmd,"AT+CIPSEND="))
	{
		while(_GPRS->ATCIPSEND == 0)
		{
			cnt++;
			osDelay(100);
			if(cnt == 30)
			{
				cnt = 0;
				printf("[%s] Field cmd\r\n",cmd);
				xSemaphoreGive(Sim800c_Semaphore);//'>'不可以解锁二值信号量
				_GPRS->P_CMD = NULL;
				_GPRS->P_CMD_CHECK = NULL;
				return ERROR;
			}
		}
		Clear_Recv_Data();	//清空处理完毕的数据
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
	//	xSemaphoreGive(Sim800c_Semaphore);//'>'不可以解锁二值信号量
		return SUCCESS;
	}
	if(strstr(cmd,"AT+CSMINS?"))//检测是否插入simcard
	{
		while(_GPRS->ATCSMINS == 0)
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,3) == ERROR)
			{
				printf("error\r\n");
				return ERROR;//如果等于ERROR的话表示超时了
			}
		}
		printf("cl");
		Clear_Recv_Data();	//清空处理完毕的数据
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}
	
	if(strstr(cmd,"AT+CIPSTATUS"))
	{
		while(_GPRS->ATCIPSTATUS == 0)
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,3) == ERROR)
				return ERROR;//如果等于ERROR的话表示超时了
		}
		Clear_Recv_Data();	//清空处理完毕的数据
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}
	if(strstr(cmd,"AT\r\n"))
	{
		while(_GPRS->AT == 0)
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,3) == ERROR)
				return ERROR;//如果等于ERROR的话表示超时了
		}
		Clear_Recv_Data();	//清空处理完毕的数据
		printf("p_GPRS->DOWN_BUF AT = %s\r\n",p_GPRS->DOWN_BUF);
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}
	if(strstr(cmd,"ATE0\r\n"))
	{
		while(_GPRS->ATE == 0)
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,3) == ERROR)
				return ERROR;//如果等于ERROR的话表示超时了
		}
		Clear_Recv_Data();	//清空处理完毕的数据
		printf("p_GPRS->DOWN_BUF ATE0 = %s\r\n",p_GPRS->DOWN_BUF);
		printf("GPRS_Rx_Dat ATE0 = %s\r\n",GPRS_Rx_Dat);
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}
	if(strstr(cmd,"AT+CIPSRIP"))
	{
		while(_GPRS->CIPSRIP == 0)
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,3) == ERROR)
				return ERROR;//如果等于ERROR的话表示超时了
		}
		Clear_Recv_Data();	//清空处理完毕的数据
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}
	if(strstr(cmd,"AT+CIPCLOSE"))
	{
		while(_GPRS->CIPCLOSE == 0)
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,3) == ERROR)
				return ERROR;//如果等于ERROR的话表示超时了
		}
		Clear_Recv_Data();	//清空处理完毕的数据
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}//AT+CIPSTART=
	if(strstr(cmd,"AT+CIPSTART="))
	{
		while(_GPRS->CIPSTATUS != 8)//表示connect OK
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,100) == ERROR)
				return ERROR;//如果等于ERROR的话表示超时了
		}
		Clear_Recv_Data();	//清空处理完毕的数据
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}
	if(strstr(cmd,"AT+CSQ"))
	{
		while(_GPRS->ATCSQ == 0)//表示connect OK
		{
			if(RECV_CMD_ZERO_TIME_CLEAR(_GPRS,cmd,&cnt,3) == ERROR)
				return ERROR;//如果等于ERROR的话表示超时了
		}
		
		Clear_Recv_Data();	//清空处理完毕的数据
		printf("CSQ SUCCESS\r\n");
		printf("p_GPRS->DOWN_BUF = %s\r\n",p_GPRS->DOWN_BUF);
		_GPRS->P_CMD = NULL;
		_GPRS->P_CMD_CHECK = NULL;
		xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
		return SUCCESS;
	}
	
}
uint8_t GPRS_CMD(GPRS_TypeDef		* _GPRS, char * cmd, char *check,uint8_t delay_time)
{
	xSemaphoreTake(Sim800c_Semaphore,portMAX_DELAY);//上锁二值信号量
	
	_GPRS->P_CMD = cmd;
	_GPRS->P_CMD_CHECK = check;
	
	Send_To_Uart2_Str((int8_t*)cmd,strlen(cmd));		//发送命令	

	//等待接收信号量
	if(delay_time == 0)//如果等于0 的话表示此命令是不发送信号量到这个任务的由变量直接操作
	{
		return RECV_CMD_ZERO_TIME(_GPRS,cmd);//检测0超时的数据
	}else{
		if( xSemaphoreTake( recv_gprs_cmd_xSemaphore, 1000*delay_time ) == pdTRUE )//等待delay_time秒
		{
		//	printf("strstr[%s]",p_GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->RX_Dispose]);
			if(strstr(p_GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->RX_Dispose],check))//检查是否等于check
			{
				Clear_Recv_Data();	//清空处理完毕的数据
				xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
				_GPRS->P_CMD = NULL;
				_GPRS->P_CMD_CHECK = NULL;
				return SUCCESS;
			}else{
				printf("cmd = [%s],check error\r\n",_GPRS->P_CMD);
				xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
				_GPRS->P_CMD = NULL;
				_GPRS->P_CMD_CHECK = NULL;
				return ERROR;
			}
		}else{
			printf("cmd = [%s],xSemaphoreTake = false\r\n",_GPRS->P_CMD);
			xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
			_GPRS->P_CMD = NULL;
			_GPRS->P_CMD_CHECK = NULL;
			return ERROR;
		}
	}
}


//链接GPRS服务器
uint8_t GPRS_CIPSTART_ON(GPRS_TypeDef		* _GPRS)//MAX 90S
{
	if(GPRS_CMD(_GPRS,"AT+CIPSTART=\"TCP\",\"182.92.0.28\",\"1883\"\r\n","CONNECT OK\r\n",0) == SUCCESS)
	{
		return SUCCESS;
	}else{
		return ERROR;
	}
}

//检查TCP状态
uint8_t GPRS_CIPSTATUS(GPRS_TypeDef		* _GPRS)
{
	_GPRS->ATCIPSTATUS = 0;
	return GPRS_CMD(_GPRS,"AT+CIPSTATUS\r\n","STATE:",0);
}

//关闭GPRS链接
uint8_t GPRS_CIPCLOSE_OFF(GPRS_TypeDef		* _GPRS)//MAX 90S
{
	GPRS_CMD(_GPRS,"AT+CIPCLOSE\r\n","ERROR\r\n",0);
	return (GPRS_CMD(_GPRS,"AT+CIPCLOSE\r\n","ERROR\r\n",0));
}

//关闭场景
uint8_t GPRS_CIPSHUT(GPRS_TypeDef		* _GPRS)	//关闭场景
{
	return (GPRS_CMD(_GPRS,"AT+CIPSHUT\r\n","SHUT OK",5));
}

uint8_t GPRS_CGDCONT(GPRS_TypeDef		* _GPRS)	//
{
	return GPRS_CMD(_GPRS,"AT+CGDCONT=1,\"IP\",\"CMLG\"\r\n","OK",10);//CMNET    CMLG
}

uint8_t GPRS_RECONNECT(GPRS_TypeDef		* _GPRS, char * host)
{
	uint8_t flag=ERROR,count = 0;
	
	osDelay(1000);
//	GPRS_CSQ(_GPRS);
//	osDelay(5000);
//	GPRS_CSMINS(_GPRS);		//检测simcard SIM Inserted 
//	if (((_GPRS->CSQ > 10) && (_GPRS->CSQ < 32))&&1)
//	{
//		if(count == 0)
//		{
//			//GPRS_CGDCONT(_GPRS);
//			flag = GPRS_CIPSTART_ON(_GPRS);
//			count++;
//		}
//		GPRS_CIPSTATUS(p_GPRS);	//获取当前TCP连接状态
//		if((p_GPRS->CIPSTATUS == 6)||(flag == SUCCESS))
//		{
//			return SUCCESS;
//		}else if(p_GPRS->CIPSTATUS == 8){//CLOSED
//			count = 0;//重新连接
//		}else if(p_GPRS->CIPSTATUS == 9){//PDP 
//			GPRS_CIPCLOSE_OFF(_GPRS);
//		//	GPRS_CGDCONT(_GPRS);
//			GPRS_CIPSHUT(_GPRS);
//			count = 0;
//		}
//		printf("flag = %d\r\n",flag);
//	}
	
	return flag;
}

//获取信号强度 +CSQ: 16, 99
uint8_t GPRS_CSQ(GPRS_TypeDef		* _GPRS)
{
	char a[3];
	char *num=a;
	_GPRS->ATCSQ = 0;		//归零操作
	if(GPRS_CMD(_GPRS,"AT+CSQ\r\n","OK\r\n",0) == SUCCESS)
	{
		GPRS_Rx_Dat = p_GPRS->DOWN_BUF;
		printf("GPRS_Rx_Dat CSQ = [%s]\r\n",GPRS_Rx_Dat);
		if (strstr(GPRS_Rx_Dat,"+CSQ:"))//
		{
			while(*GPRS_Rx_Dat != ' ')
			{
				_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose]--;
				if(_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose]==0)
					return ERROR;
				GPRS_Rx_Dat++;
			}
			GPRS_Rx_Dat++;
			while(*GPRS_Rx_Dat != ',')
			{
				_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose]--;
				if(_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose] == 0)
					return ERROR;
				*num++ = *GPRS_Rx_Dat++;
			}
			*num++ = 0;
			_GPRS->CSQ_CH[0] = a[0];
			_GPRS->CSQ_CH[1] = a[1];
			_GPRS->CSQ_CH[2] = a[2];
			
			_GPRS->CSQ = atoi(a); 
//			Clear_Recv_Data();
			return SUCCESS;
		}
		else {
//			Clear_Recv_Data();
			return ERROR;
		}
	}
	else {
//		Clear_Recv_Data();
		return ERROR;
	}
}


uint8_t GPRS_Network(GPRS_TypeDef		* _GPRS)
{
	GPRS_CSQ(_GPRS);
	if ((_GPRS->CSQ >= 10) && (_GPRS->CSQ <= 31))
		return SUCCESS;
	else return ERROR;
}

uint8_t GPRS_CIPSRIP(GPRS_TypeDef		*_GPRS)
{
	return GPRS_CMD(_GPRS,"AT+CIPSRIP=1\r\n","OK\r\n",0);
}
/*
	检测是否插入了sim卡
*/
uint8_t GPRS_CSMINS(GPRS_TypeDef		*_GPRS)
{
	char a[3];
	char *num=a;
	if(GPRS_CMD(_GPRS,"AT+CSMINS?\r\n","+CSMINS:",0) == SUCCESS)
	{
		GPRS_Rx_Dat = p_GPRS->DOWN_BUF;
		printf("GPRS_Rx_Dat CSMINS = [%s]\r\n",GPRS_Rx_Dat);
		return ERROR;
//		if (strstr(GPRS_Rx_Dat,"+CSMINS:"))//
//		{
//			while(*GPRS_Rx_Dat != ' ')
//			{
//				_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose]--;
//				if(_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose]==0)
//					return ERROR;
//				GPRS_Rx_Dat++;
//			}
//			GPRS_Rx_Dat++;
//			while(*GPRS_Rx_Dat != ',')
//			{
//				_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose]--;
//				if(_GPRS->GPRS_BUF->Len[p_GPRS->GPRS_BUF->RX_Dispose] == 0)
//					return ERROR;
//				*num++ = *GPRS_Rx_Dat++;
//			}
//			*num++ = 0;
//			_GPRS->CSQ_CH[0] = a[0];
//			_GPRS->CSQ_CH[1] = a[1];
//			_GPRS->CSQ_CH[2] = a[2];
//			
//			_GPRS->CSQ = atoi(a); 
//			return SUCCESS;
//		}
	}
//	return GPRS_CMD(_GPRS,"AT+CSMINS?\r\n","+CSMINS:",0);
}
uint8_t GPRS_Init(GPRS_TypeDef		*_GPRS)
{
	uint8_t cont = 0;
	reset_Sim800C();
	
	while(GPRS_AT(_GPRS) != SUCCESS)	//发送AT
	{
		cont++;
		if(cont == 3)
		{
			cont = 0;
			reset_Sim800C();
		}
		osDelay(100);
	}
	while(GPRS_ATE0(_GPRS) != SUCCESS);
//	while(GPRS_CIPSRIP(_GPRS) != SUCCESS);
	return SUCCESS;
}

uint8_t GPRS_ATE0(GPRS_TypeDef		*_GPRS)
{
	return GPRS_CMD(_GPRS,"ATE0\r\n","OK\r\n",0);
}
uint8_t GPRS_DAT(GPRS_TypeDef  * _GPRS,char * cmd,unsigned char* buf, int buflen)
{
	uint8_t cnt = 0;
	xSemaphoreTake(Sim800c_Semaphore,portMAX_DELAY);//上锁二值信号量UART2不允许其他数据发出
	_GPRS->ATCIPSEND = 0;
	_GPRS->P_CMD = cmd;
	_GPRS->P_CMD_CHECK = ">";
	_GPRS->ATCIPSEND = 0;
//	printf("%s\r\n",cmd);
	Send_To_Uart2_Str((int8_t*)cmd,strlen(cmd));
	
	
	if( RECV_CMD_ZERO_TIME(_GPRS,cmd) == ERROR)
	{
		printf("'>' not recv return error\r\n");
		return ERROR;
	}
	_GPRS->ATCIPSEND = 0;
	Clear_Recv_Data();
	//printf("'>' recv ok\r\n");
	//xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量

	_GPRS->P_CMD = "MQTT_DAT";
	_GPRS->P_CMD_CHECK = "SEND OK\r\n";
	_GPRS->SEND_OK = 0;
	
	Send_To_Uart2_Str((int8_t*)buf,buflen);
	
	
	while(_GPRS->SEND_OK == 0)
	{
		cnt++;
		osDelay(100);
		if(cnt == 100)
		{
			cnt = 0;
			Clear_Recv_Data();
			xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
			_GPRS->P_CMD = NULL;
			_GPRS->P_CMD_CHECK = NULL;
			return ERROR;
		}
	}
	Clear_Recv_Data();	//清空处理完毕的数据
	_GPRS->SEND_OK = 0;
	_GPRS->P_CMD = NULL;
	_GPRS->P_CMD_CHECK = NULL;
	xSemaphoreGive(Sim800c_Semaphore);//解锁二值信号量
	return SUCCESS;
}

uint16_t GPRS_CIPSEND(GPRS_TypeDef  * _GPRS,unsigned char* buf, int buflen)
{
	char CIPSEND[64];
	sprintf(CIPSEND,"AT+CIPSEND=%d\r\n\0",buflen);
	
	if(GPRS_DAT(_GPRS,CIPSEND,buf,buflen) == SUCCESS)//等待SEND OK\r\n
	{
		return SUCCESS;
	}
	return ERROR;
}


uint8_t GPRS_RECV(GPRS_TypeDef  * _GPRS,uint16_t* p_len)
{
	_GPRS->P_CMD = "RECV";
	_GPRS->P_CMD_CHECK = "RECV FROM";
	
	if( xSemaphoreTake( send_gprs_dat_xSemaphore, 1000*10 ) == pdTRUE )//等待20秒
	{
		if(strstr(p_GPRS->GPRS_BUF->Buf[_GPRS->GPRS_BUF->RX_Dispose],_GPRS->P_CMD_CHECK))// "RECV FROM"
		{
//			*p_len = _GPRS->GPRS_BUF->Len;
//			memcpy(p_GPRS->DOWN_BUF,p_GPRS->GPRS_BUF->Buf[!_GPRS->GPRS_BUF->Bufuse],_GPRS->GPRS_BUF->Len);
//			memset(p_GPRS->GPRS_BUF->Buf[!_GPRS->GPRS_BUF->Bufuse],'\0',_GPRS->GPRS_BUF->Len);
			Clear_Recv_Data();
			return SUCCESS;
		}else{
			return ERROR;
		}
	}else{
		return ERROR;
	}
}

/*
 * 从串口返回的数据中得到想要的MQTT数据
 * 拷贝到p_GPRS->DOWN_BUF里面
 * */
uint8_t Get_MQTT_Dat_From_Uart(GPRS_TypeDef  * _GPRS)
{
	char *p;
	uint8_t len;			//网络数据的长度
	uint16_t rcv_len = 0;
	/*******等下一个信号量的到来才是数据start***********/
	printf("recv = %d\r\n",GPRS_RECV(p_GPRS,&rcv_len));
	/*********等下一个信号量的到来才是数据end************/

	p = strstr(p_GPRS->DOWN_BUF,"RECV");
	p = strstr(p,"\r\n");
	p += 2;					//p已经指向了数据的头
	len = rcv_len - (p - (p_GPRS->DOWN_BUF));//总长度 - （当前地址-首地址 ） = 接收的数据的长度
	printf("len = %d\r\n",len);
	printf("p = %s",p);

	memcpy(p_GPRS->DOWN_BUF,p,len);
	return SUCCESS;
}

////鑾峰彇IMEI
//uint8_t GPRS_CGSN(GPRS_TypeDef		* _GPRS)
//{
//	char * str = NULL;
//	
//	if(GPRS_CMD(_GPRS,"AT+CGSN\r\n","OK\r\n",5) == SUCCESS)
//	{
//		//printf("GPRS_Rx_Dat GPRS_CGSN = %s\r\n",GPRS_Rx_Dat);
//		str = strstr(GPRS_Rx_Dat,"86");
//		if (str != NULL)
//		{
//			memset(_GPRS->IMEI,0,16);
//			printf("str = %s\r\n",str);
//		//	
//			strncpy(_GPRS->IMEI,str,15);
//			memset(_GPRS->DS,0,64);
//			
////			if (str = strstr(DeviceSecret1,_GPRS->IMEI))
////				strncpy(_GPRS->DS,str+16,32);
////			else if (str = strstr(DeviceSecret2,_GPRS->IMEI))
////				strncpy(_GPRS->DS,str+16,32);
////			else if (str = strstr(DeviceSecret3,_GPRS->IMEI))
////				strncpy(_GPRS->DS,str+16,32);
////			else if (str = strstr(DeviceSecret4,_GPRS->IMEI))
////				strncpy(_GPRS->DS,str+16,32);
////			else if (str = strstr(DeviceSecret5,_GPRS->IMEI))
////				strncpy(_GPRS->DS,str+16,32);
//			
////			memset(_GPRS->HEADER,'\0',64);
////			strcpy(_GPRS->HEADER,TOPIC_PRODUCTKEY);
////			strcat(_GPRS->HEADER,_GPRS->IMEI);
//	//		strcat(_GPRS->HEADER,TOPIC_orderRequest);
//				
//			return SUCCESS;
//		}
//		else return ERROR;
//	}
//	else return ERROR;
//}
//鏈満鍙风爜, +CNUM: ,"8618500226905",145,,4
//uint8_t GPRS_CNUM(GPRS_TypeDef		* _GPRS)
//{
//	char *p1,*p2;
//	uint8_t len;
//	
//	memset(_GPRS->PNUM,0,32);
//	strcpy(_GPRS->PNUM,"13812345678\0");

//	if(GPRS_CMD(_GPRS,"AT+CNUM\r\n","OK\r\n",5) == SUCCESS)
//	{
//		if (strstr(GPRS_Rx_Dat,"+CNUM:"))
//		{
//			while(*GPRS_Rx_Dat != '"')//寮曞彿閲屼负锛�"鏈満鍙风爜"
//			{
//				_GPRS->GPRS_BUF->Len--;
//				if(_GPRS->GPRS_BUF->Len == 0)
//					return ERROR;
//				GPRS_Rx_Dat++;
//			}
//			p1 = GPRS_Rx_Dat;
//			GPRS_Rx_Dat++;
//			while(*GPRS_Rx_Dat != '"')//寮曞彿閲屼负锛�"鏈満鍙风爜"
//			{
//				_GPRS->GPRS_BUF->Len--;
//				if(_GPRS->GPRS_BUF->Len == 0)
//					return ERROR;
//				GPRS_Rx_Dat++;
//			}
//			p2 = GPRS_Rx_Dat;
//			
//			len = p2-p1-1;
//			memset(_GPRS->PNUM,0,32);
//			strncpy(_GPRS->PNUM,p1+1,len);
//			_GPRS->PNUM[len] = 0;     //瀛楃涓茬粨灏鹃渶瑕佹竻0
//			return SUCCESS;
//		}
//		else return ERROR;
//	}
//	else return ERROR;
//}



