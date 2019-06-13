/*
 * LCD12864.c
 *
 *  Created on: 2018年7月25日
 *      Author: YuanWP
 */

#include "LCD12864.h"
#include "button.h"
#include "TypeFace.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "relay.h"
#include "task.h"

extern float temp;
extern float g_ht;
extern RTC_HandleTypeDef hrtc;
extern uint8_t G_Echo;
osThreadId Start_LCD12864_HUMI_TaskHandle = NULL;
osThreadId Start_LCD12864_Menu_TaskHandle = NULL;
osThreadId Start_LCD12864_Dispatch_TaskHandle;

volatile New_page_enum Now_page = Humi_Page;
extern uint8_t G_Sim800C_Signal;
/*
 * void writec(uchar com)
 * 写命令函数
 * */
void WriteCommand(uint8_t CommandByte)
{
	uint8_t i;
	CS_L;
	A0_L;
	//printf("%02x\r\n",CommandByte);
	for(i=0;i<8;i++)
	{
		if(CommandByte&0x80)
		{
			SDA_H;
		}else{
			SDA_L;
		}
		SCL_H;
		CommandByte=CommandByte<<1;
		SCL_L;
		delay_us(3);
	}

	CS_H;
	A0_H;
}
void Write_Data(uint8_t dat)
{
	uint8_t i;
	CS_L;
	A0_H;

	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		{
			SDA_H;
		}else{
			SDA_L;
		}
		SCL_H;
		dat=dat<<1;
		SCL_L;
		delay_us(3);
	}
	CS_H;
	A0_H;
}
void LCD12864_Display(uint8_t dat1,uint8_t dat2)
{
	uint8_t row,col;

   for (row=0xb0; row<0xb8; row++)    //0XB0    0XB8
   {
	  WriteCommand(row);//set page address
      WriteCommand(0x10);//set column address
      WriteCommand(0x00);
      for(col=0;col<128;col++)
      {
		Write_Data(dat1);
        Write_Data(dat2);
      }
   }

 //  osDelay(5);
}

/*
 * 12864显示函数显示汉子16*16字体
 */
void LCD12864_Put_16_16_Char(uint8_t row_addr,uint8_t column_addr,uint8_t chr,uint8_t Base_Color)
{
	uint8_t row,col,i;
	uint8_t* p_temp = Chinese_Menu_Type+chr*32;
	taskENTER_CRITICAL();		//进入临界区
	for(row = row_addr;row >= (row_addr-1);row--)
	{
		WriteCommand(row);
		WriteCommand((column_addr>>4)|0x10);		//Set column address (MSB)
		WriteCommand(column_addr&0x0F);				//Set column address (LSB)
		for(i = 0;i < 2;i++)			//(64/8)
		{
			for(col=0;col<8;col++)
			{
				if(Base_Color == WHITE_BASE){		//白背景
					Write_Data(*p_temp++);
				}else if(Base_Color == BLACK_BASE){	//加深黑的背景
					Write_Data(~(*p_temp++));
				}
			}
		}
	}
	taskEXIT_CRITICAL();//退出临界区
}
/*
 * 12864显示函数显示汉子8*16字体
 */
void LCD12864_Put_8_16_Char(uint8_t row_addr,uint8_t column_addr,uint8_t chr,uint8_t Base_Color)
{
	uint8_t row,col;
	uint8_t* p_temp = ASCIIchardot+(chr-0x20)*16;
	taskENTER_CRITICAL();		//进入临界区
	for(row = row_addr;row >= (row_addr-1);row--)
	{
		WriteCommand(row);
		WriteCommand((column_addr>>4)|0x10);		//Set column address (MSB)
		WriteCommand(column_addr&0x0F);				//Set column address (LSB)
		for(col=0;col<8;col++)
		{
			if(Base_Color == WHITE_BASE){		//白背景
				Write_Data(*p_temp++);
			}else if(Base_Color == BLACK_BASE){	//加深黑的背景
				Write_Data(~(*p_temp++));
			}
		}
	}
	taskEXIT_CRITICAL();//退出临界区
}
/*
 *
 * 初始化LCD12864
*/
void LCD12864_Init(void)
{
	RS_H;
	osDelay(220);
	RS_L;
	osDelay(420);
	RS_H;
	osDelay(220);

	WriteCommand(0xe3);  // reset signal
	WriteCommand(0xa3);  //(0xa2 1/9 bias,1/65 duty )
	WriteCommand(0xa1);  // ADC select
	WriteCommand(0xc0);  // command output select
	WriteCommand(0x2f);  // power control
	WriteCommand(0x23);  // select resistor ratio Rb/Ra
	WriteCommand(0x81);  // select volume
	WriteCommand(0x27);   // vop
	WriteCommand(0xf8);  // x4
	WriteCommand(0x08);  // x4
	WriteCommand(0xb0);//set page address
	WriteCommand(0x10);//set column address
	WriteCommand(0x00);

	WriteCommand(0xaf);  //display on
}

