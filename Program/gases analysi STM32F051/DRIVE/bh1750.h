#ifndef __BH1750_H_
#define __BH1750_H_

#include "stm32f0xx.h"


extern uint16_t result;//��ǿ
extern float result_lx;//��ǿ


//IO��������
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;} //PB11����ģʽ
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;} //PB11���ģʽ

#define  READ_SDA2  (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))

#define  IIC2_SCL_Hight GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define  IIC2_SCL_Low   GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define  IIC2_SDA_Hight GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define  IIC2_SDA_Low   GPIO_ResetBits(GPIOB, GPIO_Pin_11)


void BH1750_Init(void);
void IIC2_Start(void);//����IIC��ʼ�ź�
void IIC2_Stop(void); //����IICֹͣ�ź�
void IIC2_Ack(void);  //����ACKӦ��
void IIC2_NAck(void); //������ACKӦ��	
void IIC2_Send_Byte(uint8_t txd);
uint8_t IIC2_Read_Byte(uint8_t ack);
void Cmd_Write_BH1750(uint8_t cmd);
void Start_BH1750(void);
void Read_BH1750(void);
void Convert_BH1750(void);
void TaskBH1750(void);

uint8_t IIC2_Wait_Ack(void);


#endif
