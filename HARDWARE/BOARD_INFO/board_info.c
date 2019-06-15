/*
 * board_info.c
 *
 *  Created on: 2019年4月27日
 *      Author: Yuanwp
 */

#include "board_info.h"
#include "relay.h"
#include "24c0x.h"
#include "semphr.h"
#include "GPRS.h"
#include "SIM800C_Scheduler_data.h"
#define DEF_DEVICE_ID_LEN 				9
#define DEF_DATE_OF_MANUFACTURE_LEN		6
#define DEF_SERIAL_NUMBER				8
extern uint8_t pData[];
extern uint8_t Uart1_RxBuff[];
extern uint8_t g_send_Onenet_buf[];
extern UART_HandleTypeDef huart1;
extern uint8_t Uart1_Rx_Cnt;
extern uint8_t aRxBuffer;			//接收中断缓冲
Version_Information_Struct G_Version_Information;
extern osThreadId Start_Reset_Sim800c_Task_TaskHandle;
extern Onenet_Pack G_Send_Pack;
//Version_Information_Pack G_Version_Information_Pack;
//typedef struct{
//	uint8_t Master_Control;			//主控类型 	交流卷膜，直流卷膜，但接触器（风机），双接触器（棉被），数据采集器
//	uint8_t Device_Type;			//硬件的类型   单机版，gprs版本，wifi版本
//	uint8_t Soft_Versions[10];		//软件版本
//	uint8_t Hardware_Versions[10];	//硬件版本
//	uint8_t Device_Id[10];			//设备ID 	供ONENET使用 实际使用9个字节
//	uint8_t Date_Of_Manufacture[6];	//生产日期
//	uint8_t Serial_Number[10];		//流水号4个字节
//}Version_Information_Struct;

/*
 * 		板子信息检查函数
 */
int8_t Check_Board_Info(void)
{
	uint8_t i;
	AT24CXX_Read(VERSION_INFORMATION_ADDR,(uint8_t*)(&G_Version_Information),sizeof(G_Version_Information));	//读取板子信息

//	printf("G_Version_Information size = %d\r\n",sizeof(G_Version_Information));
	i += Check_Master_Control(&(G_Version_Information.Master_Control));//检测板子控制电机类型
	i += Check_Device_Type(&G_Version_Information.Device_Type);
	i += Check_Soft_Versions(G_Version_Information.Soft_Versions);
	i += Check_Hardware_Versions(G_Version_Information.Hardware_Versions);
	i += Check_Device_ID(G_Version_Information.Device_Id);
	i += Check_Date_Of_Manufacture(G_Version_Information.Date_Of_Manufacture);
	i += Check_Serial_Number(G_Version_Information.Serial_Number);
	if(i!=0)
	{
		printf("write board_info\r\n");
		AT24CXX_Write(VERSION_INFORMATION_ADDR,(uint8_t*)(&G_Version_Information),sizeof(G_Version_Information));	//写入板子信息
	}
	
	return 0;
}
/*
 * 子函数检查设备id辅助作用
 */
int8_t Check_Str_Number(uint8_t* p_Str_Number,uint8_t type)
{
	int8_t i = 0;
	for(i = 0;i < type;i++)
	{
		if((p_Str_Number[i]>='0')&&(p_Str_Number[i]<='9'))
		{
		}else{
			i = -1;
			return i;
		}
	}
	return i;
}
/*
 * 流水号
 */
int8_t Check_Serial_Number(uint8_t* p_str_Serial_Number)
{
	uint8_t str_Serial_Number[10];
	int8_t i = 0;
	memcpy(str_Serial_Number,p_str_Serial_Number,sizeof(str_Serial_Number));
	i = Check_Str_Number(str_Serial_Number,DEF_SERIAL_NUMBER);
	if(i == -1)
	{
		while(1)
		{
			printf("输入流水号8位\\r\\n结尾：\r\n");
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
			if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
			{
				i = Check_Str_Number(Uart1_RxBuff,DEF_SERIAL_NUMBER);
				if((strlen((const char*)Uart1_RxBuff) <= 12)&&(i != -1))
				{
					strncpy((char*)p_str_Serial_Number,(const char*)Uart1_RxBuff,DEF_SERIAL_NUMBER);
					printf("%s\r\n",p_str_Serial_Number);
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
					return 1;
				}else{
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
				}
			}
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
		}
	}
	return 0;
}
/*
 * 生产日期烧录
 */
