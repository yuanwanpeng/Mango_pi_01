/*
 * SIM800C.c
 *
 *  Created on: 2018年7月25日
 *      Author: YuanWP
 */
#include "SIM800C.h"
#include "main.h"
#include "cmsis_os.h"
#include "uart.h"
#include "stm32f1xx_hal.h"
#include "GPRS.h"
#include "Mqtt.h"
#include "board_info.h"
#include "SIM800C_Scheduler_data.h"
#include "LCD12864.h"
#include "MQTTPacket.h"
#include "string.h"
//#include "ALIOT.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;
extern uint8_t a2RxBuffer;			//接收中断缓冲
extern uint8_t Uart2_RxBuff[];
extern DMA_HandleTypeDef hdma_usart2_rx;
GPRS_TypeDef *p_GPRS,GPRS;
//Aliot_TypeDef							*p_Aliot, Aliot;
BUF_TypeDef	GPRS_Buf_T;
//Aliot_TypeDef							*p_Aliot, Aliot;

SemaphoreHandle_t Sim800c_Semaphore;//这是一个互斥信号量
extern SemaphoreHandle_t recv_gprs_cmd_xSemaphore;//二值信号量
extern SemaphoreHandle_t send_gprs_dat_xSemaphore;//二值信号量
osThreadId Start_Send_State_data_TaskHandle = NULL;	//发送数据到上位机句柄
osThreadId Start_Recv_Onenet_data_TaskHandle;	//接收数据到上位机句柄
osThreadId Start_Scheduler_data_Task_TaskHandle;
osThreadId Start_Reset_Sim800c_Task_TaskHandle;

osThreadId Sim800c_Rcv_TaskHandle;
extern	GPRS_TypeDef						*p_GPRS;
extern uint8_t Uart2_Rx_Cnt;
extern osThreadId Start_SIM800C_TaskHandle;	//800c句柄
//uint8_t G_Sim800C_Signal;


void reset_Sim800C(void)
{
	SIM800C_PWKEY_CTL_L;
	osDelay(1000);
	SIM800C_PWKEY_CTL_H;
	osDelay(2000);
	SIM800C_PWKEY_CTL_L;
	osDelay(1000);
	printf("restart 800c\r\n");
}


/*
复位Sim800C
*/
void Start_Reset_Sim800c_Task(void const * argument)
{
	uint16_t flag=0;
	vTaskSuspend( Start_SIM800C_TaskHandle );									//挂起800C的进程
	
	if(Start_Send_State_data_TaskHandle != NULL){
		osThreadTerminate(Start_Send_State_data_TaskHandle);		//删除发送任务
		Start_Send_State_data_TaskHandle = NULL;
	}
	
	p_GPRS = &GPRS;
	p_GPRS->GPRS_BUF = &GPRS_Buf_T;

	Reset_Uart_DMA(p_GPRS);
	printf("GPRS_Init = %d\r\n",GPRS_Init(p_GPRS));
	printf("GPRS_CIPCLOSE_OFF = %d\r\n",GPRS_CIPCLOSE_OFF(p_GPRS));

	while(flag != SUCCESS)
	{
		flag = GPRS_RECONNECT(p_GPRS,Port/*p_Aliot->IOT_Host*/);
		//printf("TCP OK flag = %d\r\n",flag);
	}
	printf("TCP OK flag = %d\r\n",flag);	
	while(p_GPRS->CIPSTATUS != 6)
	{
		GPRS_CIPSTATUS(p_GPRS);
		osDelay(100);
		printf("1.0 p_GPRS->CIPSTATUS = %d\r\n",p_GPRS->CIPSTATUS);
	}
	vTaskResume( Start_SIM800C_TaskHandle );									//恢复挂起
	
	taskENTER_CRITICAL();				//进入临界区
	osThreadDef(Send_State_data_Task, Start_Send_State_data_Task, osPriorityNormal, 0, 128);	//创建返回温湿度+正反转状态数
	Start_Send_State_data_TaskHandle = osThreadCreate(osThread(Send_State_data_Task), NULL);	//

	taskEXIT_CRITICAL();				//退出临界区
	G_Delete_Task_struct.D_Task = Start_Reset_Sim800c_Task_TaskHandle;
	Start_Reset_Sim800c_Task_TaskHandle = NULL;
	G_Delete_Task_struct.sign = ENABLE;
	while(1)
	{
//		xSemaphoreTake(Sim800c_Semaphore,portMAX_DELAY);//获取互斥信号量
//		GPRS_CIPCLOSE_OFF(p_GPRS);
//		xSemaphoreGive(Sim800c_Semaphore);//释放信号量
		osDelay(1000);
	}
}
/*
 * 启动SIM800C任务
 * */
void Start_Sim800c_Task(void const * argument)
{
//	HAL_TIM_Base_Start_IT(&htim2);
	Sim800c_Semaphore = xSemaphoreCreateMutex();			//创建二值信号量
	if( Sim800c_Semaphore == NULL )
	{
		printf("Sim800c_Semaphore = NULL\r\n");
	}

	taskENTER_CRITICAL();				//进入临界区
	
	osThreadDef(Scheduler_data_Task, Start_Scheduler_data_Task, osPriorityAboveNormal, 0, 128);	//开启数据返回任务调度函数
	Start_Scheduler_data_Task_TaskHandle = osThreadCreate(osThread(Scheduler_data_Task), NULL);
	
	osThreadDef(Reset_Sim800c_Task, Start_Reset_Sim800c_Task, osPriorityNormal, 0, 128);	//开启重启Sim800C初始化调度函数
	Start_Reset_Sim800c_Task_TaskHandle = osThreadCreate(osThread(Reset_Sim800c_Task), NULL);

	osThreadDef(Recv_Mqtt_data_Task, Start_Recv_Mqtt_data_Task, osPriorityNormal, 0, 128);	//创建接收数据
	Start_Recv_Onenet_data_TaskHandle = osThreadCreate(osThread(Recv_Mqtt_data_Task), NULL);	//

	taskEXIT_CRITICAL();				//退出临界区

	while(1)
	{
		osDelay(5000);		
		GPRS_CIPSTATUS(p_GPRS);
		osDelay(5000);		
		GPRS_CSQ(p_GPRS);
		printf("_GPRS->CSQ = %d\r\n",p_GPRS->CSQ);			//打印CSQ信号强度
		printf("p_GPRS->CIPSTATUS = %d\r\n",p_GPRS->CIPSTATUS);	//打印TCP连接状态
		//12864显示出信号
		
	}
}






