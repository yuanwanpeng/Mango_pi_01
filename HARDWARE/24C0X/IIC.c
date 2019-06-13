/*
 * IIC.c
 *
 *  Created on: 2018骞�10鏈�16鏃�
 *      Author: YuanWP
 */
#include "IIC.h"

#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#define AM2320_DELAY_US_IIC 8
/*
 *  鍒濆鍖朓Ic
 */
void IIC_Init(uint8_t hardware)
{
	if(hardware == AT24){
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pins :  SCL_24C0X_Pin  */
		GPIO_InitStruct.Pin = SCL_24C0X_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/*Configure GPIO pins :    SDA_24C0X_Pin                */
		GPIO_InitStruct.Pin = SDA_24C0X_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_GPIO_WritePin(SCL_24C0X_GPIO_Port, SCL_24C0X_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SDA_24C0X_GPIO_Port, SDA_24C0X_Pin,GPIO_PIN_RESET);
	}else if(hardware == AM2320){
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/*Configure GPIO pins :  SCL_24C0X_Pin  */
		GPIO_InitStruct.Pin = SCL_AM2320_Pin|SDA_AM2320_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_GPIO_WritePin(SCL_AM2320_GPIO_Port, SCL_AM2320_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(SDA_AM2320_GPIO_Port, SDA_AM2320_Pin,GPIO_PIN_SET);
	}
}
void SDA_OUT(uint8_t hardware)
{
	if(hardware == AT24)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		/*Configure GPIO pins :    SDA_24C0X_Pin                */
		GPIO_InitStruct.Pin = SDA_24C0X_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//涓婃媺杈撳嚭
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(SDA_24C0X_GPIO_Port, &GPIO_InitStruct);
	}else if(hardware == AM2320){
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = SDA_AM2320_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//涓婃媺杈撳嚭
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(SDA_AM2320_GPIO_Port, &GPIO_InitStruct);
	}
}
void SDA_IN(uint8_t hardware)
{
	if(hardware == AT24)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		/*Configure GPIO pins :    SDA_24C0X_Pin                */
		GPIO_InitStruct.Pin = SDA_24C0X_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//涓婃媺杈撳叆
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(SDA_24C0X_GPIO_Port, &GPIO_InitStruct);
	}else if(hardware == AM2320){
		GPIO_InitTypeDef GPIO_InitStruct;
		/*Configure GPIO pins :    SDA_AM2320_Pin                */
		GPIO_InitStruct.Pin = SDA_AM2320_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//涓婃媺杈撳叆
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(SDA_AM2320_GPIO_Port, &GPIO_InitStruct);
	}
}
//寮�濮嬩俊鍙�
void IIC_Start(uint8_t hardware)
{
	if(hardware == AT24)
	{
		SDA_OUT(hardware);
		IIC_AT24_SDA_H;
		IIC_AT24_SCL_H;
		delay_us(4);
		IIC_AT24_SDA_L;
		delay_us(4);
		IIC_AT24_SCL_L;
	}else if(AM2320){
		SDA_OUT(hardware);
		IIC_AM2320_SDA_H;
		IIC_AM2320_SCL_H;
		delay_us(AM2320_DELAY_US_IIC);
		IIC_AM2320_SDA_L;
		delay_us(AM2320_DELAY_US_IIC);
		IIC_AM2320_SCL_L;
	}
}
//鍋滄淇″彿
void IIC_Stop(uint8_t hardware)
{
	if(hardware == AT24)
	{
		SDA_OUT(hardware);//sda璁剧疆杈撳嚭
		IIC_AT24_SCL_L;
		IIC_AT24_SDA_L;//STOP:when CLK is high DATA change form low to high
		delay_us(4);
		IIC_AT24_SCL_H;
		IIC_AT24_SDA_H;
		delay_us(4);
	}else if(AM2320){
		SDA_OUT(hardware);//sda璁剧疆杈撳嚭
		IIC_AM2320_SDA_L;
		delay_us(AM2320_DELAY_US_IIC);
		IIC_AM2320_SCL_H;
		delay_us(AM2320_DELAY_US_IIC);
		IIC_AM2320_SDA_H;
	}
}
//绛夊緟搴旂瓟淇″彿鍒版潵
//杩斿洖鍊硷細1锛屾帴鏀跺簲绛斿け璐�
//        0锛屾帴鏀跺簲绛旀垚鍔�
uint8_t IIC_Wait_Ack(uint8_t hardware)
{
	uint8_t ucErrTime=0;
	if(hardware == AT24)
	{
		SDA_IN(hardware);      //SDA璁剧疆涓鸿緭鍏�
		IIC_AT24_SDA_H;delay_us(1);
		IIC_AT24_SCL_H;delay_us(1);
		while(HAL_GPIO_ReadPin(SDA_24C0X_GPIO_Port, SDA_24C0X_Pin))
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				IIC_Stop(hardware);
				return 1;
			}
		}
		IIC_AT24_SCL_L;//鏃堕挓杈撳嚭0
		return 0;
	}else if(AM2320){
		SDA_IN(hardware);      //SDA璁剧疆涓鸿緭鍏�
		IIC_AM2320_SDA_H;delay_us(10);
		IIC_AM2320_SCL_H;delay_us(10);
		while(HAL_GPIO_ReadPin(SDA_AM2320_GPIO_Port, SDA_AM2320_Pin))
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				IIC_Stop(hardware);
				return 1;
			}
		}
		IIC_AM2320_SCL_L;//鏃堕挓杈撳嚭0
		return 0;
	}
}
//浜х敓ACK搴旂瓟
void IIC_Ack(uint8_t hardware)
{
	if(hardware == AT24)
	{
		IIC_AT24_SCL_L;
		SDA_OUT(hardware);
		IIC_AT24_SDA_L;
		delay_us(2);
		IIC_AT24_SCL_H;
		delay_us(2);
		IIC_AT24_SCL_L;
	}else if(AM2320){
		IIC_AM2320_SCL_L;
		SDA_OUT(hardware);
		IIC_AM2320_SDA_L;
		delay_us(10);
		IIC_AM2320_SCL_H;
		delay_us(10);
		IIC_AM2320_SCL_L;
	}
}
//涓嶄骇鐢烝CK搴旂瓟
void IIC_NAck(uint8_t hardware)
{
	if(hardware == AT24)
	{
		IIC_AT24_SCL_L;
		SDA_OUT(hardware);
		IIC_AT24_SDA_H;
		delay_us(2);
		IIC_AT24_SCL_H;
		delay_us(2);
		IIC_AT24_SCL_L;
	}else if(AM2320){
		IIC_AM2320_SCL_L;
		SDA_OUT(hardware);
		IIC_AM2320_SDA_H;
		delay_us(10);
		IIC_AM2320_SCL_H;
		delay_us(10);
		IIC_AM2320_SCL_L;
	}
}
//IIC鍙戦�佷竴涓瓧鑺�
//杩斿洖浠庢満鏈夋棤搴旂瓟
//1锛屾湁搴旂瓟
//0锛屾棤搴旂瓟
void IIC_Send_Byte(uint8_t hardware,uint8_t txd)
{
	uint8_t t;
	if(hardware == AT24)
	{
		SDA_OUT(hardware);
		IIC_AT24_SCL_L;//鎷変綆鏃堕挓寮�濮嬫暟鎹紶杈�
		for(t=0;t<8;t++)
		{
				//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_AT24_SDA_H;
		else
			IIC_AT24_SDA_L;
		txd<<=1;
		delay_us(2);   //瀵筎EA5767杩欎笁涓欢鏃堕兘鏄繀椤荤殑
		IIC_AT24_SCL_H;
		delay_us(2);
		IIC_AT24_SCL_L;
		delay_us(2);
		}
	}else if(AM2320){
//		SDA_OUT(hardware);
//		uint8_t BitCnt=8;//涓�瀛楄妭8浣�
//		//璁剧疆SDA 鍙ｄ负杈撳嚭
//		do
//		{
//			IIC_AM2320_SCL_L;
//			delay_us(10);
//			if((txd&0x80)==0) //鍒ゆ柇鏈�楂樹綅鏄�0杩樻槸1
//				IIC_AM2320_SDA_L;
//			else
//				IIC_AM2320_SDA_H;
//			IIC_AM2320_SCL_H;
//			delay_us(10);
//			txd=txd<<1;//灏哹uffer涓殑鏁版嵁宸︾Щ涓�浣�
//			BitCnt--;
//		}while(BitCnt);
//		IIC_AM2320_SCL_L;
		SDA_OUT(hardware);
		IIC_AM2320_SCL_L;//鎷変綆鏃堕挓寮�濮嬫暟鎹紶杈�
		for(t=0;t<8;t++)
		{
			if((txd&0x80)>>7)
				IIC_AM2320_SDA_H;
			else
				IIC_AM2320_SDA_L;
			txd<<=1;
			delay_us(AM2320_DELAY_US_IIC);   //瀵筎EA5767杩欎笁涓欢鏃堕兘鏄繀椤荤殑
			IIC_AM2320_SCL_H;
			delay_us(AM2320_DELAY_US_IIC);
			IIC_AM2320_SCL_L;
			delay_us(AM2320_DELAY_US_IIC);
		}
	}
}
//璇�1涓瓧鑺傦紝ack=1鏃讹紝鍙戦�丄CK锛宎ck=0锛屽彂閫乶ACK
uint8_t IIC_Read_Byte(uint8_t hardware,uint8_t ack)
{
	uint8_t i,receive=0;
	if(hardware == AT24)
	{
		SDA_IN(hardware);//SDA璁剧疆涓鸿緭鍏�
		for(i=0;i<8;i++ )
		{
			IIC_AT24_SCL_L;
			delay_us(2);
			IIC_AT24_SCL_H;
			receive<<=1;
			if(HAL_GPIO_ReadPin(SDA_24C0X_GPIO_Port, SDA_24C0X_Pin))receive++;
			delay_us(1);
			}
			if (!ack)
					IIC_NAck(hardware);//鍙戦�乶ACK
			else
					IIC_Ack(hardware); //鍙戦�丄CK
			return receive;
		}else if(AM2320){
			SDA_IN(hardware);//SDA璁剧疆涓鸿緭鍏�
			uint8_t BitCnt=8,IIC_RX_Data=0;
			IIC_AM2320_SDA_H;         //璇诲叆鏁版嵁 璁剧疆SDA 鍙ｄ负杈撳叆
			do
			{
				IIC_AM2320_SCL_L;
				delay_us(10); 
				IIC_RX_Data = IIC_RX_Data<<1;
				BitCnt--;	  
				IIC_AM2320_SCL_H;
				delay_us(10);
				 if(IIC_AM2320_SDA_STATE==1)
					 IIC_RX_Data = IIC_RX_Data|0x01;  //浣庝綅缃�1
				 else
					 IIC_RX_Data = IIC_RX_Data&0x0fe; //浣庝綅娓�0
				}
				while(BitCnt);
				IIC_AM2320_SCL_L;

			if (!ack)
					IIC_NAck(hardware);//鍙戦�乶ACK
			else
					IIC_Ack(hardware); //鍙戦�丄CK
			return IIC_RX_Data;
		}
}





