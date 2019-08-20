/*
 * LCD12864_Display_Menu.c
 *
 *  Created on: 2018年9月13日
 *      Author: YuanWP
 */
#include "LCD12864_Display_Menu.h"
#include "button.h"
#include "HUMI.h"
#include "relay.h"
#include "string.h"
#include "LCD12864.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"
#include "LCD12864_Display_Menu_Route_Reset.h"
#include "LCD12864_Display_Menu_Stroking_Cycle.h"
#include "LCD12864_Display_Menu_Set_Strategy_Info.h"
extern	GPRS_TypeDef						*p_GPRS;
//extern uint8_t G_Sim800C_Signal;
/*
 * 点击选择后比如行程归零点击进入后将行程归零四个字改成白底
 * */
void LCD12864_Display_Menu_Typebase_opt(uint8_t opt,option* p_this_line)
{
	uint8_t* p_temp = Chinese_Menu_Type;
	uint8_t row,col,i,place,row_place = 0xB0;
	p_temp = p_temp + ((*p_this_line)*4*32);
	for(place = 0;place < 4;place++)		//多少个字
	{
		for(row = row_place+1;row >= row_place;row--)	//以8个点为一个行 一个字用多少行显示
		WriteCommand(0x17-place);		//Set column address (MSB)//纵列的开始地址
		WriteCommand(0x00);				//Set column address (LSB)
		for(i = 0;i < 2;i++)			//(64/8)
		{
			for(col=0;col<8;col++)
			{
				if(opt == BLACK_BASE)	//黑底
				{
					Write_Data(~(*p_temp++));
				}else if(opt == WHITE_BASE)	//白底
				{
					Write_Data(*p_temp++);
				}
			}
		}
	}
	printf("行程归零操作 将行程操作变回白底 !!!\r\n");
}

/*
 * 功能键操作
 */
void Dispose_Function_Key(option* p_this_line,Item_Menu* p_now_opt_select)
{
	switch(*p_this_line)
	{
	case Set_Strategy:		//策略设置
		if(Start_LCD12864_Set_Strategy_TaskHandle == NULL)
		{
		  osThreadDef(LCD12864_Set_Strategy_Task, Start_LCD12864_Set_Strategy_Task, osPriorityNormal, 0, 128);//创建策略菜单模块
		  Start_LCD12864_Set_Strategy_TaskHandle = osThreadCreate(osThread(LCD12864_Set_Strategy_Task), NULL);
		  Now_page = Strategy_Menu;
		  vTaskSuspend( Start_LCD12864_Menu_TaskHandle );		//挂起lcd12864显示主菜单的进程
		}
		break;
	case Route_Reset:		//行程归零
		if(Start_LCD12864_Route_Reset_TaskHandle == NULL)
		{
		  osThreadDef(LCD12864_Route_Reset_Task, Start_LCD12864_Route_Reset_Task, osPriorityNormal, 0, 128);//创建行程归零
		  Start_LCD12864_Route_Reset_TaskHandle = osThreadCreate(osThread(LCD12864_Route_Reset_Task), NULL);
		  Now_page = Route_Reset_page;
		  vTaskSuspend( Start_LCD12864_Menu_TaskHandle );		//挂起lcd12864显示主菜单的进程
		}
		break;
	case Max_Route:
		if(Start_LCD12864_Max_Route_TaskHandle == NULL)
		{
		  osThreadDef(LCD12864_Max_Route_Task, Start_LCD12864_Max_Route_Task, osPriorityNormal, 0, 128);//创建最大行程
		  Start_LCD12864_Max_Route_TaskHandle = osThreadCreate(osThread(LCD12864_Max_Route_Task), NULL);
		  Now_page = Max_Route_page;
		  vTaskSuspend( Start_LCD12864_Menu_TaskHandle );		//挂起lcd12864显示主菜单的进程
		}
		break;
	case HUMI_Compensation:
		if(Start_LCD12864_HUMI_Compensation_TaskHandle == NULL)
		{
		  osThreadDef(LCD12864_HUMI_Compensation_Task, Start_LCD12864_HUMI_Compensation_Task, osPriorityNormal, 0, 128);//创建最大行程
		  Start_LCD12864_HUMI_Compensation_TaskHandle = osThreadCreate(osThread(LCD12864_HUMI_Compensation_Task), NULL);
		  Now_page = HUMI_Compensation_page;
		  vTaskSuspend( Start_LCD12864_Menu_TaskHandle );		//挂起lcd12864显示主菜单的进程
		}
		break;
	case Delay_inspection:
		if(Start_LCD12864_Delay_Inspection_TaskHandle == NULL)
		{
		  osThreadDef(LCD12864_Delay_Inspection_Task, Start_LCD12864_Delay_Inspection_Task, osPriorityNormal, 0, 128);//创建最大行程
		  Start_LCD12864_Delay_Inspection_TaskHandle = osThreadCreate(osThread(LCD12864_Delay_Inspection_Task), NULL);
		  Now_page = Delay_Inspection_page;
		  vTaskSuspend( Start_LCD12864_Menu_TaskHandle );		//挂起lcd12864显示主菜单的进程
		}
		break;
	case Set_Time:
		if(Start_LCD12864_Set_Time_TaskHandle == NULL)
		{
		  osThreadDef(LCD12864_Set_Time_Task, Start_LCD12864_Set_Time_Task, osPriorityNormal, 0, 128);//创建最大行程
		  Start_LCD12864_Set_Time_TaskHandle = osThreadCreate(osThread(LCD12864_Set_Time_Task), NULL);
		  Now_page = Set_Time_page;
		  vTaskSuspend( Start_LCD12864_Menu_TaskHandle );		//挂起lcd12864显示主菜单的进程
		}
		break;
	case Stroking_Cycle:
		if(Start_LCD12864_Stroking_Cycle_TaskHandle == NULL)
		{
		  osThreadDef(LCD12864_Stroking_Cycle_Task, Start_LCD12864_Stroking_Cycle_Task, osPriorityNormal, 0, 128);
		  Start_LCD12864_Stroking_Cycle_TaskHandle = osThreadCreate(osThread(LCD12864_Stroking_Cycle_Task), NULL);
		  Now_page = Stroking_Cycle_page;
		  vTaskSuspend( Start_LCD12864_Menu_TaskHandle );		//挂起lcd12864显示主菜单的进程
		}
		break;
	case Version_Information:
		break;
	}
}

