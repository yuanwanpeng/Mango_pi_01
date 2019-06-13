/*
 * relay.c
 *
 *  Created on: 2018年7月25日
 *      Author: YuanWP
 */
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "relay.h"
#include "24c0x.h"
#include "Check_Stroking_Cycle.h"
#include "Monitor_IN1.h"



osThreadId Start_Relay_TaskHandle;

osThreadId Start_Check_Stroking_Cycle_TaskHandle;
osThreadId Start_Monitor_IN1_TaskHandle;
uint8_t G_New_Mode = 1;
uint16_t G_Max_Route = 159;
int16_t G_Old_Route;
int16_t G_Now_Route = 10;//当前行程
uint8_t G_Stroking_Cycle;//行程周期

void Check_Max_And_Now_Route(void)
{
	uint8_t R_Max_Route_H;
	uint8_t R_Max_Route_L;
	uint8_t R_Now_Route_H;
	uint8_t R_Now_Route_L;

	AT24CXX_Read(MAX_ROUTE,&R_Max_Route_H,sizeof(R_Max_Route_H));
	AT24CXX_Read(MAX_ROUTE+1,&R_Max_Route_L,sizeof(R_Max_Route_L));
	G_Max_Route = (R_Max_Route_H<<8)|R_Max_Route_L;

	if((G_Max_Route>9999)||(G_Max_Route<10)) //将最大行程复位为10
	{
		Init_Max_Route(10);
	}
	AT24CXX_Read(NOW_ROUTE,&R_Now_Route_H,sizeof(R_Now_Route_H));
	AT24CXX_Read(NOW_ROUTE+1,&R_Now_Route_L,sizeof(R_Now_Route_L));
	G_Now_Route = (R_Now_Route_H<<8)|R_Now_Route_L;
	if(G_Now_Route>9999) //将当前行程复位为0
	{
		Init_Now_Route(0);
	}
}

void Init_Max_Route(uint16_t Max_Route)
{
	uint8_t R_Max_Route_H,i;
	uint8_t R_Max_Route_L;

	G_Max_Route = Max_Route;
	R_Max_Route_H = G_Max_Route>>8;
	R_Max_Route_L = G_Max_Route;
	AT24CXX_Write(MAX_ROUTE,&R_Max_Route_H,sizeof(R_Max_Route_H));
	AT24CXX_Write(MAX_ROUTE+1,&R_Max_Route_L,sizeof(R_Max_Route_L));

	for(i = 0;i < 8;i++)//遍历下面的数据是否是开
	{
		if(G_Strategy[i].Route > G_Max_Route)
		{
			G_Strategy[i].Route = G_Max_Route;			
			AT24CXX_Write(i*STRATEGY_ADDR,((uint8_t*)G_Strategy)+(sizeof(G_Strategy[i])*i),sizeof(G_Strategy[i]));
		}
	}
}
void Init_Now_Route(int16_t Now_Route)
{
	uint8_t R_Now_Route_H;
	uint8_t R_Now_Route_L;

	G_Now_Route = Now_Route;
	R_Now_Route_H = G_Now_Route>>8;
	R_Now_Route_L = G_Now_Route;
	AT24CXX_Write(NOW_ROUTE,&R_Now_Route_H,sizeof(R_Now_Route_H));
	AT24CXX_Write(NOW_ROUTE+1,&R_Now_Route_L,sizeof(R_Now_Route_L));
}
/*
 *  继电器控制任务
 * */
