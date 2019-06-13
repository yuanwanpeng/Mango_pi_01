/*
 * LCD12864_Display_Menu_Set_Strategy_Info.c
 *
 *  Created on: 2018年11月7日
 *      Author: YuanWP
 */
#include "LCD12864_Display_Menu_Set_Strategy_Info.h"
#include "LCD12864_Display_Menu_Set_Strategy.h"
#include "string.h"
#include "Strategy.h"

Strategy_struct S_Strategy[8];		//8条策略缓存用
osThreadId Start_LCD12864_Set_Strategy_Info_TaskHandle;	//更改策略信息句柄


void LCD12864_Set_Strategy_Info_Refresh(uint8_t* p_Place,Line_Num* p_Line_Num)
{
	LCD12864_Set_Strategy_Info_Line1(p_Place,p_Line_Num);//显示line1~line4
	LCD12864_Set_Strategy_Info_Line2(p_Place,p_Line_Num);
	LCD12864_Set_Strategy_Info_Line3(p_Place,p_Line_Num);
	LCD12864_Set_Strategy_Info_Line4(p_Place,p_Line_Num);
}
/*
 * 向上按键按下在策略信息设置界面
 */
void LCD12864_Set_Strategy_Info_Up_Key_Opt(uint8_t* p_Place,Line_Num* p_Line_Num)
{
	switch(*p_Place){
	case STRATEGY_INFO_SWITCH:	//当前在选择
		if(S_Strategy[(*p_Line_Num)].Strategy_State == Strategy_Open)
		{
			S_Strategy[(*p_Line_Num)].Strategy_State = Strategy_Close;
		}else{
			S_Strategy[(*p_Line_Num)].Strategy_State = Strategy_Open;
		}
		break;
	case STRATEGY_INFO_STANDARD://当前在风口还是温度
		if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Route)
		{
			S_Strategy[(*p_Line_Num)].Strategy_Type = Strategy_Type_Temp;
		}else{
			S_Strategy[(*p_Line_Num)].Strategy_Type = Strategy_Type_Route;
		}
		break;
	case STRATEGY_INFO_TIME_HOURS://当前在小时
		if(S_Strategy[(*p_Line_Num)].Time.Hours < 23)
		{
			S_Strategy[(*p_Line_Num)].Time.Hours += 1;
		}
		break;
	case STRATEGY_INFO_TIME_MINUTES://当前在分钟
		if(S_Strategy[(*p_Line_Num)].Time.Minutes < 59)
		{
			S_Strategy[(*p_Line_Num)].Time.Minutes += 1;
		}
		break;
	case STRATEGY_INFO_ROUTE_4://行程的千位
		if((S_Strategy[(*p_Line_Num)].Route/1000) < 9)
		{
			S_Strategy[(*p_Line_Num)].Route += 1000;
		}
		break;
	case STRATEGY_INFO_ROUTE_3://行程的百位
		if((S_Strategy[(*p_Line_Num)].Route%1000/100) < 9)
		{
			S_Strategy[(*p_Line_Num)].Route += 100;
		}
		break;
	case STRATEGY_INFO_ROUTE_2://行程的十位
		if((S_Strategy[(*p_Line_Num)].Route%100/10) < 9)
		{
			S_Strategy[(*p_Line_Num)].Route += 10;
		}
		break;
	case STRATEGY_INFO_ROUTE_1://行程个位
		if((S_Strategy[(*p_Line_Num)].Route%10) < 9)
		{
			S_Strategy[(*p_Line_Num)].Route += 1;
		}
		break;
	case STRATEGY_INFO_MIN_TEMP://最小温度
		if((S_Strategy[(*p_Line_Num)].Temp_l) < 60)
		{
			S_Strategy[(*p_Line_Num)].Temp_l += 1;
			if((S_Strategy[(*p_Line_Num)].Temp_l+STRATEGY_INFO_TEMP_SECTION)>S_Strategy[(*p_Line_Num)].Temp_h)
			{//如果低温 大于高温的话
				S_Strategy[(*p_Line_Num)].Temp_h = S_Strategy[(*p_Line_Num)].Temp_l+STRATEGY_INFO_TEMP_SECTION;
			}
		}
		break;
	case STRATEGY_INFO_MAX_TEMP://最大
		if((S_Strategy[(*p_Line_Num)].Temp_h) < 63)
		{
			S_Strategy[(*p_Line_Num)].Temp_h += 1;
		}
		break;
	case STRATEGY_INFO_SET://设置确定
		//写入G_Strategy
		if(S_Strategy[(*p_Line_Num)].Strategy_State == Strategy_Close)//如果关闭了当前行
		{
			Init_Strategy((*p_Line_Num));//从当前行以后的写入
		}

		if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Route)
		{
			if(S_Strategy[(*p_Line_Num)].Route > G_Max_Route)//大于全局的最大行程
			{
				S_Strategy[(*p_Line_Num)].Route = G_Max_Route;//设置成最大行程
			}
		}

		memcpy((uint8_t*)G_Strategy,(uint8_t*)S_Strategy,sizeof(S_Strategy));//深拷贝

		AT24CXX_Write((*p_Line_Num)*0x10,((uint8_t*)S_Strategy)+(sizeof(S_Strategy[(*p_Line_Num)])*(*p_Line_Num)),sizeof(S_Strategy[(*p_Line_Num)]));
		LCD12864_Display_Success();	//显示成功
		osDelay(1500);
		LCD12864_Display_Clear_Success();
		break; 
	}
}
/*
 * 向下按键按下在策略信息设置界面
 * 注释同上
 */
