/*
 * TIM.c
 *
 *  Created on: 2019Äê7ÔÂ6ÈÕ
 *      Author: Administrator
 */
 
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "TIM.h"
extern TIM_HandleTypeDef htim2;

extern	GPRS_TypeDef							*p_GPRS;
extern	Timer_TypeDef							*p_Tap_Timer;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (p_GPRS->GPRS_BUF->RX_Time >= Start_Delay)
		p_GPRS->GPRS_BUF->RX_Time ++;
//	p_Tap_Timer->ping_timer ++;
//	p_Tap_Timer->test_timer ++;
//	p_Tap_Timer->temp_timer ++;
//	
//	if (p_Tap_Timer->tap1_timer >= Start_Delay)
//		p_Tap_Timer->tap1_timer ++;
//	
//	if (p_Tap_Timer->tap2_timer >= Start_Delay)
//		p_Tap_Timer->tap2_timer ++;
//	
//	if (p_Tap_Timer->tap3_timer >= Start_Delay)
//		p_Tap_Timer->tap3_timer ++;
//	
	HAL_TIM_Base_Start_IT(&htim2);
}