/*
 * 打印℃符号24x32
 * */
void LCD12864_Put_Symbol_C(void)
{
	uint8_t row,col,i;
	uint8_t* p_temp = Symbol;
	WriteCommand(0xB1);
	WriteCommand(0x11);
	WriteCommand(0x00);
	for(row = 0xB5;row >= 0xB2;row--)
	{
		WriteCommand(row);
		WriteCommand(0x14);	//Set column address (MSB)0x15
		WriteCommand(0x0a);		//Set column address (LSB)0x04
		for(i = 0;i < 2;i++)		//(64/8)
		{
			for(col=0;col<12;col++)
			{
				Write_Data(*p_temp++);
			}
		}
	}
}
/*
 * 第x行显示什么
 * */
void LCD12864_Put_HUMI(float temp)
{
	uint8_t row,col,i,place;
	uint8_t num[5];
	uint8_t* p_temp;
	uint16_t int_temp;
	int_temp = (temp)/1;
	num[0] = int_temp/100;
	num[1] = int_temp%100/10;
	num[2] = 10;			//10表示‘.’
	num[3] = int_temp%10;
	for(place = 0;place < 4;place++)
	{
		p_temp = Symbol_number+64*num[place];
		for(row = 0xB5;row >= 0xB2;row--)
		{
			WriteCommand(row);
			WriteCommand(0x10+place);	//Set column address (MSB)0x11
			WriteCommand(0x08);		//Set column address (LSB)0x00
			for(i = 0;i < 2;i++)		//(64/8)
			{
				for(col=0;col<8;col++)
				{
					Write_Data(*p_temp++);
				}
			}
		}
	}
}
/*
 * 成功字符
 */
void LCD12864_Display_Success(void)
{
	if(Now_page != Strategy_Menu_Info_page)
	{
		LCD12864_Put_16_16_Char(0xB3,0x44,0x2B,WHITE_BASE);//成字
		LCD12864_Put_16_16_Char(0xB3,0x54,0x2C,WHITE_BASE);//功字
	}else{
		LCD12864_Put_16_16_Char(0xB5,0x64,0x2B,WHITE_BASE);//成字
		LCD12864_Put_16_16_Char(0xB5,0x74,0x2C,WHITE_BASE);//功字
	}
}
/*
 * 删除成功字符
 */
void LCD12864_Display_Clear_Success(void)
{
	if(Now_page != Strategy_Menu_Info_page)
	{
		LCD12864_Put_16_16_Char(0xB3,0x44,0x2D,WHITE_BASE);//成字删除
		LCD12864_Put_16_16_Char(0xB3,0x54,0x2D,WHITE_BASE);//功字删除
	}else{
		LCD12864_Put_16_16_Char(0xB5,0x64,0x2D,WHITE_BASE);//成字删除
		LCD12864_Put_16_16_Char(0xB5,0x74,0x2D,WHITE_BASE);//功字删除
	}
}
/*
 * 打印出来文字到12864显示  Sim卡信号状态
 */
