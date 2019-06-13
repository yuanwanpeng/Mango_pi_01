/*
 * delay.c
 *
 *  Created on: 2018年10月16日
 *      Author: YuanWP
 */
#include "delay.h"

static uint8_t  fac_us=0;							//us延时被乘数

void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;		//LOAD值
	ticks=nus*fac_us; 					//需要的节拍数
	tcnt=0;
	vTaskSuspendAll();		//关闭调取器
	told=SysTick->VAL;        					//刚进入计数器的计数值
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;		//
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;				//
		}
	};
	xTaskResumeAll();						//恢复调度器
}