void LCD12864_Set_Strategy_Info_Down_Key_Opt(uint8_t* p_Place,Line_Num* p_Line_Num)
{
	uint16_t Min_Time,UP_Min_Time;

	switch(*p_Place){
	case STRATEGY_INFO_SWITCH:
		if(S_Strategy[(*p_Line_Num)].Strategy_State == Strategy_Open)
		{
			S_Strategy[(*p_Line_Num)].Strategy_State = Strategy_Close;
		}else{
			S_Strategy[(*p_Line_Num)].Strategy_State = Strategy_Open;
		}
		break;
	case STRATEGY_INFO_STANDARD:
		if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Route)
		{
			S_Strategy[(*p_Line_Num)].Strategy_Type = Strategy_Type_Temp;
		}else{
			S_Strategy[(*p_Line_Num)].Strategy_Type = Strategy_Type_Route;
		}
		break;
	case STRATEGY_INFO_TIME_HOURS:
		if((*p_Line_Num) == 0)
		{
			if(S_Strategy[(*p_Line_Num)].Time.Hours > 4)
			{
				S_Strategy[(*p_Line_Num)].Time.Hours -= 1;
			}
		}else{
			Min_Time = (S_Strategy[(*p_Line_Num)].Time.Hours-1) * 60 + S_Strategy[(*p_Line_Num)].Time.Minutes;//计算出当前任务启动时间
			UP_Min_Time = (S_Strategy[((*p_Line_Num)-1)].Time.Hours) * 60 + S_Strategy[((*p_Line_Num)-1)].Time.Minutes;//下一个任务启动时间
			//计算时间
			if((S_Strategy[(*p_Line_Num)].Time.Hours > 4)&&(UP_Min_Time<Min_Time))//这一个任务启动时间要大于上一个任务启动时间
			{
				S_Strategy[(*p_Line_Num)].Time.Hours -= 1;
			}
		}
		break;
	case STRATEGY_INFO_TIME_MINUTES:
		if((*p_Line_Num) == 0)
		{
			if(S_Strategy[(*p_Line_Num)].Time.Minutes > 0)
			{
				S_Strategy[(*p_Line_Num)].Time.Minutes -= 1;
			}
		}else{
			Min_Time = (S_Strategy[(*p_Line_Num)].Time.Hours) * 60 + S_Strategy[(*p_Line_Num)].Time.Minutes;//计算出当前任务启动时间
			UP_Min_Time = (S_Strategy[((*p_Line_Num)-1)].Time.Hours) * 60 + S_Strategy[((*p_Line_Num)-1)].Time.Minutes;//上一个任务启动时间
			if((S_Strategy[(*p_Line_Num)].Time.Minutes > 0)&&(UP_Min_Time<Min_Time))
			{
				S_Strategy[(*p_Line_Num)].Time.Minutes -= 1;
			}
		}
		break;
	case STRATEGY_INFO_ROUTE_4:
		if((S_Strategy[(*p_Line_Num)].Route/1000) > 0)
		{
			S_Strategy[(*p_Line_Num)].Route -= 1000;
		}
		break;
	case STRATEGY_INFO_ROUTE_3:
		if((S_Strategy[(*p_Line_Num)].Route%1000/100) > 0)
		{
			S_Strategy[(*p_Line_Num)].Route -= 100;
		}
		break;
	case STRATEGY_INFO_ROUTE_2:
		if((S_Strategy[(*p_Line_Num)].Route%100/10) > 0)
		{
			S_Strategy[(*p_Line_Num)].Route -= 10;
		}
		break;
	case STRATEGY_INFO_ROUTE_1:
		if((S_Strategy[(*p_Line_Num)].Route%10) > 0)
		{
			S_Strategy[(*p_Line_Num)].Route -= 1;
		}
	case STRATEGY_INFO_MIN_TEMP:
		if((S_Strategy[(*p_Line_Num)].Temp_l) > 0)
		{
			S_Strategy[(*p_Line_Num)].Temp_l -= 1;
		}
		break;
	case STRATEGY_INFO_MAX_TEMP:
		if((S_Strategy[(*p_Line_Num)].Temp_h)>STRATEGY_INFO_TEMP_SECTION)
		{
			if((S_Strategy[(*p_Line_Num)].Temp_h)>(S_Strategy[(*p_Line_Num)].Temp_l+STRATEGY_INFO_TEMP_SECTION))//温差必须大于3度
			{
				S_Strategy[(*p_Line_Num)].Temp_h -= 1;
			}
		}
		break;
	case STRATEGY_INFO_SET:

		break;
	}
}
/*
 * 初始化设置策略信息
 */
