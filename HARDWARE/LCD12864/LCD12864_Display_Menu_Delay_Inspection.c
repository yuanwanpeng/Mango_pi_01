/*
 * LCD12864_Display_Menu_Delay_Inspection.c
 *
 *  Created on: 2018年10月22日
 *      Author: YuanWP
 */
#include "LCD12864_Display_Menu_Max_Route.h"
#include "LCD12864_Display_Menu_Route_Reset.h"
#include "button.h"
#include "HUMI.h"
#include "relay.h"
#include "string.h"
#include "LCD12864.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"
#include "LCD12864_Display_Menu_Delay_Inspection.h"
osThreadId Start_LCD12864_Delay_Inspection_TaskHandle;

void LCD12864_Display_Delay_Inspection_Interior(uint16_t* p_S_Delay_Inspection,uint8_t* p_Coordinate)
{
	LCD12864_Put_16_16_Char(0xB7,0x14,0x0E,WHITE_BASE);//延字
	LCD12864_Put_16_16_Char(0xB7,0x24,0x0F,WHITE_BASE);//时字
	LCD12864_Put_16_16_Char(0xB7,0x34,0x10,WHITE_BASE);//检字
	LCD12864_Put_16_16_Char(0xB7,0x44,0x11,WHITE_BASE);//查字
	LCD12864_Put_8_16_Char(0xB7,0x54,':',WHITE_BASE);

	LCD12864_Display_Delay_Inspection_Refresh(p_S_Delay_Inspection,p_Coordinate);

	LCD12864_Put_16_16_Char(0xB5,0x52,0x2A,WHITE_BASE);//秒字
 
	LCD12864_Put_16_16_Char(0xB3,0x14,0x02,WHITE_BASE);//设字
	LCD12864_Put_16_16_Char(0xB3,0x24,0x03,WHITE_BASE);//置字
}
void LCD12864_Display_Delay_Inspection_Refresh(uint16_t* p_S_Delay_Inspection,uint8_t* p_Coordinate)
{
	char Delay_Inspection_arr[3];
	memset(Delay_Inspection_arr,'0',sizeof(Delay_Inspection_arr));

	sprintf(Delay_Inspection_arr,"%03d",*p_S_Delay_Inspection);
	//printf("Delay_Inspection_arr = %s,p_S_Delay_Inspection = %d\r\n",Delay_Inspection_arr,*p_S_Delay_Inspection);
	LCD12864_Put_8_16_Char(0xB5,0x3a,Delay_Inspection_arr[0],(*p_Coordinate)==DELAY_INSPECTION_0);
	LCD12864_Put_8_16_Char(0xB5,0x42,Delay_Inspection_arr[1],(*p_Coordinate)==DELAY_INSPECTION_1);
	LCD12864_Put_8_16_Char(0xB5,0x4a,Delay_Inspection_arr[2],(*p_Coordinate)==DELAY_INSPECTION_2);

	LCD12864_Put_16_16_Char(0xB3,0x14,0x02,(*p_Coordinate)==DELAY_INSPECTION_SET);//设字
	LCD12864_Put_16_16_Char(0xB3,0x24,0x03,(*p_Coordinate)==DELAY_INSPECTION_SET);//置字
}
/*
 * 打印此界面
 * */