void Start_Relay_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;

	osThreadDef(Check_Stroking_Cycle_Task, Start_Check_Stroking_Cycle_Task, osPriorityNormal, 0, 64);//创建检测行程周期的模块
	Start_Check_Stroking_Cycle_TaskHandle = osThreadCreate(osThread(Check_Stroking_Cycle_Task), NULL);

 	osThreadDef(Monitor_IN1_Task, Start_Monitor_IN1_Task, osPriorityNormal, 0, 64);//创建监听IN1管脚的task
	Start_Monitor_IN1_TaskHandle = osThreadCreate(osThread(Monitor_IN1_Task), NULL);

	G_Old_Route = G_Now_Route;//将当前行程记录到我的buff buff将行程加5得到最大行程后的5个点位

	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,	//2 等待按键信息 切换到手动模式下
				(uint32_t)0xffffffff,
				(uint32_t*)&NotifyValue,
				(TickType_t)portMAX_DELAY);
		if(err==pdTRUE)
		{
			Relay_key_Opt((uint8_t)NotifyValue);
		}
	}
}
void Relay_key_Opt(uint8_t key)
{
	switch(key){
		case K_1_num_long:	//长按上按键后需要正转
			if(G_New_Mode == MANUAL_MODE){
				G_New_Mode = 100;//切换到手动/自动模式设置到没有的策略中让策略自动更新这里设置的100
			}else if(G_New_Mode != MANUAL_MODE){
				G_New_Mode = MANUAL_MODE;
			}
			LCD12864_Put_Str_At_Present_Mode(G_New_Mode);//显示手动/自动模式
			break;
		case K_2_num_long:	//长按上按键后需要正转
			if(G_New_Mode == MANUAL_MODE)
			{
				Motor_Forward();
			}
			break;
		case K_3_num_long:	//长按下按键后需要反转
			if(G_New_Mode == MANUAL_MODE)
			{
				Motor_Reverses();
			}
			break;
		case K_4_num_long:	//长按取消按键后需要停止
			if(G_New_Mode == MANUAL_MODE)
			{
				Motor_Stop();
			}
			break;
	}
}
void Motor_Stop(void)
{
	uint8_t In_State,relay1,relay2;
	In_State = IN1_STATE;
	relay1 = RELAY1_STATE;
	relay2 = RELAY2_STATE;
	G_Old_Route = G_Now_Route;//将当前行程记录到我的buff buff将行程加5得到最大行程后的5个点位
	while((In_State==1)&&((relay1==0)||(relay2==0)))//判断他等不等于0
	{
		osDelay(50);
		In_State = IN1_STATE;
	}
	RELAY1_DISABLE;
	RELAY2_DISABLE;
	LED_REV_DISABLE;
	LED_FWD_DISABLE;
}
/*
 * 正转函数
 * */
void Motor_Forward(void)
{
	uint8_t relay1,relay2;
	relay1 = RELAY1_STATE;
	relay2 = RELAY2_STATE;
	if(relay2 == 1)
	{
		RELAY2_DISABLE;
		osDelay(500);
		RELAY1_ENABLE;
		LED_FWD_ENABLE;
		LED_REV_DISABLE;
	}else if(relay1 == 1){

	}
}
/*
 * 反转函数
 * */
void Motor_Reverses(void)
{
	uint8_t relay1,relay2;
	relay1 = RELAY1_STATE;
	relay2 = RELAY2_STATE;
	if(relay1 == 1)
	{
		RELAY1_DISABLE;
		osDelay(500);
		RELAY2_ENABLE;
		LED_REV_ENABLE;
		LED_FWD_DISABLE;
	}else if(relay2 == 1)
	{
	}
}

/*
 * 复位当前行程为0
 */
void G_Route_Reset(void)
{
	printf("G_Now_Route = %d\r\n",G_Now_Route);
	G_Now_Route = 0;
	G_Old_Route = G_Now_Route;//将当前行程记录到我的buff buff将行程加5得到最大行程后的5个点位
	Init_Now_Route(0);
	printf("G_Now_Route = %d\r\n",G_Now_Route);
}
void Check_Stroking_Cycle(void)
{
	uint8_t Stroking_Cycle;//G_Stroking_Cycle
	AT24CXX_Read(STORKING_CYCLE,&Stroking_Cycle,sizeof(Stroking_Cycle));
	if((Stroking_Cycle>100)||(Stroking_Cycle<3))
	{
		Init_Stroking_Cycle(3);
	}else{
		G_Stroking_Cycle = Stroking_Cycle;
	}
}
void Init_Stroking_Cycle(uint8_t Stroking_Cycle)
{
	AT24CXX_Write(STORKING_CYCLE,&Stroking_Cycle,sizeof(Stroking_Cycle));
	G_Stroking_Cycle = Stroking_Cycle;
}

