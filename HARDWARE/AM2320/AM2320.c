/*
 * AM2320.c
 *
 *  Created on: 2019年3月17日
 *      Author: YuanWP
 */
#include "AM2320.h"
 
#define IIC_Add 0xB8    //器件地址
#define IIC_RX_Length 15
// 变量定义
uint8_t Sensor_Data[5]={0x00,0x00,0x00,0x00,0x00};
uint8_t IIC_TX_Buffer[]={0x03,0x00,0x04}; //读温湿度命令（无CRC校验）
unsigned char IIC_RX_Buffer[IIC_RX_Length] = {0x00};//读回的温湿度

uint8_t WR_Flag;

uint8_t Test_Ack(void)
{
	uint8_t ACK_Flag=0;
	SDA_IN(AM2320);
	IIC_AM2320_SCL_L;
	IIC_AM2320_SDA_H;
	delay_us(10);
	IIC_AM2320_SCL_H;
	delay_us(10);
	if(IIC_AM2320_SDA_STATE==0)
		ACK_Flag = 1;
	else 
		ACK_Flag = 0;
	IIC_AM2320_SCL_L;
	return ACK_Flag;
}

uint8_t WriteNByte(uint8_t sla,uint8_t *s,uint8_t n)
{
	uint8_t i;

	IIC_Start(AM2320); //启动I2C
	IIC_Send_Byte(AM2320, sla);//发送器件地址
	if(!Test_Ack())
	{	
		WR_Flag = 1;//这里有问题**************
		printf("w wr_flag1\r\n");
		return(0);
	}
	for(i=0;i<n;i++)//写入8字节数据
	{
		IIC_Send_Byte(AM2320,*(s+i));
		if(!Test_Ack())
		{
			WR_Flag = 1;//这里也有问题
			printf("w wr_flag2\r\n");
			return(0);
		}
	}
	IIC_Stop(AM2320);
	return(1);
}

uint8_t ReadNByte(uint8_t Sal, uint8_t *p,uint8_t n)
{
  uint8_t i;
  IIC_Start(AM2320); 			//启动I2C
  IIC_Send_Byte(AM2320,(Sal) | 0x01); 	//发送器件地址
  if(!Test_Ack())
  {
  	WR_Flag = 1;
		printf("r wr_flag3\r\n");
		return(0);
  }
  osDelay(1);
  for(i=0;i<n-1;i++)  //读取字节数据
  {
     *(p+i)=IIC_Read_Byte(AM2320,1); //读取数据需要ack
  }
  *(p+n-1)=IIC_Read_Byte(AM2320,0);  //不需要ack
	IIC_Stop(AM2320);
  return(1);	 
}
void Waken(void)
{
	IIC_Start(AM2320);						//起始信号
	IIC_Send_Byte(AM2320,0xB8);	  //发送器件地址							
	Test_Ack();										//校验位
	osDelay(2);										//延时最大3ms唤醒操作
	IIC_Stop(AM2320);							//产生一个停止条件
}

void Clear_Data (void)
{
	int i;
	for(i=0;i<IIC_RX_Length;i++)
	{
		IIC_RX_Buffer[i] = 0x00;
	}//接收数据清零
}
///计算CRC校验码
unsigned int CRC16(unsigned char *ptr, unsigned char len)
{
   unsigned int crc=0xffff;
   unsigned char i;
   while(len--)
   {
       crc ^=*ptr++;
       for(i=0;i<8;i++)
	   {
	       if(crc & 0x1)
		   {
		      crc>>=1;
			  crc^=0xa001;
		   }
		   else
		   {
		      crc>>=1;
		   }
	   }
   }
   return crc;
}
///检测CRC校验码是否正确
unsigned char CheckCRC(unsigned char *ptr,unsigned char len)
{
  unsigned int crc;
	crc=(unsigned int)CRC16(ptr,len-2);
	if(ptr[len-1]==(crc>>8) && ptr[len-2]==(crc & 0x00ff))
	{
	    return 0xff;
	}
	else
	{
	   return 0x0;
	}
}
float AM2320_Get_TempResistor(float* p_temp,float* rh)
{
//	HUMI_struct humi_struct;
	IIC_AM2320_SDA_H;
	IIC_AM2320_SCL_H;
	Clear_Data(); // 清除收到数据
	WR_Flag = 0;
	Waken();	  // 唤醒传感器

	//发送读指令
	WriteNByte(IIC_Add,IIC_TX_Buffer,3);
	//发送读取或写数据命令后，至少等待2MS（给探头返回数据作时间准备）
	osDelay(3); 
	
	//读返回数据
	ReadNByte(IIC_Add,IIC_RX_Buffer,8);
	
	IIC_AM2320_SDA_H;
	IIC_AM2320_SCL_H;
	if(CheckCRC(IIC_RX_Buffer,8))
	{
		*p_temp = (IIC_RX_Buffer[4]*256+IIC_RX_Buffer[5]);
		*rh = (IIC_RX_Buffer[2]*256+IIC_RX_Buffer[3])/10;
	}else{
		*p_temp = 0;
		*rh = 0;
	}

	return 1;
}		





