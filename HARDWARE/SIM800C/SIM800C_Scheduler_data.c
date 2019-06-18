/*
 * SIM800C_Scheduler_data.c
 *
 *  Created on: 2019年5月5日
 *      Author: Administrator
 */

#include "SIM800C_Scheduler_data.h"
#include <string.h>
#include "SIM800C.h"
extern uint8_t pData[];
extern uint8_t G_Sim800C_Signal;
extern char Send_data_recv[];
extern uint8_t Recv_cmd_data[];
extern osThreadId Start_Scheduler_data_Task_TaskHandle;
extern osThreadId Start_Send_State_data_TaskHandle;	//发送数据到上位机句柄
extern osThreadId Start_Recv_Onenet_data_TaskHandle;	//接收数据到上位机句柄
char *G_p_index_start = pData,*G_p_index_end = pData;			//表示一帧数据的前和尾

/*
 *接收到SEND OK\r\n 操作
 */
void Recv_SEND_OK_Opt(void)
{
	BaseType_t err;
	memset(Send_data_recv,'\0',32);
	strcpy(Send_data_recv,"\r\nSEND OK\r\n");
	
	taskENTER_CRITICAL();				//进入临界区
	G_p_index_start = strstr((const char*)G_p_index_end,"\r\nSEND OK\r\n");
	if(G_p_index_end < pData)
	{
		printf("Recv_SEND_OK_Opt =addr= 0x04");
	}
	G_p_index_end = strstr((const char*)G_p_index_end,"\r\nSEND OK\r\n");
	if(G_p_index_end < pData)
	{
		printf("Recv_SEND_OK_Opt =addr= 0x04");
	}
	/*检查后面有没有数据*/
	uint8_t end_data_len;	//结尾后的数据长度
	G_p_index_end += (strlen("\r\nSEND OK\r\n"));	//最后一个字节\r\nSEND OK\r\n_这个底杠位置
	end_data_len = strlen((const char*)G_p_index_end);			//判断底杠后面有没有数据
	
	
	if(Start_Send_State_data_TaskHandle != NULL)
	{
		err = xTaskNotify((TaskHandle_t)Start_Send_State_data_TaskHandle,
			(uint32_t)RCV_SEND_OK,
			(eNotifyAction)eSetValueWithOverwrite);//发送SEND OK信号
	}
	if(end_data_len == 0)	//(=0表示OK\r\n 之后一个字节数据地址)判断是否等于0 0 表示后面没有数据
	{
		Reset_Uart_DMA();
	}//如果有数据 也没有关系，判断的时候从G_p_index_end开始判断;
	taskEXIT_CRITICAL();				//退出临界区
}
/*
 * 接收到信号强度信号操作
 */
void Recv_CSQ_Opt(void)
{
	char *str = strstr((const char*)G_p_index_end,"\r\n+CSQ: ");//如果是信号信息
	G_Sim800C_Signal = atoi(str+7);
//	free(str);
//	xTaskNotify((TaskHandle_t)Start_SIM800C_TaskHandle,//发送至主任务线程
//		(uint32_t)RCV_OK,
//		(eNotifyAction)eSetValueWithOverwrite);
	taskENTER_CRITICAL();				//进入临界区
	G_p_index_start = strstr((const char*)G_p_index_end,"\r\n+CSQ: ");
	G_p_index_end = strstr((const char*)G_p_index_end,"OK\r\n");

//	if(p_index == pData)//判断"\r\n+CSQ:"的首地址知否是pData，相等表示坐标一样是第一个字节
//	{
//		memset(pData+p_index,"\0",p_index);
//	}
	/*检查后面有没有数据*/
	uint8_t end_data_len;	//结尾后的数据长度
//	printf("G_p_index_end = %s\r\n",G_p_index_end);
	G_p_index_end += (strlen("OK\r\n")+1);	//最后一个字节OK\r\n_这个底杠位置
	end_data_len = strlen((const char*)G_p_index_end);	//判断底杠后面有没有数据
//	printf("end_data_len = %d,G_p_index_end = %s,strlen = %d\r\n",end_data_len,G_p_index_end,(strlen("OK\r\n")+1));
	
	if(end_data_len == 0)	//(表示OK\r\n 之后一个字节数据地址)判断是否等于0 0 表示后面没有数据
	{
//		printf("Recv_CSQ_Opt Reset_Uart_DMA\r\n");
		Reset_Uart_DMA();
	}//如果有数据 也没有关系，判断的时候从G_p_index_end开始判断;
	taskEXIT_CRITICAL();				//退出临界区
}
/*
 *	\r\n> 符号操作
 */
