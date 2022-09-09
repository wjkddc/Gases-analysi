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

//IO��������
#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB7����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB7���ģʽ

#define  READ_SDA  (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))

#define  IIC_SCL_Hight GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define  IIC_SCL_Low   GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define  IIC_SDA_Hight GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define  IIC_SDA_Low   GPIO_ResetBits(GPIOB, GPIO_Pin_7)



//IIC���в�������
extern void IIC_Init(void);                //��ʼ��IIC��IO��				 
extern void IIC_Start(void);				//����IIC��ʼ�ź�
extern void IIC_Stop(void);	  			//����IICֹͣ�ź�
extern void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
extern uint8_t IIC_Read_Byte(uint8_t ack);//IIC��ȡһ���ֽ�
extern uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
extern void IIC_Ack(void);					//IIC����ACK�ź�
extern void IIC_NAck(void);				//IIC������ACK�ź�

extern void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
extern uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);
					  
extern uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);							//ָ����ַ��ȡһ���ֽ�
extern void AT24CXX_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);		//ָ����ַд��һ���ֽ�

extern uint8_t AT24CXX_ReadOneByte_Page(uint8_t Page,uint16_t ReadAddr);							//ָ����ַ��ȡһ���ֽ�
extern void AT24CXX_WriteOneByte_Page(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite);		//ָ����ַд��һ���ֽ�


extern void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);//ָ����ַ��ʼд��ָ�����ȵ�����
extern uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len);					//ָ����ַ��ʼ��ȡָ����������
extern void AT24CXX_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
extern void AT24CXX_Read(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

extern uint8_t AT24CXX_Check(void);  //�������

#endif