/*
 * 目的显示全部的选择菜单
 * */
void LCD12864_Key_Null_Opt(option* p_this_line,uint8_t* p_page,Item_Menu* p_now_opt_select)
{
	if(*p_this_line < Delay_inspection)
	{
		LCD12864_Display_Set_Strategy(p_this_line);		//策略设置框
		LCD12864_Display_Route_Reset(p_this_line);			//
		LCD12864_Display_Max_Route(p_this_line);			//
		LCD12864_Display_HUMI_Compensation(p_this_line);
	}else{
		LCD12864_Display_Delay_inspection(p_this_line);
		LCD12864_Display_Set_Time(p_this_line);
		LCD12864_Display_Stroking_Cycle(p_this_line);
		LCD12864_Display_Version_Information(p_this_line);
	}
}

void LCD12864_Display_Menu_Option_Refresh(option* p_this_line,uint8_t opt)
{
//	Set_Strategy = 0,	//策略设置
//	Route_Reset,		//行程归零
//	Max_Route,			//最大行程
//	HUMI_Compensation,	//温度补偿
//	Delay_inspection,	//延时检查
//	Set_Time,			//时间校准
//	Stroking_Cycle,		//行程周期
//	Version_Information,//版本信息
	switch(*p_this_line){
	case Set_Strategy:	//当前在策略设置不能减了
		if(opt==ADD_LINE)
		{
			(*p_this_line)++;
			LCD12864_Display_Set_Strategy(p_this_line);
			LCD12864_Display_Route_Reset(p_this_line);
		}
		break;
	case Route_Reset:
		if(opt==ADD_LINE)
		{
			(*p_this_line)++;
			LCD12864_Display_Route_Reset(p_this_line);
			LCD12864_Display_Max_Route(p_this_line);
		}else if(opt==SUB_LINE){
			(*p_this_line)--;
			LCD12864_Display_Set_Strategy(p_this_line);
			LCD12864_Display_Route_Reset(p_this_line);
		}
		break;
	case Max_Route:
		if(opt==ADD_LINE)
		{
			(*p_this_line)++;
			LCD12864_Display_Max_Route(p_this_line);
			LCD12864_Display_HUMI_Compensation(p_this_line);
		}else if(opt==SUB_LINE){
			(*p_this_line)--;
			LCD12864_Display_Route_Reset(p_this_line);
			LCD12864_Display_Max_Route(p_this_line);
		}
		break;
	case HUMI_Compensation:
		if(opt==ADD_LINE)
		{//页面需要跳转 下一页
//			(*p_this_line)++;
//			LCD12864_Display_Max_Route(p_this_line);
//			LCD12864_Display_HUMI_Compensation(p_this_line);
			(*p_this_line)++;
			LCD12864_Display_Delay_inspection(p_this_line);
			LCD12864_Display_Set_Time(p_this_line);
			LCD12864_Display_Stroking_Cycle(p_this_line);
			LCD12864_Display_Version_Information(p_this_line);

		}else if(opt==SUB_LINE){
			(*p_this_line)--;
			LCD12864_Display_Max_Route(p_this_line);
			LCD12864_Display_HUMI_Compensation(p_this_line);
		}
		break;
	case Delay_inspection:
		if(opt==ADD_LINE)
		{
			(*p_this_line)++;
			LCD12864_Display_Delay_inspection(p_this_line);
			LCD12864_Display_Set_Time(p_this_line);
		}else if(opt==SUB_LINE){//页面需要跳转 上一页
			(*p_this_line)--;
			LCD12864_Display_Set_Strategy(p_this_line);		//策略设置框
			LCD12864_Display_Route_Reset(p_this_line);			//
			LCD12864_Display_Max_Route(p_this_line);			//
			LCD12864_Display_HUMI_Compensation(p_this_line);
		}
		break;
	case Set_Time:
		if(opt==ADD_LINE)
		{
			(*p_this_line)++;
			LCD12864_Display_Set_Time(p_this_line);
			LCD12864_Display_Stroking_Cycle(p_this_line);
		}else if(opt==SUB_LINE){
			(*p_this_line)--;
			LCD12864_Display_Set_Time(p_this_line);
			LCD12864_Display_Delay_inspection(p_this_line);
		}
		break;
	case Stroking_Cycle:
		if(opt==ADD_LINE)
		{
			(*p_this_line)++;
			LCD12864_Display_Stroking_Cycle(p_this_line);
			LCD12864_Display_Version_Information(p_this_line);
		}else if(opt==SUB_LINE){
			(*p_this_line)--;
			LCD12864_Display_Stroking_Cycle(p_this_line);
			LCD12864_Display_Set_Time(p_this_line);
		}
		break;
	case Version_Information:
		if(opt==ADD_LINE)
		{
		}else if(opt==SUB_LINE){
			(*p_this_line)--;
			LCD12864_Display_Version_Information(p_this_line);
			LCD12864_Display_Stroking_Cycle(p_this_line);
		}
		break;

	}
}

