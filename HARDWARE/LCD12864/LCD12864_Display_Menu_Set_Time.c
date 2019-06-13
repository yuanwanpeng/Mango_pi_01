/*
 * LCD12864_Display_Menu_Set_Time.c
 *
 *  Created on: 2018年10月22日
 *      Author: YuanWP
 */

#include "LCD12864_Display_Menu_Set_Time.h"
#include "LCD12864_Display_Menu_Max_Route.h"
#include "LCD12864_Display_Menu_Route_Reset.h"
#include "button.h"
#include "HUMI.h"
#include "relay.h"
#include "string.h"
#include "LCD12864.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"
#include "LCD12864_Display_Menu_HUMI_Compensation.h"

osThreadId Start_LCD12864_Set_Time_TaskHandle;

void LCD12864_Display_Set_Time_Interior(int8_t* p_S_Place,RTC_TimeTypeDef* p_Time)
{
	LCD12864_Put_16_16_Char(0xB7,0x14,0x0F,WHITE_BASE);//时字
	LCD12864_Put_16_16_Char(0xB7,0x24,0x12,WHITE_BASE);//间字
	LCD12864_Put_16_16_Char(0xB7,0x34,0x13,WHITE_BASE);//校字
	LCD12864_Put_16_16_Char(0xB7,0x44,0x14,WHITE_BASE);//准字
	LCD12864_Put_8_16_Char(0xB7,0x54,':',WHITE_BASE);

	LCD12864_Display_Set_Time_Refresh(p_S_Place,p_Time);

	LCD12864_Put_16_16_Char(0xB3,0x14,0x02,WHITE_BASE);//设字
	LCD12864_Put_16_16_Char(0xB3,0x24,0x03,WHITE_BASE);//置字

}
void LCD12864_Display_Set_Time_Refresh(int8_t* p_S_Place,RTC_TimeTypeDef* p_Time)
{

	char Hours[3];
	char Minutes[3];
	char Seconds[3];

	memset(Hours,'0',sizeof(Hours));
	memset(Minutes,'0',sizeof(Minutes));
	memset(Seconds,'0',sizeof(Seconds));

	sprintf(Hours,"%02d",(int8_t)p_Time->Hours);
	sprintf(Minutes,"%02d",(int8_t)p_Time->Minutes);
	sprintf(Seconds,"%02d",(int8_t)p_Time->Seconds);

	if(*p_S_Place == HOURS)
	{
		LCD12864_Put_8_16_Char(0xB5,0x2a,Hours[0],BLACK_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x32,Hours[1],BLACK_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x3a,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x42,Minutes[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x4A,Minutes[1],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x52,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x5A,Seconds[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x62,Seconds[1],WHITE_BASE);
		LCD12864_Put_16_16_Char(0xB3,0x14,0x02,(*p_S_Place)==SET);//设字
		LCD12864_Put_16_16_Char(0xB3,0x24,0x03,(*p_S_Place)==SET);//置字
	}else if(*p_S_Place == MINUTES){
		LCD12864_Put_8_16_Char(0xB5,0x2a,Hours[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x32,Hours[1],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x3a,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x42,Minutes[0],BLACK_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x4A,Minutes[1],BLACK_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x52,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x5A,Seconds[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x62,Seconds[1],WHITE_BASE);
		LCD12864_Put_16_16_Char(0xB3,0x14,0x02,(*p_S_Place)==SET);//设字
		LCD12864_Put_16_16_Char(0xB3,0x24,0x03,(*p_S_Place)==SET);//置字
	}else if(*p_S_Place == SECONDS){
		LCD12864_Put_8_16_Char(0xB5,0x2a,Hours[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x32,Hours[1],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x3a,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x42,Minutes[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x4A,Minutes[1],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x52,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x5A,Seconds[0],BLACK_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x62,Seconds[1],BLACK_BASE);
		LCD12864_Put_16_16_Char(0xB3,0x14,0x02,(*p_S_Place)==SET);//设字
		LCD12864_Put_16_16_Char(0xB3,0x24,0x03,(*p_S_Place)==SET);//置字
	}else if(*p_S_Place == SET){
		LCD12864_Put_8_16_Char(0xB5,0x2a,Hours[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x32,Hours[1],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x3a,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x42,Minutes[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x4A,Minutes[1],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x52,':',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x5A,Seconds[0],WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB5,0x62,Seconds[1],WHITE_BASE);
		LCD12864_Put_16_16_Char(0xB3,0x14,0x02,(*p_S_Place)==SET);//设字
		LCD12864_Put_16_16_Char(0xB3,0x24,0x03,(*p_S_Place)==SET);//置字
	}


}
/*
 * 打印此界面
 * */
void LCD12864_Set_Time_Key_Null_Opt(int8_t* p_S_Place,RTC_TimeTypeDef* p_Time)
{
	LCD12864_Display_Set_Time_Interior(p_S_Place,p_Time);		//当前行程
}
void LCD12864_Display_Set_Time_Page(uint8_t key)
{
	static RTC_TimeTypeDef S_Time;
	static int8_t S_Place = HOURS;//当前在小时还是分钟
	static uint8_t Mark = 0;
	if(Mark==0)
	{
		HAL_RTC_GetTime(&hrtc,&S_Time,RTC_FORMAT_BIN);
		Mark = 1;
	}
	switch(key)
	{
	case NULL:	//第一次进入显示全部菜单内容
		LCD12864_Set_Time_Key_Null_Opt(&S_Place,&S_Time);
		break;
	case K_1_num://选择按键
		switch(S_Place){
		case HOURS:
			S_Place = MINUTES;
			break;
		case MINUTES:
			S_Place = SECONDS;
			break;
		case SECONDS:
			S_Place = SET;
			break;
		case SET:
			S_Place = HOURS;
			break;
		}
		LCD12864_Display_Set_Time_Refresh(&S_Place,&S_Time);
		break;
	case K_2_num:	//向上
		switch(S_Place){
		case HOURS:
			if((S_Time.Hours) < 24)
			{
				S_Time.Hours++;
			}
			break;
		case MINUTES:
			if((S_Time.Minutes) < 59)
			{
				S_Time.Minutes++;
			}
			break;
		case SECONDS:
			if((S_Time.Seconds) < 59)
			{
				S_Time.Seconds++;
			}
			break;
		case SET:
			HAL_RTC_SetTime(&hrtc,&S_Time,RTC_FORMAT_BIN);//设置当前时间
			LCD12864_Display_Success();	//显示成功
			osDelay(1500);
			LCD12864_Display_Clear_Success();
			break;
		}
		LCD12864_Display_Set_Time_Refresh(&S_Place,&S_Time);
		break;
	case K_3_num:	//向下
		switch(S_Place){
		case HOURS:
			if((S_Time.Hours) > 0)
			{
				S_Time.Hours--;
			}
			break;
		case MINUTES:
			if((S_Time.Minutes) > 0)
			{
				S_Time.Minutes--;
			}
			break;
		case SECONDS:
			if((S_Time.Seconds) > 0)
			{
				S_Time.Seconds--;
			}
		}
		LCD12864_Display_Set_Time_Refresh(&S_Place,&S_Time);
		break;
	case K_4_num:	//
		if(Now_page == Set_Time_page){
			//返回菜单界面
			LCD12864_Display(0x00,0x00);//清屏
			LCD12864_Display_Menu(NULL);
			Mark = 0;
			Now_page = Menu_Page;
			S_Place = HOURS;//当前在小时还是分钟
			vTaskResume(Start_LCD12864_Menu_TaskHandle);//恢复挂起
			G_Delete_Task_struct.D_Task = Start_LCD12864_Set_Time_TaskHandle;
			Start_LCD12864_Set_Time_TaskHandle = NULL;
			G_Delete_Task_struct.sign = ENABLE;
		}
		break;
	default:
		break;
	}
}

void Start_LCD12864_Set_Time_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;
	LCD12864_Display(0x00,0x00);//清屏
	LCD12864_Display_Set_Time_Page(NULL);//第一次进入显示全部菜单内容
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,	//2 等待按键信息
				(uint32_t)0xffffffff,
				(uint32_t*)&NotifyValue,
				(TickType_t)portMAX_DELAY);
		if(err==pdTRUE)
		{
			LCD12864_Display_Set_Time_Page((uint8_t)NotifyValue);
		}
	}
}

