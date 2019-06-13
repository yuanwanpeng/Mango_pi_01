/*
 * AM2320.h
 *
 *  Created on: 2019Äê3ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef HARDWARE_AM2320_AM2320_H_
#define HARDWARE_AM2320_AM2320_H_

#include "main.h"
#include "FreeRTOS.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "IIC.h"

typedef struct{
	uint8_t function_code;
	uint8_t data_len;
	uint8_t humi_h_8bit;
	uint8_t humi_l_8bit;
	uint8_t temp_h_8bit;
	uint8_t temp_l_8bit;
	uint8_t crc_l_8bit;
	uint8_t crc_h_8bit;
}HUMI_struct;

float AM2320_Get_TempResistor(float* p_temp,float* rh);

#endif /* HARDWARE_BUTTON_BUTTON_H_ */
