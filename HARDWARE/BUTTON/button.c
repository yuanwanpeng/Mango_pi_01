/*
 * button.c
 *
 *  Created on: 2018年7月24日
 *      Author: YuanWP
 */
#include "button.h"
#include "BUZZER.h"
#include "queue.h"
//#include "Check_Stroking_Cycle.h"
#include "cmsis_os.h"
#define SHAKE_TIME 			100			//按键消逗时长
#define SCAN_BUTTON_SIZE	64			//扫描按键任务堆栈大小
#define BUTTON_EXE_SIZE		128			//执行接收扫描按键任务堆栈大小
osThreadId ScanButtonTaskHandle;		//扫描按键的任务句柄
osThreadId ButtonExeTaskHandle;			//接收扫描按键的键值并发送给执行任务的任务句柄
extern osThreadId Start_LCD12864_TaskHandle;	//开始12864的任务句柄
extern osThreadId defaultTaskHandle;		//起始任务句柄
QueueHandle_t xQueue1;					//消息队列句柄(已经被替换)

/*
 * 开始创建按键任务
 */
void Start_Button_Task(void const * argument)
{

	osThreadDef(ScanButtonTask, Scan_Button_Task, osPriorityNormal, 0, SCAN_BUTTON_SIZE);	//扫描 key任务准备
	ScanButtonTaskHandle = osThreadCreate(osThread(ScanButtonTask), NULL);	//创建扫描 key任务创建

	osThreadDef(ButtonExeTask, Button_Exe_Task, osPriorityNormal, 0, BUTTON_EXE_SIZE);	//按键执行由KEY扫描任务后通过xTaskNotifyWait函数获取键值
	ButtonExeTaskHandle = osThreadCreate(osThread(ButtonExeTask), NULL);	//创建分配KEY值的任务。Start_LCD12864_TaskHandle发送到这个任务中

	while(1)
	{
		osDelay(1);
	}
}
/*
 *    扫描 key
 */
void Scan_Button_Task(void const * argument)
{
	BaseType_t err = 1;
	uint16_t touch_time = 0;
	uint8_t touch_mark = 0;
//	HAL_GPIO_WritePin(GPIOA, PWKEY_CTL_Pin, GPIO_PIN_RESET);//启动SIM800C
	osDelay(6000);
//	HAL_GPIO_WritePin(GPIOA, PWKEY_CTL_Pin, GPIO_PIN_SET);
	osThreadTerminate(defaultTaskHandle);		//删除开始任务
	osThreadTerminate(Start_BUTTON_TaskHandle);	//删除创建按键任务的函数

	while(1)
	{
		if(BUTTON1==0)
		{
			osDelay(SHAKE_TIME);
			if(BUTTON1==0)
			{
				while(BUTTON1==0){
					osDelay(SHAKE_TIME);
					if(touch_time<(TOUCH_TIME+10))
						touch_time++;
					if((touch_time >= TOUCH_TIME)&&(touch_mark==0)){
						touch_mark = 1;//设置标志位
						xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
								(uint32_t)K_1_num_long,
								(eNotifyAction)eSetValueWithOverwrite);
					}
				}
				if((ButtonExeTaskHandle!=NULL)&&(touch_time < TOUCH_TIME))
				{
					xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
							(uint32_t)K_1_num,
							(eNotifyAction)eSetValueWithOverwrite);
				}
				touch_time = 0;
				touch_mark = 0;//设置标志位
			}	//发送信号按键 选择 按键
		}
		if(BUTTON2==0)
		{
			osDelay(SHAKE_TIME);
			if(BUTTON2==0)
			{
				while(BUTTON2==0){
					osDelay(SHAKE_TIME);
					if(touch_time<(TOUCH_TIME+10))
						touch_time++;
					if((touch_time >= TOUCH_TIME)&&(touch_mark==0)){
						touch_mark = 1;//设置标志位
						xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
								(uint32_t)K_2_num_long,
								(eNotifyAction)eSetValueWithOverwrite);
					}
				}
				if((ButtonExeTaskHandle!=NULL)&&(touch_time < TOUCH_TIME))
				{
					xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
							(uint32_t)K_2_num,
							(eNotifyAction)eSetValueWithOverwrite);
				}
				touch_time = 0;
				touch_mark = 0;//设置标志位
			}	//发送信号按键 上 按键
		}
		if(BUTTON3==0)
		{
			osDelay(SHAKE_TIME);
			if(BUTTON3==0)
			{
				while(BUTTON3==0){
					osDelay(SHAKE_TIME);
					if(touch_time<(TOUCH_TIME+10))
						touch_time++;
					if((touch_time >= TOUCH_TIME)&&(touch_mark==0)){
						touch_mark = 1;//设置标志位
						xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
								(uint32_t)K_3_num_long,
								(eNotifyAction)eSetValueWithOverwrite);
					}
				}
				if((ButtonExeTaskHandle!=NULL)&&(touch_time < TOUCH_TIME))
				{
					xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
							(uint32_t)K_3_num,
							(eNotifyAction)eSetValueWithOverwrite);
				}
				touch_time = 0;
				touch_mark = 0;//设置标志位
			}	//发送信号按键 下 按键
		}
		if(BUTTON4==0)
		{
			osDelay(SHAKE_TIME);
			if(BUTTON4==0)
			{
				while(BUTTON4==0){
					osDelay(SHAKE_TIME);
					if(touch_time<(TOUCH_TIME+10))
						touch_time++;
					if((touch_time >= TOUCH_TIME)&&(touch_mark==0)){
						touch_mark = 1;//设置标志位
						xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
								(uint32_t)K_4_num_long,
								(eNotifyAction)eSetValueWithOverwrite);
					}
				}
				if((ButtonExeTaskHandle!=NULL)&&(touch_time < TOUCH_TIME))
				{
					err = xTaskNotify((TaskHandle_t)ButtonExeTaskHandle,
							(uint32_t)K_4_num,
							(eNotifyAction)eSetValueWithOverwrite);
				}
				touch_time = 0;
				touch_mark = 0;//设置标志位
			}	//发送信号按键 取消 按键
		}
		if(err==pdFAIL)
		{
			printf("1 send key value false\r\n");
		}
	}
}

