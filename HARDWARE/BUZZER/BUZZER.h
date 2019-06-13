/*
 * BUZZER.h
 *
 *  Created on: 2018Äê7ÔÂ25ÈÕ
 *      Author: Administrator
 */

#ifndef HARDWARE_BUZZER_BUZZER_H_
#define HARDWARE_BUZZER_BUZZER_H_
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "main.h"

#define BUZZER_ENABLE 	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
#define BUZZER_DISABLE 	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);

void Buzzer_ONE_Cycle(void);
void Buzzer_TWO_Cycle(void);
#endif /* HARDWARE_BUZZER_BUZZER_H_ */