void LCD12864_Put_Signel_Sim(uint8_t signal)
{
	uint8_t row,col,i;
	uint8_t* p_temp = Signal_Strength+((signal/10)*16);

	//LCD12864_Put_8_16_Char(0xB7,0x04,Hours[0],WHITE_BASE);

	for(row = 0xB7;row >= 0xB6;row--)
	{
		WriteCommand(row);
		WriteCommand(0x10);		//Set column address (MSB)
		WriteCommand(0x04);				//Set column address (LSB)
		for(i = 0;i < 1;i++)			//(64/8)
		{
			for(col=0;col<8;col++)
			{
				Write_Data(*p_temp++);
			}
		}
	}
}
/*
 * 打印出来文字到12864显示 wifi信号状态
 */
void LCD12864_Put_Signel_Wifi(void)
{
	uint8_t row,col,i;
	uint8_t* p_temp = Wifi_Signel;
	for(row = 0xB7;row >= 0xB6;row--)
	{
		WriteCommand(row);
		WriteCommand(0x11);		//Set column address (MSB)
		WriteCommand(0x00);				//Set column address (LSB)
		for(i = 0;i < 1;i++)			//(64/8)
		{
			for(col=0;col<8;col++)
			{
				Write_Data(*p_temp++);
			}
		}
	}
}
/*
 * 打印出来文字到12864显示电机状态
 * 0--停转
 * 1--正转
 * 2--反转
 */
void LCD12864_Put_Motor_Situation(uint8_t motor_situation)
{
	uint8_t relay1,relay2;
	if(motor_situation == NULL)
	{
		relay1 = RELAY1_STATE;
		relay2 = RELAY2_STATE;
		if(relay1 == 0){//表示在正转
			motor_situation = FOREWARD;
		}else if(relay2 == 0){
			motor_situation = REVERSAL;
		}else if(relay2==relay1){
			motor_situation = STALLING;
		}
	}
	switch(motor_situation){
	case STALLING:			//停转
		//p_temp = Chinese_Menu_Type+(0x1d*32);
		LCD12864_Put_16_16_Char(0xB7,0x60,0x1D,WHITE_BASE);//1D是 停 字
		LCD12864_Put_16_16_Char(0xB7,0x70,0x21,WHITE_BASE);//21是 转 字
		break;
	case FOREWARD:			//正转
		LCD12864_Put_16_16_Char(0xB7,0x60,0x1F,WHITE_BASE);//1F是 正 字
		LCD12864_Put_16_16_Char(0xB7,0x70,0x21,WHITE_BASE);//21是 转 字
		break;
	case REVERSAL:			//反转
		LCD12864_Put_16_16_Char(0xB7,0x60,0x20,WHITE_BASE);//20是 反 字
		LCD12864_Put_16_16_Char(0xB7,0x70,0x21,WHITE_BASE);//21是 转 字
		break;
	}
}

/*
 * 打印出来文字到12864显示
 * 手动
 * 策略x
 */
void LCD12864_Put_Str_At_Present_Mode(uint8_t mode_num)
{
	if(mode_num>0)
	{
		LCD12864_Put_16_16_Char(0xB7,0x20,0x00,WHITE_BASE);//策字
		LCD12864_Put_16_16_Char(0xB7,0x30,0x01,WHITE_BASE);//略字
		LCD12864_Put_8_16_Char(0xB7,0x40,mode_num+0x30,WHITE_BASE);

	}else{
		LCD12864_Put_16_16_Char(0xB7,0x20,0x33,WHITE_BASE);//手字
		LCD12864_Put_16_16_Char(0xB7,0x30,0x34,WHITE_BASE);//动字
		LCD12864_Put_8_16_Char(0xB7,0x40,0x20,WHITE_BASE); //' '
	}

}
/*
 * 打印出来文字到12864显示
 */
void LCD12864_Put_Str_Welcome(void)
{
	uint8_t row,col,i,place;
	uint8_t* p_temp = Welcome_type;
	for(place = 0;place <= 6;place++)
	{
		for(row = 0xB6;row >= 0xB2;row--)
		{
			WriteCommand(row);
			WriteCommand(0x10+place);		//Set column address (MSB)
			WriteCommand(0x08);				//Set column address (LSB)
			for(i = 0;i <2;i++)			//
			{
				for(col=0;col<8;col++)
				{
					Write_Data(*p_temp++);
				}
			}
			//*p_temp++;
		}
	}
}
/*
 * 风口：  9999   T 00:00
 */
