/*
 * BUZZER.c
 *
 *  Created on: 2018Äê7ÔÂ25ÈÕ
 *      Author: YuanWP
 */
#include "BUZZER.h"
osThreadId Start_Buzzer_TaskHandle = NULL;
void Buzzer_TWO_Cycle(void)
{
	BUZZER_ENABLE;
	osDelay(300);
	BUZZER_DISABLE;
}
void Buzzer_ONE_Cycle(void)
{
	BUZZER_ENABLE;
	osDelay(150);
	BUZZER_DISABLE;
}


