/*
 * LCD12864_Display_Menu_Set_Strategy.c
 *
 *  Created on: 2018年10月16日
 *      Author: YuanWP
 */

#include "LCD12864_Display_Menu_Set_Strategy.h"
#include "button.h"
osThreadId Start_LCD12864_Set_Strategy_TaskHandle;

Strategy_struct G_Strategy[8];	//8条策略

void LCD12864_Set_Strategy_Menu(uint8_t key)
{
	static Line_Num Strategy_this_line = Line1;
	/*
	 * 策略x 从24c02中读取数据
	 * */
	switch(key){
	case NULL:
		if(Strategy_this_line <= Line4)
		{
			LCD12864_Set_Strategy_Line1(&Strategy_this_line);
			LCD12864_Set_Strategy_Line2(&Strategy_this_line);
			LCD12864_Set_Strategy_Line3(&Strategy_this_line);
			LCD12864_Set_Strategy_Line4(&Strategy_this_line);
		}else if(Strategy_this_line >= Line5){
			LCD12864_Set_Strategy_Line5(&Strategy_this_line);
			LCD12864_Set_Strategy_Line6(&Strategy_this_line);
			LCD12864_Set_Strategy_Line7(&Strategy_this_line);
			LCD12864_Set_Strategy_Line8(&Strategy_this_line);
		}
		break;
	case K_1_num://功能按键
		if(Start_LCD12864_Set_Strategy_Info_TaskHandle == NULL)
		{
			if(Strategy_this_line>Line1)
			{
				if(G_Strategy[Strategy_this_line-1].Strategy_State == Strategy_Open)
				{
					osThreadDef(LCD12864_Set_Strategy_Info_Task, Start_LCD12864_Set_Strategy_Info_Task, osPriorityNormal, 0, 128);//创建任务设置模块
					Start_LCD12864_Set_Strategy_Info_TaskHandle = osThreadCreate(osThread(LCD12864_Set_Strategy_Info_Task), &Strategy_this_line);
					Now_page = Strategy_Menu_Info_page;
					vTaskSuspend( Start_LCD12864_Set_Strategy_TaskHandle );		//挂起lcd12864显示主菜单的进程
				}
			}else{
				osThreadDef(LCD12864_Set_Strategy_Info_Task, Start_LCD12864_Set_Strategy_Info_Task, osPriorityNormal, 0, 128);//创建任务设置模块
				Start_LCD12864_Set_Strategy_Info_TaskHandle = osThreadCreate(osThread(LCD12864_Set_Strategy_Info_Task), &Strategy_this_line);
				Now_page = Strategy_Menu_Info_page;
				vTaskSuspend( Start_LCD12864_Set_Strategy_TaskHandle );		//挂起lcd12864显示主菜单的进程
			}
		}
		break;
	case K_2_num://向上按键
		if(Strategy_this_line>Line1)//第二个判断表示当前在上下按键无效操作的菜单选项下
		{
			LCD12864_Display_Set_Strategy_Refresh(&Strategy_this_line,SUB_LINE);//减一行
		}
		break;
	case K_3_num://向下按键
		if(Strategy_this_line<Line8)//第二个判断表示当前在上下按键无效操作的菜单选项下
		{
			LCD12864_Display_Set_Strategy_Refresh(&Strategy_this_line,ADD_LINE);//加一行
		}
		break;
	case K_4_num://取消按键
		if(Now_page == Strategy_Menu){
			//返回菜单界面
			LCD12864_Display(0x00,0x00);//清屏
			LCD12864_Display_Menu(NULL);
			Now_page = Menu_Page;
			Strategy_this_line = Line1;
			vTaskResume(Start_LCD12864_Menu_TaskHandle);//恢复挂起

			G_Delete_Task_struct.D_Task = Start_LCD12864_Set_Strategy_TaskHandle;
			Start_LCD12864_Set_Strategy_TaskHandle = NULL;
			G_Delete_Task_struct.sign = ENABLE;
		}
		break;
	}
}

