/*
 * relay.h
 *
 *  Created on: 2018年7月25日
 *      Author: YuanWP
 */

#ifndef HARDWARE_RELAY_RELAY_H_
#define HARDWARE_RELAY_RELAY_H_
#include "stdint.h"
#include "button.h"
#include "board_info.h"



#define RELAY1_ENABLE 	HAL_GPIO_WritePin(OUTC1_GPIO_Port, OUTC1_Pin, GPIO_PIN_RESET);
#define RELAY1_DISABLE 	HAL_GPIO_WritePin(OUTC1_GPIO_Port, OUTC1_Pin, GPIO_PIN_SET);
#define RELAY1_STATE 	HAL_GPIO_ReadPin(OUTC1_GPIO_Port, OUTC1_Pin);

#define LED_REV_ENABLE 	HAL_GPIO_WritePin(LED_REV_GPIO_Port, LED_REV_Pin, GPIO_PIN_RESET);
#define LED_REV_DISABLE HAL_GPIO_WritePin(LED_REV_GPIO_Port, LED_REV_Pin, GPIO_PIN_SET);
#define LED_REV_STATE 	HAL_GPIO_ReadPin(LED_REV_GPIO_Port, LED_REV_Pin);

#define LED_FWD_ENABLE 	HAL_GPIO_WritePin(LED_FWD_GPIO_Port, LED_FWD_Pin, GPIO_PIN_RESET);
#define LED_FWD_DISABLE HAL_GPIO_WritePin(LED_FWD_GPIO_Port, LED_FWD_Pin, GPIO_PIN_SET);
#define LED_FWD_STATE 	HAL_GPIO_ReadPin(LED_FWD_GPIO_Port, LED_FWD_Pin);

#define RELAY2_ENABLE 	HAL_GPIO_WritePin(OUTC2_GPIO_Port, OUTC2_Pin, GPIO_PIN_RESET);
#define RELAY2_DISABLE 	HAL_GPIO_WritePin(OUTC2_GPIO_Port, OUTC2_Pin, GPIO_PIN_SET);
#define RELAY2_STATE 	HAL_GPIO_ReadPin(OUTC2_GPIO_Port, OUTC2_Pin);

#define RELAY3_ENABLE 	HAL_GPIO_WritePin(OUTC3_GPIO_Port, OUTC3_Pin, GPIO_PIN_RESET);
#define RELAY3_DISABLE 	HAL_GPIO_WritePin(OUTC3_GPIO_Port, OUTC3_Pin, GPIO_PIN_SET);
#define RELAY3_STATE 		HAL_GPIO_ReadPin(OUTC3_GPIO_Port, OUTC3_Pin);


#define MANUAL_MODE 0

extern uint16_t G_Max_Route;
extern int16_t G_Now_Route;//当前行程
extern int16_t G_Old_Route;
extern uint8_t G_Stroking_Cycle;//行程周期
extern osThreadId Start_Relay_TaskHandle;
/*
 *  运行继电器
 *
 */
void Check_Max_And_Now_Route(void);
void Init_Max_Route(uint16_t Max_Route);
void Init_Now_Route(int16_t Now_Route);
void Start_Relay_Task(void const * argument);
void G_Route_Reset(void);
void Relay_key_Opt(uint8_t key);
void Motor_Forward(void);
void Motor_Reverses(void);
void Motor_Stop(void);
void Check_Stroking_Cycle(void);
void Init_Stroking_Cycle(uint8_t Stroking_Cycle);
#endif /* HARDWARE_RELAY_RELAY_H_ */