void LCD12864_Display_Menu(uint8_t key)
{
//	osThreadId Old_Start_LCD12864_Menu_TaskHandle;
	static option this_line = Set_Strategy;
	static uint8_t page = 0;
	static Item_Menu now_opt_select = At_Null;
	switch(key)
	{
	case NULL:	//第一次进入显示全部菜单内容
		LCD12864_Key_Null_Opt(&this_line,&page,&now_opt_select);
		break;
	case K_1_num://选择按键
		Dispose_Function_Key(&this_line,&now_opt_select);
		break;
	case K_2_num:	//向上 this_line   this_line =当前已经加深的行
		if(this_line>Set_Strategy)//第二个判断表示当前在上下按键无效操作的菜单选项下
		{
			LCD12864_Display_Menu_Option_Refresh(&this_line,SUB_LINE);//减一行
		}
		break;
	case K_3_num:	//向下 this_line+1   this_line =当前已经加深的行
		if(this_line<Version_Information)//第二个判断表示当前在上下按键无效操作的菜单选项下
		{
			LCD12864_Display_Menu_Option_Refresh(&this_line,ADD_LINE);//加一行
		}
		break;
	case K_4_num:	//向下 this_line+1   this_line =当前已经加深的行
		if(now_opt_select == At_Null)//表示在主菜单下，按下返回按键表示要回到显示常规温度界面 (复原主界面)
		{
			this_line = Set_Strategy;
			page = 0;
			LCD12864_Display(0x00,0x00);
			LCD12864_Put_Str_At_Present_Mode(G_New_Mode);
			LCD12864_Put_Signel_Sim(p_GPRS->CSQ/*G_Sim800C_Signal*/);
//			LCD12864_Put_Signel_Wifi();
			LCD12864_Put_Motor_Situation(1);
			LCD12864_Put_Open_Mouth_And_Time();
			vTaskResume(Start_LCD12864_HUMI_TaskHandle);//恢复lcd12864显示温度的进程
			//需要更改
			G_Delete_Task_struct.sign = ENABLE;
			G_Delete_Task_struct.D_Task = Start_LCD12864_Menu_TaskHandle;
			Start_LCD12864_Menu_TaskHandle = NULL;
		}
		break;
	default:

		break;
	}
}


/*
 * 显示菜单任务
 * 	行程归零  复位
 * 	最大行程  ___
 * 	温度补偿  ___
 * 	延时检查  ___
 */
void Start_LCD12864_Display_Menu_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;
	LCD12864_Display(0x00,0x00);//清屏
	LCD12864_Display_Menu(NULL);//第一次进入显示全部菜单内容
	Now_page = Menu_Page;
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,			//2 等待按键信息
							(uint32_t)0xffffffff,
							(uint32_t*)&NotifyValue,
							(TickType_t)portMAX_DELAY);
		Now_page = Menu_Page;

		if(err==pdTRUE)
		{
			LCD12864_Display_Menu((uint8_t)NotifyValue);
		}
	}
}


/*
 * 1.策略设置
 */