void LCD12864_Display_Set_Strategy_Refresh(Line_Num* p_Strategy_this_line,uint8_t opt)
{
	switch(*p_Strategy_this_line){
	case Line1:
		if(opt==ADD_LINE)//只能加不能减 因为是最上一行
		{
			(*p_Strategy_this_line)++;
			LCD12864_Set_Strategy_Line1(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line2(p_Strategy_this_line);
		}
		break;
	case Line2:
		if(opt==ADD_LINE)
		{
			(*p_Strategy_this_line)++;
			LCD12864_Set_Strategy_Line2(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line3(p_Strategy_this_line);
		}else if(opt==SUB_LINE){
			(*p_Strategy_this_line)--;
			LCD12864_Set_Strategy_Line2(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line1(p_Strategy_this_line);
		}
		break;
	case Line3:
		if(opt==ADD_LINE)
		{
			(*p_Strategy_this_line)++;
			LCD12864_Set_Strategy_Line3(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line4(p_Strategy_this_line);
		}else if(opt==SUB_LINE){
			(*p_Strategy_this_line)--;
			LCD12864_Set_Strategy_Line3(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line2(p_Strategy_this_line);
		}
		break;
	case Line4:
		if(opt==ADD_LINE)
		{
			(*p_Strategy_this_line)++;
			LCD12864_Set_Strategy_Line5(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line6(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line7(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line8(p_Strategy_this_line);
		}else if(opt==SUB_LINE){
			(*p_Strategy_this_line)--;
			LCD12864_Set_Strategy_Line4(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line3(p_Strategy_this_line);
		}
		break;
	case Line5:
		if(opt==ADD_LINE)
		{
			(*p_Strategy_this_line)++;
			LCD12864_Set_Strategy_Line5(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line6(p_Strategy_this_line);
		}else if(opt==SUB_LINE){
			(*p_Strategy_this_line)--;
			LCD12864_Set_Strategy_Line1(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line2(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line3(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line4(p_Strategy_this_line);
		}
		break;
	case Line6:
		if(opt==ADD_LINE)
		{
			(*p_Strategy_this_line)++;
			LCD12864_Set_Strategy_Line6(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line7(p_Strategy_this_line);
		}else if(opt==SUB_LINE){
			(*p_Strategy_this_line)--;
			LCD12864_Set_Strategy_Line6(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line5(p_Strategy_this_line);
		}
		break;
	case Line7:
		if(opt==ADD_LINE)
		{
			(*p_Strategy_this_line)++;
			LCD12864_Set_Strategy_Line7(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line8(p_Strategy_this_line);
		}else if(opt==SUB_LINE){
			(*p_Strategy_this_line)--;
			LCD12864_Set_Strategy_Line7(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line6(p_Strategy_this_line);
		}
		break;
	case Line8:
		if(opt==ADD_LINE)
		{
		}else if(opt==SUB_LINE){
			(*p_Strategy_this_line)--;
			LCD12864_Set_Strategy_Line8(p_Strategy_this_line);
			LCD12864_Set_Strategy_Line7(p_Strategy_this_line);
		}
		break;
	}
}
/*
 * 策略设置任务操作
 * */
void Start_LCD12864_Set_Strategy_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;
	LCD12864_Display(0x00,0x00);//清空界面
	LCD12864_Set_Strategy_Menu(NULL);//初始化策略是指界面
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,			//2 等待按键信息
			(uint32_t)0xffffffff,
			(uint32_t*)&NotifyValue,
			(TickType_t)portMAX_DELAY);
		if(err==pdTRUE)
		{
			LCD12864_Set_Strategy_Menu((uint8_t)NotifyValue);
		}

	}
}

void LCD12864_Set_Strategy_Line(uint8_t row,uint8_t list_num,Line_Num *p_Strategy_this_line,Line_Num This_Line_Num)
{
	uint8_t color;
	char Hours[3];
	char Minutes[3];

	if(*p_Strategy_this_line == This_Line_Num){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	sprintf(Hours,"%02d",G_Strategy[list_num].Time.Hours);
	sprintf(Minutes,"%02d",G_Strategy[list_num].Time.Minutes);
	LCD12864_Put_8_16_Char(row,0x04,list_num+0x31,color);
	LCD12864_Put_8_16_Char(row,0x0C,'.',color);
	LCD12864_Put_8_16_Char(row,0x14,Hours[0],color);
	LCD12864_Put_8_16_Char(row,0x1c,Hours[1],color);
	LCD12864_Put_8_16_Char(row,0x24,':',color);
	LCD12864_Put_8_16_Char(row,0x2c,Minutes[0],color);
	LCD12864_Put_8_16_Char(row,0x34,Minutes[1],color);
	LCD12864_Put_8_16_Char(row,0x3c,' ',color);
	if(G_Strategy[list_num].Strategy_Type == Strategy_Type_Route){
		LCD12864_Put_16_16_Char(row,0x44,0x22,color);//风字
		LCD12864_Put_16_16_Char(row,0x54,0x23,color);//口字
	}else if(G_Strategy[list_num].Strategy_Type == Strategy_Type_Temp){
		LCD12864_Put_16_16_Char(row,0x44,0x0A,color);//温字
		LCD12864_Put_16_16_Char(row,0x54,0x0B,color);//度字
	}
	LCD12864_Put_8_16_Char(row,0x64,' ',color);
	if(G_Strategy[list_num].Strategy_State == Strategy_Open){
		LCD12864_Put_16_16_Char(row,0x6C,0x24,color);//开字
	}else if(G_Strategy[list_num].Strategy_State == Strategy_Close){
		LCD12864_Put_16_16_Char(row,0x6C,0x25,color);//关字
	}
}
void LCD12864_Set_Strategy_Line1(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB7,0,p_Strategy_this_line,Line1);
}
void LCD12864_Set_Strategy_Line2(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB5,1,p_Strategy_this_line,Line2);
}
void LCD12864_Set_Strategy_Line3(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB3,2,p_Strategy_this_line,Line3);
}
void LCD12864_Set_Strategy_Line4(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB1,3,p_Strategy_this_line,Line4);
}
void LCD12864_Set_Strategy_Line5(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB7,4,p_Strategy_this_line,Line5);
}
void LCD12864_Set_Strategy_Line6(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB5,5,p_Strategy_this_line,Line6);
}
void LCD12864_Set_Strategy_Line7(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB3,6,p_Strategy_this_line,Line7);
}
void LCD12864_Set_Strategy_Line8(Line_Num *p_Strategy_this_line)
{
	LCD12864_Set_Strategy_Line(0xB1,7,p_Strategy_this_line,Line8);
}










