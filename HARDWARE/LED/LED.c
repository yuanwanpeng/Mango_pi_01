#include "LED.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "delay.h"
#include "BUZZER.h"
#include "24c0x.h"
#include "string.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"
#include "Check_Stroking_Cycle.h"
#include "task.h"
#define Start_RUN_LED_SIZE	64
osThreadId Start_RUN_LED_TaskHandle;
extern osThreadId Start_LED_TaskHandle;
extern osThreadId Start_LCD12864_TaskHandle;
extern RTC_HandleTypeDef hrtc;
TaskHandle_t xIdleTaskHandle;
uint8_t G_Echo;
/*
 *
 */
void Start_RUN_LED_Task(void const * argument)
{
	TaskStatus_t TaskStatus;
//	uint8_t time;
	osThreadTerminate(Start_LED_TaskHandle);
	osDelay(8000);
	while(1)
	{
		HAL_GPIO_WritePin(MCU_RUN_LED_GPIO_Port, MCU_RUN_LED_Pin, GPIO_PIN_RESET);
		osDelay(500);
		HAL_GPIO_WritePin(MCU_RUN_LED_GPIO_Port, MCU_RUN_LED_Pin, GPIO_PIN_SET);
		osDelay(500);
//内存查询start
//		printf("TOTAL_HEAP_SIZE = %d\r\n",configTOTAL_HEAP_SIZE);
//		printf("剩下Heap= %d\r\n",xPortGetFreeHeapSize());
//		time++;
//		if(time > 20)
//		{
//				time = 0;
//				printf("abcdeawefawefawefaewfae\r\n");
//		}
//		vTaskGetInfo(xIdleTaskHandle,
//					&TaskStatus,
//					pdTRUE,
//					eInvalid);
//		printf("TaskStatus = %d\r\n",TaskStatus.eCurrentState);
//内存查询end
		if(Start_LCD12864_HUMI_TaskHandle != NULL)
		{
			vTaskGetInfo(Start_LCD12864_HUMI_TaskHandle,
					&TaskStatus,
					pdTRUE,
					eInvalid);
			//printf("Start_LCD12864_HUMI_TaskHandle = %d\r\n",TaskStatus.eCurrentState);
			if((TaskStatus.eCurrentState == eSuspended)&&(Now_page != Humi_Page))
			{
				G_Echo++;
				if(G_Echo>10)
				{
					while(Now_page != Humi_Page)
					{
						xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,//发送信号到显示函数
											(uint32_t)K_4_num,
											(eNotifyAction)eSetValueWithOverwrite);
						osDelay(300);
					}
				}
			}else{
				G_Echo = 0;
			}
		}

	}
}
/*
 *
 */
void Start_LED_Task(void const * argument)
{
	osThreadDef(Run_Led_Task, Start_RUN_LED_Task, osPriorityNormal, 0, 64);
	Start_RUN_LED_TaskHandle = osThreadCreate(osThread(Run_Led_Task), NULL);


	while(1)
	{

	}
	
}

//void Printf_Task_Size(int8_t* name)
//{
//	UBaseType_t size;
//	size = uxTaskGetStackHighWaterMark( NULL );
//	printf("%s = %d\r\n",name,size);
//}