int8_t Check_Date_Of_Manufacture(uint8_t* p_str_Date_Of_Manufacture)
{
	uint8_t str_Date_Of_Manufacture[10];
	int8_t i = 0;
	memcpy(str_Date_Of_Manufacture,p_str_Date_Of_Manufacture,sizeof(str_Date_Of_Manufacture));
	i = Check_Str_Number(str_Date_Of_Manufacture,DEF_DATE_OF_MANUFACTURE_LEN);
	if(i == -1)
	{
		while(1)
		{
			printf("输入生产日期\\r\\n结尾：\r\n");
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
			if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
			{
				i = Check_Str_Number(Uart1_RxBuff,DEF_DATE_OF_MANUFACTURE_LEN);
				if((strlen((const char*)Uart1_RxBuff) <= 12)&&(i != -1))
				{
					strncpy((char*)p_str_Date_Of_Manufacture,(const char*)Uart1_RxBuff,DEF_DATE_OF_MANUFACTURE_LEN);
					printf("%s\r\n",p_str_Date_Of_Manufacture);
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
					return 1;
				}else{
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
				}
			}
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
		}
	}
	return 0;
}

/*
 * 检测设备ID版本
 */
int8_t Check_Device_ID(uint8_t* p_Device_ID)
{
	uint8_t str_Device_ID[10];
	int8_t i = 0;
	memcpy(str_Device_ID,p_Device_ID,sizeof(str_Device_ID));

	i = Check_Str_Number(str_Device_ID,DEF_DEVICE_ID_LEN);

	if(i == -1)
	{
		while(1)
		{
			printf("输入设备ID\\r\\n结尾：\r\n");
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
			if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
			{
				i = Check_Str_Number(Uart1_RxBuff,DEF_DEVICE_ID_LEN);
				if((strlen((const char*)Uart1_RxBuff) <= 12)&&(i != -1))
				{
					strncpy((char*)p_Device_ID,(const char*)Uart1_RxBuff,DEF_DEVICE_ID_LEN);
					printf("%s\r\n",p_Device_ID);
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
					return 1;
				}else{
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
				}
			}
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
		}
	}
	return 0;
}
/*
 * 检测硬件版本
 */
int8_t Check_Hardware_Versions(uint8_t* p_Hardware_Versions)
{
	uint8_t str_version[10];
	memcpy(str_version,p_Hardware_Versions,sizeof(str_version));

	if(strstr((const char*)str_version,"V") == NULL)
	{
		while(1)
		{
			printf("输入硬件版本\\r\\n结尾格式(V1.x)：\r\n");
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
			if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
			{
				if((strlen((const char*)Uart1_RxBuff) < 10)&&(Uart1_RxBuff[0] == 'V'))
				{
					strncpy((char*)p_Hardware_Versions,(const char*)Uart1_RxBuff,Uart1_Rx_Cnt-2);
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
					return 1;
				}else{
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
				}
			}
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
		}
	}
	return 0;
}

/*
 * 检测软件版本
 */
int8_t Check_Soft_Versions(uint8_t* p_Soft_Versions)
{
	uint8_t str_version[10];
	memcpy(str_version,p_Soft_Versions,sizeof(str_version));

	if(strstr((const char*)str_version,"V") == NULL)
	{
		while(1)
		{
			printf("输入软件版本\\r\\n结尾格式(V1.x)：\r\n");
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
			if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
			{
				if((strlen((const char*)Uart1_RxBuff) < 10)&&(Uart1_RxBuff[0] == 'V'))
				{
					strncpy((char*)p_Soft_Versions,(const char*)Uart1_RxBuff,Uart1_Rx_Cnt-2);
					printf("%s",Uart1_RxBuff);
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
					return 1;
				}else{
					memset(Uart1_RxBuff, 0x00, 64);
					Uart1_Rx_Cnt = 0;
				}
			}
			HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
		}
	}
	return 0;
}
/*
 * 检测硬件类型
 * */
int8_t Check_Device_Type(uint8_t* p_Device_Type)
{
	//需要添加自动检测功能
	if(*p_Device_Type != 0x01){//0x01表示GPRS版本
		*p_Device_Type = 0x01;
	}
	return 0;
}
/*
 * 检查主控类型
 */
int8_t Check_Master_Control(uint8_t* p_Master_Control)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_PinState state;
	
	GPIO_InitStruct.Pin = OUTC3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(OUTC3_GPIO_Port, &GPIO_InitStruct);

	state = RELAY3_STATE;
	if(state == 0){			//等于0表示让电阻拉下去了是交流电机模式
		if(*p_Master_Control != 0x00){	//0x00表示交流卷膜电机
			p_Master_Control = 0x00;	//设置为交流电机记录到全局变量中
		}
	}else{
		//直流电机（待续）
	}
	GPIO_InitStruct.Pin = OUTC3_Pin;	//恢复检测管脚状态
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	return 0;
}