void LCD12864_Set_Strategy_Info_Page(uint8_t key,Line_Num* p_Line_Num)
{
	static uint8_t S_Place = STRATEGY_INFO_SWITCH;//当前哪个位置设置策略信息内容
	/*
	 * 策略x 从24c02中读取数据
	 * */
	switch(key){
	case NULL:
		LCD12864_Set_Strategy_Info_Line1(&S_Place,p_Line_Num);
		LCD12864_Set_Strategy_Info_Line2(&S_Place,p_Line_Num);
		LCD12864_Set_Strategy_Info_Line3(&S_Place,p_Line_Num);
		LCD12864_Set_Strategy_Info_Line4(&S_Place,p_Line_Num);
		break;
	case K_1_num://功能按键
		switch(S_Place){
		case STRATEGY_INFO_SWITCH://在开启关闭位置
			S_Place++;
			break;
		case STRATEGY_INFO_STANDARD://在基准位置
			S_Place++;
			break;
		case STRATEGY_INFO_TIME_HOURS://在时间小时位置
			S_Place++;
			break;
		case STRATEGY_INFO_TIME_MINUTES://在时间分钟位置
			if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Route)
			{
				S_Place = STRATEGY_INFO_ROUTE_4;
			}else if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Temp){
				S_Place = STRATEGY_INFO_MIN_TEMP;
			}
			break;
		case STRATEGY_INFO_ROUTE_4://在千位位置
			S_Place++;
			break;
		case STRATEGY_INFO_ROUTE_3://在百位位置
			S_Place++;
			break;
		case STRATEGY_INFO_ROUTE_2://在十位位置
			S_Place++;
			break;
		case STRATEGY_INFO_ROUTE_1://在个位位置
			S_Place = STRATEGY_INFO_SET;
			break;
		case STRATEGY_INFO_MIN_TEMP:
			S_Place = STRATEGY_INFO_MAX_TEMP;
			break;
		case STRATEGY_INFO_MAX_TEMP:
			S_Place = STRATEGY_INFO_SET;
			break;
		case STRATEGY_INFO_SET://在开启关闭位置
			S_Place = STRATEGY_INFO_SWITCH;
			break;
		}
		LCD12864_Set_Strategy_Info_Refresh(&S_Place,p_Line_Num);
		break;
	case K_2_num://向上按键
		LCD12864_Set_Strategy_Info_Up_Key_Opt(&S_Place,p_Line_Num);
		LCD12864_Set_Strategy_Info_Refresh(&S_Place,p_Line_Num);
		break;
	case K_3_num://向下按键
		LCD12864_Set_Strategy_Info_Down_Key_Opt(&S_Place,p_Line_Num);
		LCD12864_Set_Strategy_Info_Refresh(&S_Place,p_Line_Num);
		break;
	case K_4_num://取消按键
		if(Now_page == Strategy_Menu_Info_page){
			//返回策略的菜单界面
			LCD12864_Display(0x00,0x00);//清屏
			LCD12864_Set_Strategy_Menu(NULL);
			memcpy((uint8_t*)S_Strategy,(uint8_t*)G_Strategy,sizeof(S_Strategy));
			Now_page = Strategy_Menu;

			//Strategy_this_line = Line1;
			S_Place = STRATEGY_INFO_SWITCH;
			vTaskResume(Start_LCD12864_Set_Strategy_TaskHandle);//恢复挂起
			G_Delete_Task_struct.D_Task = Start_LCD12864_Set_Strategy_Info_TaskHandle;
			Start_LCD12864_Set_Strategy_Info_TaskHandle = NULL;
			G_Delete_Task_struct.sign = ENABLE;
		}
		break;
	}
}

