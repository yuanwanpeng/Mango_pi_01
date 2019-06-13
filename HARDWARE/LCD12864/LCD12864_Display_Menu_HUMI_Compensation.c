/*
 * LCD12864_Display_Menu_HUMI_Compensation.c
 *
 *  Created on: 2018年10月19日
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
#include "LCD12864_Display_Menu_HUMI_Compensation.h"

osThreadId Start_LCD12864_HUMI_Compensation_TaskHandle;

void LCD12864_Display_HUMI_Compensation_Interior(int8_t* p_S_comp,uint8_t* p_Coordinate)
{
	LCD12864_Put_16_16_Char(0xB7,0x14,0x0A,WHITE_BASE);//温字
	LCD12864_Put_16_16_Char(0xB7,0x24,0x0B,WHITE_BASE);//度字
	LCD12864_Put_16_16_Char(0xB7,0x34,0x0C,WHITE_BASE);//补字
	LCD12864_Put_16_16_Char(0xB7,0x44,0x0D,WHITE_BASE);//偿字
	LCD12864_Put_8_16_Char(0xB7,0x54,':',WHITE_BASE);

	LCD12864_Display_HUMI_Compensation_Refresh(p_S_comp,p_Coordinate);

	LCD12864_Put_16_16_Char(0xB5,0x54,0x2F,WHITE_BASE);//℃字

	LCD12864_Put_16_16_Char(0xB3,0x14,0x02,WHITE_BASE);//设字
	LCD12864_Put_16_16_Char(0xB3,0x24,0x03,WHITE_BASE);//置字

}
void LCD12864_Display_HUMI_Compensation_Refresh(int8_t* p_S_comp,uint8_t* p_Coordinate)
{
	char comp_arr[5];
	memset(comp_arr,'0',sizeof(comp_arr));

	if(*p_S_comp>=0)
	{
		sprintf(comp_arr,"%02d",*p_S_comp);
		LCD12864_Put_8_16_Char(0xB5,0x2a,comp_arr[0],(*p_Coordinate)==0);
		LCD12864_Put_8_16_Char(0xB5,0x32,'.',(*p_Coordinate)==0);
		LCD12864_Put_8_16_Char(0xB5,0x3a,comp_arr[1],(*p_Coordinate)==0);
		LCD12864_Put_8_16_Char(0xB5,0x42,' ',WHITE_BASE);
	}else{
		sprintf(comp_arr,"%03d",*p_S_comp);
		LCD12864_Put_8_16_Char(0xB5,0x2a,comp_arr[0],(*p_Coordinate)==0);
		LCD12864_Put_8_16_Char(0xB5,0x32,comp_arr[1],(*p_Coordinate)==0);
		LCD12864_Put_8_16_Char(0xB5,0x3a,'.',(*p_Coordinate)==0);
		LCD12864_Put_8_16_Char(0xB5,0x42,comp_arr[2],(*p_Coordinate)==0);
	}

	LCD12864_Put_16_16_Char(0xB3,0x14,0x02,(*p_Coordinate)==1);//设字
	LCD12864_Put_16_16_Char(0xB3,0x24,0x03,(*p_Coordinate)==1);//置字
}
/*
 * 打印此界面
 * */
void LCD12864_HUMI_Compensation_Key_Null_Opt(int8_t* p_S_comp,uint8_t* p_Coordinate)
{
	LCD12864_Display_HUMI_Compensation_Interior(p_S_comp,p_Coordinate);		//当前行程
}
void LCD12864_Display_HUMI_Compensation_Page(uint8_t key)
{
	static int8_t S_comp = 0;
	static uint8_t Mark = 0;
	static uint8_t S_Coordinate = 0;//当前坐标
	if(Mark==0)
	{
		S_comp = (int8_t)(comp*10);
		Mark = 1;
	}
	switch(key)
	{
	case NULL:	//第一次进入显示全部菜单内容
		LCD12864_HUMI_Compensation_Key_Null_Opt(&S_comp,&S_Coordinate);
		break;
	case K_1_num://选择按键
//		LCD12864_Display_HUMI_Compensation_Refresh(&S_comp);
//		comp = ((float)S_comp)/10;
//		AT24CXX_Write(HUMI_COMP,(uint8_t*)&S_comp,sizeof(S_comp));
		//切换到下一选项
		if(S_Coordinate<1)
		{
			S_Coordinate++;
		}else{
			S_Coordinate=0;
		}
		LCD12864_Display_HUMI_Compensation_Refresh(&S_comp,&S_Coordinate);
		break;
	case K_2_num:	//
		if(S_Coordinate==0)//坐标在设置上面
		{
			if(S_comp<50)
			{
				S_comp += 1;
				LCD12864_Display_HUMI_Compensation_Refresh(&S_comp,&S_Coordinate);
			}
		}else if(S_Coordinate==1)
		{
			LCD12864_Display_HUMI_Compensation_Refresh(&S_comp,&S_Coordinate);
			comp = ((float)S_comp)/10;
			AT24CXX_Write(HUMI_COMP,(uint8_t*)&S_comp,sizeof(S_comp));
			LCD12864_Display_Success();	//显示成功
			osDelay(1500);
			LCD12864_Display_Clear_Success();
		}
		break;
	case K_3_num:	//
		if(S_Coordinate==0)//坐标在设置上面
		{
			if(S_comp>(-50))
			{
				S_comp -= 1;
				LCD12864_Display_HUMI_Compensation_Refresh(&S_comp,&S_Coordinate);
			}
		}
		break;
	case K_4_num:	//
		if(Now_page == HUMI_Compensation_page){
			printf("now_page == HUMI_Compensation_page\r\n");
			//返回菜单界面
			LCD12864_Display(0x00,0x00);//清屏
			LCD12864_Display_Menu(NULL);
			S_comp = (int8_t)(comp*10);
			Mark = 0;
			Now_page = Menu_Page;
			S_Coordinate = 0;
			vTaskResume(Start_LCD12864_Menu_TaskHandle);//恢复挂起

			G_Delete_Task_struct.D_Task = Start_LCD12864_HUMI_Compensation_TaskHandle;
			Start_LCD12864_HUMI_Compensation_TaskHandle = NULL;
			G_Delete_Task_struct.sign = ENABLE;
		}
		break;
	default:
		break;
	}
}

void Start_LCD12864_HUMI_Compensation_Task(void const * argument)
{
	BaseType_t err;
	uint32_t NotifyValue;
	LCD12864_Display(0x00,0x00);//清屏
	LCD12864_Display_HUMI_Compensation_Page(NULL);//第一次进入显示全部菜单内容
	while(1)
	{
		err = xTaskNotifyWait((uint32_t)0x00,	//2 等待按键信息
				(uint32_t)0xffffffff,
				(uint32_t*)&NotifyValue,
				(TickType_t)portMAX_DELAY);
		if(err==pdTRUE)
		{
			LCD12864_Display_HUMI_Compensation_Page((uint8_t)NotifyValue);
		}
	}
}