void Button_Exe_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,
				(uint32_t)0xffffffff,
				(uint32_t*)&NotifyValue,
				(TickType_t)portMAX_DELAY);
		if(err==pdTRUE)
		{
			switch((uint8_t)NotifyValue){
			case K_1_num:
				printf("功能按键\r\n");//打印调试
				Buzzer_ONE_Cycle();
				if(Start_LCD12864_TaskHandle!=NULL)
				{
					xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
							(uint32_t)K_1_num,
							(eNotifyAction)eSetValueWithOverwrite);//发送信号按键 选择 按键
					if(err==pdFAIL)
					{
						printf("2 send key value false\r\n");
					}
				}
				break;
			case K_2_num:
				printf("向上按键\r\n");//打印调试
				Buzzer_ONE_Cycle();
				if(Start_LCD12864_TaskHandle!=NULL)
				{
					xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
							(uint32_t)K_2_num,
							(eNotifyAction)eSetValueWithOverwrite);//发送信号按键 上 按键
					if(err==pdFAIL)
					{
						printf("2 send key value false\r\n");
					}
				}
				break;
			case K_3_num:
				printf("向下按键\r\n");//打印调试
				Buzzer_ONE_Cycle();
				if(Start_LCD12864_TaskHandle!=NULL)
				{
					xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
							(uint32_t)K_3_num,
							(eNotifyAction)eSetValueWithOverwrite);//发送信号按键 下 按键
					if(err==pdFAIL)
					{
						printf("2 send key value false\r\n");
					}
				}
				break;
			case K_4_num:
				printf("取消按键\r\n");//打印调试
				printf("TOTAL_HEAP_SIZE = %d\r\n",configTOTAL_HEAP_SIZE);
				printf("剩下Heap= %d\r\n",xPortGetFreeHeapSize());
				Buzzer_ONE_Cycle();	//蜂鸣器响一下
				if(Start_LCD12864_TaskHandle!=NULL)
				{
					err = xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
							(uint32_t)K_4_num,
							(eNotifyAction)eSetValueWithOverwrite);//发送信号按键 取消 按键
					if(err==pdFAIL)
					{
						printf("2 send key value false\r\n");
					}
				}
				break;
			case K_1_num_long:
				printf("选择按键长按\r\n");//打印调试
				Buzzer_TWO_Cycle();		//蜂鸣器长响一下
				err = xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
						(uint32_t)K_1_num_long,
						(eNotifyAction)eSetValueWithOverwrite);
				if(err==pdFAIL)
				{
					printf("1 send key value false\r\n");
				}
				break;
			case K_2_num_long:
				printf("向上按键长按\r\n");//打印调试
				Buzzer_TWO_Cycle();		//蜂鸣器长响一下
				err = xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
						(uint32_t)K_2_num_long,
						(eNotifyAction)eSetValueWithOverwrite);
				if(err==pdFAIL)
				{
					printf("2 send key value false\r\n");
				}
				break;
			case K_3_num_long:
				printf("向下按键长按\r\n");//打印调试
				Buzzer_TWO_Cycle();		//蜂鸣器长响一下
				err = xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
					(uint32_t)K_3_num_long,
					(eNotifyAction)eSetValueWithOverwrite);
				if(err==pdFAIL)
				{
					printf("2 send key value false\r\n");
				}
				break;
			case K_4_num_long:
				printf("取消按键长按\r\n");//打印调试
				Buzzer_TWO_Cycle();		//蜂鸣器长响一下
				err = xTaskNotify((TaskHandle_t)Start_LCD12864_TaskHandle,
					(uint32_t)K_4_num_long,
					(eNotifyAction)eSetValueWithOverwrite);
				if(err==pdFAIL)
				{
					printf("2 send key value false\r\n");
				}
				break;
			}
		}
	}
}