/*
 * 策略设置任务信息更改操作
 * */
void Start_LCD12864_Set_Strategy_Info_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;
	LCD12864_Display(0x00,0x00);//清空界面
	LCD12864_Set_Strategy_Info_Page(NULL,((Line_Num*)argument));//初始化策略是指界面
//	printf("argument = %d\r\n",*((Line_Num*)argument));
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,			//2 等待按键信息
					(uint32_t)0xffffffff,
					(uint32_t*)&NotifyValue,
					(TickType_t)portMAX_DELAY);
		if(err==pdTRUE)
		{
			LCD12864_Set_Strategy_Info_Page((uint8_t)NotifyValue,((Line_Num*)argument));
		}
	}
}
/*
 * 设置信息第1行
 * 策略X 开启/关闭
 */
void LCD12864_Set_Strategy_Info_Line1(uint8_t* p_Place,Line_Num* p_Line_Num)
{
	LCD12864_Put_16_16_Char(0xB7,0x04,0x00,WHITE_BASE);	//策字
	LCD12864_Put_16_16_Char(0xB7,0x14,0x01,WHITE_BASE);	//略字
	LCD12864_Put_8_16_Char(0xB7,0x24,(*p_Line_Num)+0x31,WHITE_BASE);	//NUM字
	if(S_Strategy[(*p_Line_Num)].Strategy_State == Strategy_Open)
	{
		LCD12864_Put_16_16_Char(0xB7,0x34,0x24,(*p_Place)==STRATEGY_INFO_SWITCH);	// 开启字
		LCD12864_Put_16_16_Char(0xB7,0x44,0x30,(*p_Place)==STRATEGY_INFO_SWITCH);
	}else{
		LCD12864_Put_16_16_Char(0xB7,0x34,0x25,(*p_Place)==STRATEGY_INFO_SWITCH);	// 关闭字
		LCD12864_Put_16_16_Char(0xB7,0x44,0x31,(*p_Place)==STRATEGY_INFO_SWITCH);
	}
	LCD12864_Put_16_16_Char(0xB7,0x64,0x02,(*p_Place)==STRATEGY_INFO_SET);	//设置两个字
	LCD12864_Put_16_16_Char(0xB7,0x74,0x03,(*p_Place)==STRATEGY_INFO_SET);
}
/*
 * 设置信息第2行
 */
void LCD12864_Set_Strategy_Info_Line2(uint8_t* p_Place,Line_Num* p_Line_Num)
{
	LCD12864_Put_16_16_Char(0xB5,0x04,0x32,WHITE_BASE);	//基字
	LCD12864_Put_16_16_Char(0xB5,0x14,0x14,WHITE_BASE);	//准字
	LCD12864_Put_8_16_Char(0xB5,0x24,':',WHITE_BASE);	//:符
	if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Route)
	{
		LCD12864_Put_16_16_Char(0xB5,0x34,0x22,(*p_Place)==STRATEGY_INFO_STANDARD);	// 风口字
		LCD12864_Put_16_16_Char(0xB5,0x44,0x23,(*p_Place)==STRATEGY_INFO_STANDARD);
	}else{
		LCD12864_Put_16_16_Char(0xB5,0x34,0x0A,(*p_Place)==STRATEGY_INFO_STANDARD);	// 温度字
		LCD12864_Put_16_16_Char(0xB5,0x44,0x0B,(*p_Place)==STRATEGY_INFO_STANDARD);
	}
}
/*
 * 设置信息第3行
 */
