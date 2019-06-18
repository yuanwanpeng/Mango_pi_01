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
//#include "stm32f1xx_hal_dma.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t a2RxBuffer;			//接收中断缓冲
extern uint8_t Uart2_RxBuff[];
extern DMA_HandleTypeDef hdma_usart2_rx;
extern uint8_t *G_p_index_start,*G_p_index_end;

SemaphoreHandle_t Sim800c_Semaphore;//这是一个互斥信号量

osThreadId Start_Send_State_data_TaskHandle = NULL;	//发送数据到上位机句柄
osThreadId Start_Recv_Onenet_data_TaskHandle;	//接收数据到上位机句柄
osThreadId Start_Scheduler_data_Task_TaskHandle;
osThreadId Start_Reset_Sim800c_Task_TaskHandle;

osThreadId Sim800c_Rcv_TaskHandle;
extern uint8_t Uart2_Rx_Cnt;
extern osThreadId Start_SIM800C_TaskHandle;	//800c句柄
uint8_t G_Sim800C_Signal;


void reset_Sim800C(void)
{
	//vTaskSuspend( Start_Scheduler_data_Task_TaskHandle );		//挂起lcd12864显示主菜单的进程
	SIM800C_PWKEY_CTL_L;
	osDelay(1000);
	SIM800C_PWKEY_CTL_H;
	osDelay(3000);
	SIM800C_PWKEY_CTL_L;
	osDelay(2000);
	printf("restart 800c\r\n");
	Reset_Uart_DMA();
}

void init_Sim800C(void)
{
	uint8_t start_state = 0;
	reset_Sim800C();
	Reset_Uart_DMA();
	while(GPRS_AT(AT,"OK\r") == ERROR)
	{
		osDelay(300);
		start_state++;
		if(start_state == 3)
		{
			start_state = 0;
			reset_Sim800C();
			Reset_Uart_DMA();
		}
	}
	Reset_Uart_DMA();
	osDelay(300);	
	GPRS_ATE0(ATE0);
	osDelay(5000);
	Reset_Uart_DMA();
	vTaskResume( Start_Scheduler_data_Task_TaskHandle );//恢复挂起
	do{
		Reset_Uart_DMA();
		Get_Sim800C_Signal();
		printf("Get_Sim800C_Signal\r\n");
		osDelay(1000);
	}while(G_Sim800C_Signal<10);
	vTaskSuspend( Start_Scheduler_data_Task_TaskHandle );//挂起
	Reset_Uart_DMA();
	GPRS_AT_CGDCONT(AT_CGDCONT,"OK\r");
	printf("GPRS_AT_CGDCONT = ok\r\n");
	Reset_Uart_DMA();
	osDelay(300);
	GPRS_AT_CGATT(AT_CGATT,"OK\r");
	Reset_Uart_DMA();
	osDelay(300);
	GPRS_AT_CIPCSGP(AT_CIPCSGP,"OK\r");
	Reset_Uart_DMA();
	osDelay(300);
	GPRS_AT_CIPSTATUS(AT_CIPSTATUS,"OK\r");//查询状态INITIAL
	osDelay(300);
	Reset_Uart_DMA();
}



/*
复位Sim800C
*/
void Start_Reset_Sim800c_Task(void const * argument)
{
	osDelay(100);
	vTaskSuspend( Start_Scheduler_data_Task_TaskHandle );	//挂起调度数据进程
	vTaskSuspend( Start_SIM800C_TaskHandle );				//挂起800C的进程
	
	if(Start_Send_State_data_TaskHandle != NULL){
		osThreadTerminate(Start_Send_State_data_TaskHandle);//删除发送任务
		Start_Send_State_data_TaskHandle = NULL;
	}
	printf("init_sim800C\r\n");
	init_Sim800C();
	while(GPRS_AT_CIPSTART(AT_CIPSTART,"CONNECT OK") == ERROR)
	{
		reset_Sim800C();
		init_Sim800C();
	}
	Reset_Uart_DMA();
	
	vTaskResume( Start_Scheduler_data_Task_TaskHandle );	//恢复挂起
	vTaskResume( Start_SIM800C_TaskHandle );							//恢复挂起
	
	taskENTER_CRITICAL();				//进入临界区
	
	osThreadDef(Send_State_data_Task, Start_Send_State_data_Task, osPriorityNormal, 0, 128);	//创建返回温湿度+正反转状态数
	Start_Send_State_data_TaskHandle = osThreadCreate(osThread(Send_State_data_Task), NULL);	//

	taskEXIT_CRITICAL();				//退出临界区
	G_Delete_Task_struct.D_Task = Start_Reset_Sim800c_Task_TaskHandle;
	Start_Reset_Sim800c_Task_TaskHandle = NULL;
	G_Delete_Task_struct.sign = ENABLE;
	while(1)
	{
		osDelay(1000);
	}
}

void Start_Sim800c_Task(void const * argument)
{
	Sim800c_Semaphore = xSemaphoreCreateMutex();
	if( Sim800c_Semaphore == NULL )
	{
		printf("Sim800c_Semaphore = NULL\r\n");
	}
	else
	{
	//	printf("xSemaphore = success\r\n");
	}
	
	taskENTER_CRITICAL();				//进入临界区
	
	osThreadDef(Scheduler_data_Task, Start_Scheduler_data_Task, osPriorityNormal, 0, 128);	//开启数据返回任务调度函数
	Start_Scheduler_data_Task_TaskHandle = osThreadCreate(osThread(Scheduler_data_Task), NULL);
	
	osThreadDef(Reset_Sim800c_Task, Start_Reset_Sim800c_Task, osPriorityNormal, 0, 128);	//开启重启Sim800C初始化调度函数
	Start_Reset_Sim800c_Task_TaskHandle = osThreadCreate(osThread(Reset_Sim800c_Task), NULL);

	osThreadDef(Recv_Onenet_data_Task, Start_Recv_Onenet_data_Task, osPriorityNormal, 0, 128);	//创建接收数据
	Start_Recv_Onenet_data_TaskHandle = osThreadCreate(osThread(Recv_Onenet_data_Task), NULL);	//

	taskEXIT_CRITICAL();				//退出临界区

	while(1)
	{
		xSemaphoreTake(Sim800c_Semaphore,portMAX_DELAY);//获取互斥信号量
		Get_Sim800C_Signal();
		xSemaphoreGive(Sim800c_Semaphore);//释放信号量
		//printf("G_Sim800C_Signal = %d\r\n",G_Sim800C_Signal);
		//12864显示出信号
		osDelay(15000);
	}
}






