/*
 * HUMI.c
 *
 *  Created on: 2018年8月14日
 *      Author: YuanWP
 */
#include "HUMI.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "LCD12864.h"
#include <math.h>
 #include "AM2320.h"
//extern ADC_HandleTypeDef hadc1;
extern osThreadId Start_LCD12864_HUMI_TaskHandle;
float Compensation = 10.2;
const float Rp=10000.0;             //10K
const float T2 = (273.15+25.0); //T2
const float Bx = 3950.0;              //B
const float Ka = 273.15;
float temp;
float g_rh;
float comp = 0.5;

void Init_Humi_Comp(void)
{
	int8_t S_comp;
	AT24CXX_Read(HUMI_COMP,(uint8_t*)&S_comp,sizeof(S_comp));
//	printf("S_comp = %d\r\n",S_comp);
	if((S_comp<-50)||(S_comp>50))
	{
//		printf("reinit comp\r\n");
		S_comp = 5;			//初始化为0.5补偿
		AT24CXX_Write(HUMI_COMP,(uint8_t*)&S_comp,sizeof(S_comp));
		AT24CXX_Read(HUMI_COMP,(uint8_t*)&S_comp,sizeof(S_comp));
	}

	comp = ((float)S_comp)/10;
}

void Start_HUMI_Task(void const * argument)
{
//	float Rt;
	//HAL_ADC_Start(&hadc1);

	LCD12864_Display(0x00,0x00);
	osDelay(3000);
	IIC_Init(AM2320);
	
	while(1)
	{
		AM2320_Get_TempResistor(&temp,&g_rh);
		//printf("temp = %d\r\n",(uint8_t)(temp));
		temp+=comp; //加0.5误差矫正
		osDelay(1000);
	}
}



