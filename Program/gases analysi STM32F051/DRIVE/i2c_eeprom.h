#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#include "stm32f0xx.h" 

#define AT24C16_Addarss 0
#define Page1 0xA0
#define Page2 0xa2
#define Page3 0xa4
#define Page4 0xa6
#define Page5 0xa8

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  
#define AT24C512	65534

#define EE_TYPE AT24C512//AT24C16

//IO方向设置
#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7输出模式

#define  READ_SDA  (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))

#define  IIC_SCL_Hight GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define  IIC_SCL_Low   GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define  IIC_SDA_Hight GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define  IIC_SDA_Low   GPIO_ResetBits(GPIOB, GPIO_Pin_7)



//IIC所有操作函数
extern void IIC_Init(void);                //初始化IIC的IO口				 
extern void IIC_Start(void);				//发送IIC开始信号
extern void IIC_Stop(void);	  			//发送IIC停止信号
extern void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
extern uint8_t IIC_Read_Byte(uint8_t ack);//IIC读取一个字节
extern uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
extern void IIC_Ack(void);					//IIC发送ACK信号
extern void IIC_NAck(void);				//IIC不发送ACK信号

extern void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
extern uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);
					  
extern uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);							//指定地址读取一个字节
extern void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);		//指定地址写入一个字节

extern uint8_t AT24CXX_ReadOneByte_Page(uint8_t Page,uint16_t ReadAddr);							//指定地址读取一个字节
extern void AT24CXX_WriteOneByte_Page(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite);		//指定地址写入一个字节


extern void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);//指定地址开始写入指定长度的数据
extern uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);					//指定地址开始读取指定长度数据
extern void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	//从指定地址开始写入指定长度的数据
extern void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   	//从指定地址开始读出指定长度的数据

extern uint8_t AT24CXX_Check(void);  //检查器件

#endif