//typedef struct{
//	uint8_t Master_Control;			//主控类型 	交流卷膜，直流卷膜，但接触器（风机），双接触器（棉被），数据采集器
//	uint8_t Device_Type;			//硬件的类型   单机版，gprs版本，wifi版本
//	uint8_t Soft_Versions[10];		//软件版本
//	uint8_t Hardware_Versions[10];	//硬件版本
//	uint8_t Device_Id[10];			//设备ID 	供ONENET使用 实际使用9个字节
//	uint8_t Date_Of_Manufacture[6];	//生产日期
//	uint8_t Serial_Number[10];		//流水号4个字节
//}Version_Information_Struct;

//typedef struct{
//	uint8_t	Pack_type; 		// MQTT类型=3  DupFlag=0 QosLevel=0 Retain=0
//	uint8_t RemainedLength;	//remainedLength=包长度，注意【不计算这一位和前一位】
//	uint16_t  TopicNameLen;	//主题名长度
//	uint8_t TopicName[10];	 //TopicName=$dp
//	uint8_t DpType;			// DpType=3 TimeSet=0
//	uint16_t JsonStrLen;  	 // JsonStrLen=21
//	uint8_t JsonStr[64]; // JsonStr={"Temp":10,"Humi":95}
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

/*
 * 发送设备硬件版本等信息到ONENET
 */
uint16_t init_OneNet_Version_Information_Pack(void)
{
	uint8_t index = 0;
	uint8_t i = 0;
	uint32_t pack_len=0;
	uint8_t str[16];
	unsigned long encodedByte;
	G_Send_Pack.Pack_type = 0x30;
	G_Send_Pack.TopicNameLen = 0x0003;
	memcpy(G_Send_Pack.TopicName,"$dp",strlen("$dp"));
	G_Send_Pack.DpType = 0x03;
	G_Send_Pack.JsonStr = cJSON_CreateObject();
	if(!G_Send_Pack.JsonStr){
		printf("get root faild !\r\n");
	}
	memset(str,'\0',sizeof(str));

	sprintf(str,"%d",G_Version_Information.Master_Control);
	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"Master_Control",(const char*)str);
	sprintf(str,"%d",G_Version_Information.Device_Type);
	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"Device_Type",(const char*)str);
	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"Soft_Versions",(const char*)G_Version_Information.Soft_Versions);
	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"Hardware_Versions",(const char*)G_Version_Information.Hardware_Versions);
	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"Device_Id",(const char*)G_Version_Information.Device_Id);
	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"Date_Of_Manufacture",(const char*)G_Version_Information.Date_Of_Manufacture);
	cJSON_AddStringToObject(G_Send_Pack.JsonStr,"Serial_Number",(const char*)G_Version_Information.Serial_Number);
	char* s;
	s = cJSON_PrintUnformatted(G_Send_Pack.JsonStr);
	pack_len = strlen((const char*)s)+8;
	memset(G_Send_Pack.RemainedLength,'\0',4);
	do{
		 encodedByte = pack_len % 128;
		 pack_len = pack_len / 128;
		 if(pack_len > 0){
			 encodedByte = encodedByte | 128;
		 }
		 G_Send_Pack.RemainedLength[i++] =  (unsigned char)encodedByte;
	}while(pack_len);
	s = cJSON_PrintUnformatted(G_Send_Pack.JsonStr);

	index = Create_Send_Pack(s);
	
	return index;
}
/*
 * 上传版本信息，上电上传
 */
uint8_t Send_Version_Information(void)
{
	BaseType_t err = 1;
	uint32_t NotifyValue;
	uint16_t Pack_Len;
	uint8_t cmp;
	Reset_Uart_DMA();
	while(GPRS_AT_CIPSEND(AT_CIPSEND,">")==ERROR)
	{
		printf("Send_Version_Information AT_CIPSEND reset\r\n");
		xSemaphoreGive(Sim800c_Semaphore);
		taskENTER_CRITICAL();				//进入临界区
		osThreadDef(Reset_Sim800c_Task, Start_Reset_Sim800c_Task, osPriorityNormal, 0, 128);	//开启重启Sim800C初始化调度函数
		Start_Reset_Sim800c_Task_TaskHandle = osThreadCreate(osThread(Reset_Sim800c_Task), NULL);
		taskEXIT_CRITICAL();				//退出临界区
	}
	Pack_Len = init_OneNet_Version_Information_Pack();
	Send_To_Uart2_Str((int8_t*)g_send_Onenet_buf,Pack_Len);
	//需要等待返回数据返回一个SEND OK
	err = xTaskNotifyWait((uint32_t)0x00,
			(uint32_t)0xffffffff,
			(uint32_t*)&NotifyValue,
			(TickType_t)10000);
	cmp = strcmp(Send_data_recv,"\r\nSEND OK\r\n");//对比是不是这个数据
	if(err == pdFALSE){
		return ERROR;
	}else if((err == pdTRUE)&&(NotifyValue == RCV_SEND_OK)&&(cmp == 0)){
		return SUCCESS;
	}
}