void LCD12864_Put_Open_Mouth_And_Time(void)
{
//	LCD12864_Put_16_16_Char(0xB1,0x04,0x22,WHITE_BASE);//22是 风 字
//	LCD12864_Put_16_16_Char(0xB1,0x14,0x23,WHITE_BASE);//23是 口 字
//	LCD12864_Put_8_16_Char(0xB1,0x21,':',WHITE_BASE);

//	LCD12864_Put_8_16_Char(0xB1,0x50+4,'T',WHITE_BASE);
}
/*
 * 主界面时间刷新 显示小时和分钟
 */
void LCD12864_Refresh_Time(void)
{
	RTC_TimeTypeDef Time;
	char Hours[3];
	char Minutes[3];

	memset(Hours,'0',sizeof(Hours));
	memset(Minutes,'0',sizeof(Minutes));

	HAL_RTC_GetTime(&hrtc,&Time,RTC_FORMAT_BIN);

	sprintf(Hours,"%02d",(int8_t)Time.Hours);
	sprintf(Minutes,"%02d",(int8_t)Time.Minutes);

	LCD12864_Put_8_16_Char(0xB1,0x58+4,Hours[0],WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB1,0x60+4,Hours[1],WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB1,0x68+4,':',WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB1,0x70+4,Minutes[0],WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB1,0x78+4,Minutes[1],WHITE_BASE);
}
/*
 * 主界面刷新 行程数
 */
void LCD12864_Refresh_Route(void)
{
	char Route[10];

	memset(Route,'0',sizeof(Route));

	sprintf(Route,"%04d",(int8_t)G_Now_Route);
	//printf("Route = %s\r\n",Route);

//	LCD12864_Put_8_16_Char(0xB1,0x2a-3,Route[0],WHITE_BASE);
//	LCD12864_Put_8_16_Char(0xB1,0x32-3,Route[1],WHITE_BASE);
//	LCD12864_Put_8_16_Char(0xB1,0x3a-3,Route[2],WHITE_BASE);
//	LCD12864_Put_8_16_Char(0xB1,0x42-3,Route[3],WHITE_BASE);

	LCD12864_Put_8_16_Char(0xB1,0x00+4,Route[0],WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB1,0x08+4,Route[1],WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB1,0x10+4,Route[2],WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB1,0x18+4,Route[3],WHITE_BASE);
}
/*
 *	显示RH符号
 */
void LCD12864_Put_Symbol_RH(void)
{
	LCD12864_Put_8_16_Char(0xB3,0x70+4,'R',WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB3,0x78+4,'H',WHITE_BASE);
}
/*
 * 打印湿度
 */
void LCD12864_Put_RH(float RH)
{
	uint8_t rh = (uint8_t)RH;
	char array[3];
	//printf("%d\r\n",rh);
	sprintf(array,"%02d",rh);
	LCD12864_Put_8_16_Char(0xB3,0x60+2,array[0],WHITE_BASE);
	LCD12864_Put_8_16_Char(0xB3,0x68+2,array[1],WHITE_BASE);
}
/*
 *显示温度/湿度/信号强度(只做显示)
 */
void Start_LCD12864_Display_HUMI_Task(void const * argument)
{
	LCD12864_Display(0x00,0x00);
	LCD12864_Put_Str_At_Present_Mode(G_New_Mode);
	//LCD12864_Put_Signel_Wifi();
	LCD12864_Put_Motor_Situation(NULL);
	LCD12864_Put_Open_Mouth_And_Time();
	while(1)
	{
		LCD12864_Put_Signel_Sim(G_Sim800C_Signal);//显示信号强度
		LCD12864_Put_HUMI(temp);//这里显示温度
		LCD12864_Put_Symbol_C();
		LCD12864_Put_RH(g_rh);
		LCD12864_Put_Symbol_RH();	//显示湿度符号
		LCD12864_Refresh_Time();
		LCD12864_Refresh_Route();
		LCD12864_Put_Motor_Situation(NULL);
		Now_page = Humi_Page;
		osDelay(1000);
	}
}


