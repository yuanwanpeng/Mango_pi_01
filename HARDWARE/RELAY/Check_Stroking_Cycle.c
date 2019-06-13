/*
 * Check_Stroking_Cycle.c
 *
 *  Created on: 2018年10月25日
 *      Author: YuanWP
 */
#include "Check_Stroking_Cycle.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "relay.h"
#include "24c0x.h"
#include "BUZZER.h"
//TimerHandle_t Stroking_Cycle_Timer01Handle;
/*
 * 开始行程周期监测任务
 */
void Start_Check_Stroking_Cycle_Task(void const * argument)
{
	uint8_t relay1,relay2;
	static uint16_t Time = 0;
	int16_t Old_G_Now_Route = G_Now_Route;
	while(1)
	{
		relay1 = RELAY1_STATE;
		relay2 = RELAY2_STATE;
		if(relay1&&relay2)	//=1 表示电机没有正转或者反转  不计算时间
		{
			Time = 0;
		}else{
			if((Time>G_Stroking_Cycle))
			{
				//强制停止转动电机
				Motor_Stop();
				printf("蜂鸣器报警启动 没有接受到应有的干簧管信号\r\n");//蜂鸣器报警
				BUZZER_ENABLE;
			}else{
				Time++;			//计算时间
				if(G_Now_Route!=Old_G_Now_Route)//行程不相等说明正常,重新计时
				{
					Time = 0;
					Old_G_Now_Route = G_Now_Route;
				}
			}
		}
		osDelay(1000);		//一秒
	}
}


//void Create_Stroking_Cycle_Timer01(void)
//{
//	taskENTER_CRITICAL();
//	Stroking_Cycle_Timer01Handle = xTimerCreate("Stroking_Cycle_Timer01",
//										(TickType_t	)100,
//										(UBaseType_t)pdTRUE,
//										(void *		)1,
//										(TimerCallbackFunction_t)Stroking_Cycle_Timer01_CallBack);
//	taskEXIT_CRITICAL();
//}
////计时器的回调函数
//void Stroking_Cycle_Timer01_CallBack(TimerHandle_t xTimer)
//{
//	printf("Stroking_Cycle_Timer01_CallBack\r\n");
//}