void Recv_Symbol_Opt(void)
{
	BaseType_t err;
	char *str;
	memset(Send_data_recv,'\0',128);
	strcpy(Send_data_recv,">");
	taskENTER_CRITICAL();				//进入临界区
	G_p_index_start = strstr((const char*)G_p_index_end,"\r\n>");
		if(G_p_index_end < pData)
	{
		printf("Recv_Symbol_Opt =addr= 0x04");
	}
	G_p_index_end = strstr((const char*)G_p_index_end,"\r\n> ");
	if(G_p_index_end < pData)
	{
		printf("Recv_Symbol_Opt =addr= 0x04");
	}
	/*检查后面有没有数据*/
	uint8_t end_data_len;	//结尾后的数据长度
	G_p_index_end += (strlen("\r\n> "));	//最后一个字节OK\r\n_这个底杠位置
	end_data_len = strlen((const char*)G_p_index_end);	//判断底杠后面有没有数据
	
	err = xTaskNotify((TaskHandle_t)Start_Send_State_data_TaskHandle,//发送至主任务线程
		(uint32_t)RCV_WRITE_SYMBEL,
		(eNotifyAction)eSetValueWithOverwrite);
//	printf(">G_p_index_end = %s,end_data_len = %d,err = %d\r\n",G_p_index_end,end_data_len,err);
	if(end_data_len == 0)	//(表示OK\r\n 之后一个字节数据地址)判断是否等于0 0 表示后面没有数据
	{
//		printf("Reset_Uart_DMA\r\n");
		Reset_Uart_DMA();
	}//如果有数据 也没有关系，判断的时候从G_p_index_end开始判断;
	
	taskEXIT_CRITICAL();				//退出临界区
}
/*
 *  接收到了数据'0'开头
 */
void Recv_Data_Opt(void)
{
	//检测数据进行抛弃
	uint8_t i = 0;
	uint32_t encodedByte;
	uint8_t check_len = strlen(G_p_index_end+1);//检查使用几个字节得出数据长度最长4字节
	uint8_t mqtt_len[4] = {0x00,0x00,0x00,0x00};
	uint16_t TopicNameLen = 0x0000;
	if(check_len<=4){
		strcpy((char*)mqtt_len,(const char*)(G_p_index_end+1));
		uint32_t multiplier = 1;
		uint32_t value = 0;
		do{
			encodedByte = mqtt_len[i++];
			value += (encodedByte & 127) * multiplier;
			multiplier *= 128;
			if (multiplier > 128*128*128)
				break;
		}while ((encodedByte & 128) != 0);
		printf("value = %d\r\n",value);//数据长度
		TopicNameLen |= (G_p_index_end[check_len+1]<<8);	//第一位
		TopicNameLen |= G_p_index_end[check_len+2];			//第二位
		printf("TopicNameLen = %d\r\n",TopicNameLen);
	//	printf("rcv_dat = %s\r\n",(pData+1+check_len+2+TopicNameLen));
		strcpy(Recv_cmd_data,(const char*)(G_p_index_end+1+check_len+2+TopicNameLen));
		G_p_index_end += strlen((const char*)(G_p_index_end+1+check_len+2+TopicNameLen))+1;//更改指针下标到最后一个字节的下一个字节
		//root = cJSON_Parse((const char*)(pData+1+check_len+2+TopicNameLen));
		xTaskNotify((TaskHandle_t)Start_Recv_Onenet_data_TaskHandle,
					(uint32_t)RCV_CMD_DAT,
					(eNotifyAction)eSetValueWithOverwrite);

		uint8_t end_data_len = strlen((const char*)G_p_index_end);	//判断底杠后面有没有数据

		taskENTER_CRITICAL();				//进入临界区
		if(end_data_len == 0)	//(表示OK\r\n 之后一个字节数据地址)判断是否等于0 0 表示后面没有数据
		{
			Reset_Uart_DMA();
		}//如果有数据 也没有关系，判断的时候从G_p_index_end开始判断;
		taskEXIT_CRITICAL();				//退出临界区
	}else{
	}
}
/*
 * 解析sim800C返回数据的task
 */
void Start_Scheduler_data_Task(void const * argument)
{
	char *str;
	Reset_Uart_DMA();
	while(1)
	{
		//监控pData数据  G_p_index_end指针永远指向一帧处理完数据的在以后一个字节
		if(strstr((const char *)G_p_index_end,"OK\r\n"))	//接收到了OK
		{
			str = strstr((const char*)G_p_index_end,"\r\n+CSQ: ");//如果是信号信息
			if(str != NULL)
			{
				printf("into csq = %s\r\n",G_p_index_end);
				Recv_CSQ_Opt();			//接收到信号强度信号操作
			}
			str = strstr((const char*)G_p_index_end,"\r\nSEND OK\r\n");//如果是信号信息
			if(str != NULL)
			{
				Recv_SEND_OK_Opt();		//接收到SEND OK操作
			}
		}
		if(strstr((const char*)G_p_index_end,"\r\n> "))//返回了>符号
		{
			//printf("pData> = %s\r\n",pData);
			str = strstr((const char*)G_p_index_end,"\r\n> ");//如果是待发送符号信息
			if(str != NULL)
			{
				Recv_Symbol_Opt();
			}
		}
		if(G_p_index_end[0] == '0')//0x30开头
		{
			Recv_Data_Opt();
		}
	}
}