void Send_Notify(uint8_t key)	//按键消息发送至哪个界面
{
	if(Now_page == Humi_Page)
	{
		if(Start_Relay_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_Relay_TaskHandle,//因为这不是菜单内操作所以不往菜单中发信息
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Menu_Page)
	{
		if(Start_LCD12864_Menu_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Menu_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Strategy_Menu)
	{
		if(Start_LCD12864_Set_Strategy_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Set_Strategy_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Route_Reset_page)
	{
		if(Start_LCD12864_Route_Reset_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Route_Reset_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Max_Route_page)
	{
		if(Start_LCD12864_Max_Route_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Max_Route_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == HUMI_Compensation_page)
	{
		if(Start_LCD12864_HUMI_Compensation_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_HUMI_Compensation_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Delay_Inspection_page)
	{
		if(Start_LCD12864_Delay_Inspection_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Delay_Inspection_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Set_Time_page)
	{
		if(Start_LCD12864_Set_Time_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Set_Time_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Stroking_Cycle_page)
	{
		if(Start_LCD12864_Stroking_Cycle_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Stroking_Cycle_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}else if(Now_page == Strategy_Menu_Info_page)
	{
		if(Start_LCD12864_Set_Strategy_Info_TaskHandle != NULL)
		{
			xTaskNotify((TaskHandle_t)Start_LCD12864_Set_Strategy_Info_TaskHandle,
						(uint32_t)key,
						(eNotifyAction)eSetValueWithOverwrite);
		}
	}
}

/*
 * 页面调度函数
 * 只要控制也面显示的操作，
 * */
void Start_LCD12864_Task(void const * argument)
{
	BaseType_t err;

	uint32_t NotifyValue;
	LCD12864_Init();
	HAL_GPIO_WritePin(BACKLIGHT_GPIO_Port,BACKLIGHT_Pin,GPIO_PIN_RESET);
	LCD12864_Display(0x00,0x00);
	LCD12864_Put_Str_Welcome();
	osDelay(5000);
	LCD12864_Display(0x00,0x00);
	taskENTER_CRITICAL();				//进入临界区
	osThreadDef(LCD12864_Display_HUMI_Task, Start_LCD12864_Display_HUMI_Task, osPriorityNormal, 0, 64);//创建温控模块
	Start_LCD12864_HUMI_TaskHandle = osThreadCreate(osThread(LCD12864_Display_HUMI_Task), NULL);
	taskEXIT_CRITICAL();				//退出临界区
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,
						(uint32_t)0xffffffff,
						(uint32_t*)&NotifyValue,
						(TickType_t)portMAX_DELAY);
		G_Echo = 0;
		if(err==pdTRUE)
		{
			if(((uint8_t)NotifyValue)==K_1_num)		//功能按键
			{
				if(Now_page == Humi_Page)
				{
					vTaskSuspend(Start_LCD12864_HUMI_TaskHandle);		//挂起lcd12864显示温度的进程
					osThreadDef(LCD12864_Display_Menu_Task, Start_LCD12864_Display_Menu_Task, osPriorityNormal, 0, 64);//创建菜单任务
					Start_LCD12864_Menu_TaskHandle = osThreadCreate(osThread(LCD12864_Display_Menu_Task), NULL);
				}else{
					Send_Notify(K_1_num);
				}
			}
			if(((uint8_t)NotifyValue)==K_2_num)		//向上按键
			{
				Send_Notify(K_2_num);
			}
			if(((uint8_t)NotifyValue)==K_3_num)		//向下按键
			{
				Send_Notify(K_3_num);
			}
			if(((uint8_t)NotifyValue)==K_4_num)		//取消按键
			{
				Send_Notify(K_4_num);
			}
			if(((uint8_t)NotifyValue)==K_1_num_long)	//选择按键长按
			{
				Send_Notify(K_1_num_long);
			}
			if(((uint8_t)NotifyValue)==K_2_num_long)	//向上按键长按
			{
				Send_Notify(K_2_num_long);
			}
			if(((uint8_t)NotifyValue)==K_3_num_long)	//向下按键长按
			{
				Send_Notify(K_3_num_long);
			}
			if(((uint8_t)NotifyValue)==K_4_num_long)	//取消按键长按
			{
				Send_Notify(K_4_num_long);
			}
		}
	}
}


