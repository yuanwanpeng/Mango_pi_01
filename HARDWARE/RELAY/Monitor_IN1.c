/*
 * Monitor_IN1.c
 *
 *  Created on: 2018年10月25日
 *      Author: YuanWP
 */
#include "Monitor_IN1.h"

uint8_t G_Beyond_Route_Mask;

void Start_Monitor_IN1_Task(void const * argument)//开始行程周期监测任务
{
	uint8_t relay1,relay2,In_State;
	while(1)
	{
		relay1 = RELAY1_STATE;
		relay2 = RELAY2_STATE;
		In_State = IN1_STATE;
		if(In_State==1)					//开始时候等于0  他会转到1
		{
			if(relay1==0)				//表示继电器1在启动 正转
			{
				while(In_State)			//判断他等不等于0
				{
					osDelay(100);
					In_State = IN1_STATE;
				}
				G_Now_Route++;			//行程加一圈
				if((G_Now_Route == G_Max_Route)&&(G_New_Mode != 0))//当前行程等于最大行程  自动模式下
				{
					Motor_Stop();		//当前行程大于等于最大行程停止转动
				}
				if((G_New_Mode == 0) && (G_Now_Route >= G_Max_Route))//如果模式等于0 表示实在手动模式下，判断当前行程大于等于最大行程吗？
				{
					if(G_Now_Route == G_Max_Route)
					{
						
						Motor_Stop();		//当前行程大于等于最大行程停止转动
					}
					if(G_Beyond_Route_Mask == 0)
					{
						//G_Old_Route = G_Now_Route;//将当前行程记录到我的buff buff将行程加5得到最大行程后的5个点位
						G_Beyond_Route_Mask = 1;
					}
					if(G_Now_Route == (G_Old_Route+5))
					{
						G_Beyond_Route_Mask = 0;
					//	G_Old_Route = G_Now_Route;
						Motor_Stop();		//当前行程大于等于最大行程停止转动
					}
				}
				Save_G_Now_Route(G_Now_Route);
			}else if(relay2==0)			//表示继电器2在启动 反转
			{
				while(In_State)			//判断他等不等于0
				{
					osDelay(100);
					In_State = IN1_STATE;
				}
				G_Now_Route--;			//行程减一圈
				if((G_Now_Route == 0)&&(G_New_Mode != 0))	//行程等于0停止转动
				{
					Motor_Stop();		//当前行程等于0停止转动
				}
				if((G_New_Mode == 0) && (G_Now_Route <= 0))//如果模式等于0 表示实在手动模式下，判断当前行程大于等于最大行程吗？
				{
					if(G_Now_Route == 0)
					{
						Motor_Stop();		//当前行程大于等于最大行程停止转动
					}
					if(G_Beyond_Route_Mask == 0)
					{
						//G_Old_Route = G_Now_Route;//将当前行程记录到我的buff buff将行程加5得到最大行程后的5个点位
						G_Beyond_Route_Mask = 1;
					}
					if(G_Now_Route == (G_Old_Route-5))
					{
						G_Beyond_Route_Mask = 0;
					//	G_Old_Route = G_Now_Route;
						Motor_Stop();		//当前行程大于等于最大行程停止转动
					}
				}
				Save_G_Now_Route(G_Now_Route);
			}
		}
	}
}
/*将行程记录到我的24c02上面*/
void Save_G_Now_Route(int16_t Now_Route)
{
	uint8_t Now_Route_H;
	uint8_t Now_Route_L;
	Now_Route_H = Now_Route>>8;
	Now_Route_L = Now_Route;
	AT24CXX_Write(NOW_ROUTE,&Now_Route_H,1);
	AT24CXX_Write(NOW_ROUTE+1,&Now_Route_L,1);
}