void LCD12864_Display_Set_Strategy(option* this_line)
{
	uint8_t color;
	if(*this_line == Set_Strategy){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB7,0x04,'1',color);
	LCD12864_Put_8_16_Char(0xB7,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB7,0x14,0x00,color);//策字
	LCD12864_Put_16_16_Char(0xB7,0x24,0x01,color);//略字
	LCD12864_Put_16_16_Char(0xB7,0x34,0x02,color);//设字
	LCD12864_Put_16_16_Char(0xB7,0x44,0x03,color);//置字
}
/*
 * 2.行程归零
 */
void LCD12864_Display_Route_Reset(option* this_line)
{
	uint8_t color;
	if(*this_line == Route_Reset){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB5,0x04,'2',color);
	LCD12864_Put_8_16_Char(0xB5,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB5,0x14,0x04,color);//行字
	LCD12864_Put_16_16_Char(0xB5,0x24,0x05,color);//程字
	LCD12864_Put_16_16_Char(0xB5,0x34,0x06,color);//归字
	LCD12864_Put_16_16_Char(0xB5,0x44,0x07,color);//零字
}
/*
 * 3.最大行程
 */
void LCD12864_Display_Max_Route(option* this_line)
{
	uint8_t color;
	if(*this_line == Max_Route){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB3,0x04,'3',color);
	LCD12864_Put_8_16_Char(0xB3,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB3,0x14,0x08,color);//最字
	LCD12864_Put_16_16_Char(0xB3,0x24,0x09,color);//大字
	LCD12864_Put_16_16_Char(0xB3,0x34,0x04,color);//行字
	LCD12864_Put_16_16_Char(0xB3,0x44,0x05,color);//程字
}
/*
 * 4.温度补偿
 */
void LCD12864_Display_HUMI_Compensation(option* this_line)
{
	uint8_t color;
	if(*this_line == HUMI_Compensation){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB1,0x04,'4',color);
	LCD12864_Put_8_16_Char(0xB1,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB1,0x14,0x0A,color);//温字
	LCD12864_Put_16_16_Char(0xB1,0x24,0x0B,color);//度字
	LCD12864_Put_16_16_Char(0xB1,0x34,0x0C,color);//补字
	LCD12864_Put_16_16_Char(0xB1,0x44,0x0D,color);//偿字
}
/*
 * 5.延时检查
 */
void LCD12864_Display_Delay_inspection(option* this_line)
{
	uint8_t color;
	if(*this_line == Delay_inspection){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB7,0x04,'5',color);
	LCD12864_Put_8_16_Char(0xB7,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB7,0x14,0x0E,color);//延字
	LCD12864_Put_16_16_Char(0xB7,0x24,0x0F,color);//时字
	LCD12864_Put_16_16_Char(0xB7,0x34,0x10,color);//检字
	LCD12864_Put_16_16_Char(0xB7,0x44,0x11,color);//查字
}
/*
 * 6.时间校准
 */
void LCD12864_Display_Set_Time(option* this_line)
{
	uint8_t color;
	if(*this_line == Set_Time){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB5,0x04,'6',color);
	LCD12864_Put_8_16_Char(0xB5,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB5,0x14,0x0F,color);//时字
	LCD12864_Put_16_16_Char(0xB5,0x24,0x12,color);//间字
	LCD12864_Put_16_16_Char(0xB5,0x34,0x13,color);//校字
	LCD12864_Put_16_16_Char(0xB5,0x44,0x14,color);//准字
}
/*
 * 7.行程周期
 */
void LCD12864_Display_Stroking_Cycle(option* this_line)
{
	uint8_t color;
	if(*this_line == Stroking_Cycle){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB3,0x04,'7',color);
	LCD12864_Put_8_16_Char(0xB3,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB3,0x14,0x04,color);//行字
	LCD12864_Put_16_16_Char(0xB3,0x24,0x05,color);//程字
	LCD12864_Put_16_16_Char(0xB3,0x34,0x15,color);//周字
	LCD12864_Put_16_16_Char(0xB3,0x44,0x16,color);//期字
}
/*
 * 8.版本信息
 */
void LCD12864_Display_Version_Information(option* this_line)
{
	uint8_t color;
	if(*this_line == Version_Information){
		color = BLACK_BASE;
	}else{
		color = WHITE_BASE;
	}
	LCD12864_Put_8_16_Char(0xB1,0x04,'8',color);
	LCD12864_Put_8_16_Char(0xB1,0x0C,'.',color);
	LCD12864_Put_16_16_Char(0xB1,0x14,0x17,color);//版字
	LCD12864_Put_16_16_Char(0xB1,0x24,0x18,color);//本字
	LCD12864_Put_16_16_Char(0xB1,0x34,0x19,color);//信字
	LCD12864_Put_16_16_Char(0xB1,0x44,0x1A,color);//息字
}