void LCD12864_Delay_Inspection_Key_Null_Opt(uint16_t* p_S_Delay_Inspection,uint8_t* p_Coordinate)
{
	LCD12864_Display_Delay_Inspection_Interior(p_S_Delay_Inspection,p_Coordinate);		//当前行程
}
void LCD12864_Display_Delay_Inspection_Page(uint8_t key)
{
	static uint16_t S_Delay_Inspection = 0;
	static uint8_t Mark = 0;
	static uint8_t S_Coordinate = DELAY_INSPECTION_0;//当前坐标
	if(Mark==0)
	{
		S_Delay_Inspection = G_Delay_Inspection;
		Mark = 1;
	}
	switch(key)
	{
	case NULL:	//第一次进入显示全部菜单内容
		LCD12864_Delay_Inspection_Key_Null_Opt(&S_Delay_Inspection,&S_Coordinate);
		break;
	case K_1_num://选择按键
//		LCD12864_Display_Delay_Inspection_Refresh(&S_Delay_Inspection);
//		G_Delay_Inspection = S_Delay_Inspection;
//		Init_Delay_Inspection(G_Delay_Inspection);
		if(S_Coordinate<3)
		{
			S_Coordinate++;
		}else{
			S_Coordinate=0;
		}
		LCD12864_Display_Delay_Inspection_Refresh(&S_Delay_Inspection,&S_Coordinate);
		break;
	case K_2_num:	//向上按键
		switch(S_Coordinate){
		case DELAY_INSPECTION_0:	//百位
			if((S_Delay_Inspection+100) <= 600){
				S_Delay_Inspection += 100;
			}
			break;
		case DELAY_INSPECTION_1:	//十位
			if((S_Delay_Inspection+10) <= 600){
				S_Delay_Inspection += 10;
			}
			break;
		case DELAY_INSPECTION_2:	//个位
			if((S_Delay_Inspection+1) <= 600){
				S_Delay_Inspection += 1;
			}
			break;
		case DELAY_INSPECTION_SET:	//设置位
			LCD12864_Display_Delay_Inspection_Refresh(&S_Delay_Inspection,&S_Coordinate);
			G_Delay_Inspection = S_Delay_Inspection;
			Init_Delay_Inspection(G_Delay_Inspection);
			LCD12864_Display_Success();	//显示成功
			osDelay(1500);
			LCD12864_Display_Clear_Success();
			break;
		}
		LCD12864_Display_Delay_Inspection_Refresh(&S_Delay_Inspection,&S_Coordinate);
		break;
	case K_3_num:	//向下按键
		switch(S_Coordinate){
		case DELAY_INSPECTION_0:	//百位
			if((S_Delay_Inspection-100) >= 0){
				S_Delay_Inspection -= 100;
			}
			break;
		case DELAY_INSPECTION_1:	//十位
			if((S_Delay_Inspection-10) >= 0){
				S_Delay_Inspection -= 10;
			}
			break;
		case DELAY_INSPECTION_2:	//个位
			if((S_Delay_Inspection-1) >= 0){
				S_Delay_Inspection -= 1;
			}
			break;
		}
		LCD12864_Display_Delay_Inspection_Refresh(&S_Delay_Inspection,&S_Coordinate);
		break;
	case K_4_num:	//
		if(Now_page == Delay_Inspection_page){
			printf("now_page == Delay_Inspection_page\r\n");
			//返回菜单界面
			LCD12864_Display(0x00,0x00);//清屏
			LCD12864_Display_Menu(NULL);
			S_Delay_Inspection = G_Delay_Inspection;
			Mark = 0;
			Now_page = Menu_Page;
			S_Coordinate=0;
			vTaskResume(Start_LCD12864_Menu_TaskHandle);//恢复挂起
			G_Delete_Task_struct.D_Task = Start_LCD12864_Delay_Inspection_TaskHandle;
			Start_LCD12864_Delay_Inspection_TaskHandle = NULL;
			G_Delete_Task_struct.sign = ENABLE;
		}
		break;
	default:
		break;
	}
}

void Start_LCD12864_Delay_Inspection_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;
	LCD12864_Display(0x00,0x00);//清屏
	LCD12864_Display_Delay_Inspection_Page(NULL);//第一次进入显示全部菜单内容
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,	//2 等待按键信息
				(uint32_t)0xffffffff,
				(uint32_t*)&NotifyValue,
				(TickType_t)portMAX_DELAY);
		if(err==pdTRUE)
		{
			LCD12864_Display_Delay_Inspection_Page((uint8_t)NotifyValue);
		}
	}
}