void LCD12864_Set_Strategy_Info_Line3(uint8_t* p_Place,Line_Num* p_Line_Num)
{
	char Hours[3];
	char Minutes[3];

	sprintf(Hours,"%02d",S_Strategy[(*p_Line_Num)].Time.Hours);
	sprintf(Minutes,"%02d",S_Strategy[(*p_Line_Num)].Time.Minutes);

	LCD12864_Put_16_16_Char(0xB3,0x04,0x0F,WHITE_BASE);	//时字
	LCD12864_Put_16_16_Char(0xB3,0x14,0x12,WHITE_BASE);	//间字
	LCD12864_Put_8_16_Char(0xB3,0x24,':',WHITE_BASE);	//:符

	LCD12864_Put_8_16_Char(0xB3,0x34,Hours[0],(*p_Place)==STRATEGY_INFO_TIME_HOURS);	//字
	LCD12864_Put_8_16_Char(0xB3,0x3C,Hours[1],(*p_Place)==STRATEGY_INFO_TIME_HOURS);
	LCD12864_Put_8_16_Char(0xB3,0x44,':',WHITE_BASE);	//:符
	LCD12864_Put_8_16_Char(0xB3,0x4C,Minutes[0],(*p_Place)==STRATEGY_INFO_TIME_MINUTES);
	LCD12864_Put_8_16_Char(0xB3,0x54,Minutes[1],(*p_Place)==STRATEGY_INFO_TIME_MINUTES);
}
/*
 * 设置信息第4行
 */
void LCD12864_Set_Strategy_Info_Line4(uint8_t* p_Place,Line_Num* p_Line_Num)
{
	char Route[5];
	char Min_Temp[3];
	char Max_Temp[3];
	if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Route)
	{
		sprintf(Route,"%04d",S_Strategy[(*p_Line_Num)].Route);
		LCD12864_Put_16_16_Char(0xB1,0x04,0x22,WHITE_BASE);	//风字
		LCD12864_Put_16_16_Char(0xB1,0x14,0x23,WHITE_BASE);	//口字
		LCD12864_Put_8_16_Char(0xB1,0x24,':',WHITE_BASE);	//:符
		LCD12864_Put_8_16_Char(0xB1,0x34,Route[0],(*p_Place)==STRATEGY_INFO_ROUTE_4);	// 风口/温度字
		LCD12864_Put_8_16_Char(0xB1,0x3C,Route[1],(*p_Place)==STRATEGY_INFO_ROUTE_3);
		LCD12864_Put_8_16_Char(0xB1,0x44,Route[2],(*p_Place)==STRATEGY_INFO_ROUTE_2);	//:符
		LCD12864_Put_8_16_Char(0xB1,0x4C,Route[3],(*p_Place)==STRATEGY_INFO_ROUTE_1);	// 风口/温度字
		LCD12864_Put_16_16_Char(0xB1,0x54,0x2D,WHITE_BASE);	//清空符号
		LCD12864_Put_16_16_Char(0xB1,0x64,0x2D,WHITE_BASE);	//
		LCD12864_Put_16_16_Char(0xB1,0x74,0x2D,WHITE_BASE);	//
	}else if(S_Strategy[(*p_Line_Num)].Strategy_Type == Strategy_Type_Temp){
		sprintf(Min_Temp,"%02d",S_Strategy[(*p_Line_Num)].Temp_l);
		sprintf(Max_Temp,"%02d",S_Strategy[(*p_Line_Num)].Temp_h);
		LCD12864_Put_16_16_Char(0xB1,0x04,0x0A,WHITE_BASE);	//温字
		LCD12864_Put_16_16_Char(0xB1,0x14,0x0B,WHITE_BASE);	//度字
		LCD12864_Put_8_16_Char(0xB1,0x24,':',WHITE_BASE);	//:符
		LCD12864_Put_8_16_Char(0xB1,0x34,Min_Temp[0],(*p_Place)==STRATEGY_INFO_MIN_TEMP);	//
		LCD12864_Put_8_16_Char(0xB1,0x3C,Min_Temp[1],(*p_Place)==STRATEGY_INFO_MIN_TEMP);
		LCD12864_Put_16_16_Char(0xB1,0x44,0x2F,WHITE_BASE);	//℃字
		LCD12864_Put_8_16_Char(0xB1,0x54,'-',WHITE_BASE);	//--
		LCD12864_Put_8_16_Char(0xB1,0x5c,'-',WHITE_BASE);
		LCD12864_Put_8_16_Char(0xB1,0x64,Max_Temp[0],(*p_Place)==STRATEGY_INFO_MAX_TEMP);	//符
		LCD12864_Put_8_16_Char(0xB1,0x6C,Max_Temp[1],(*p_Place)==STRATEGY_INFO_MAX_TEMP);	//
		LCD12864_Put_16_16_Char(0xB1,0x74,0x2F,WHITE_BASE);	//℃字
	}
}
