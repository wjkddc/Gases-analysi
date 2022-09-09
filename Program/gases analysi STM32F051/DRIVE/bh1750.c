#include "bh1750.h"
#include "delay.h"

#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

float result_lx=0;
uint8_t BUF[2]={0};
uint16_t result=0;

//��ʼ��IIC2
void IIC2_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  //GPIOB6,B7��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	IIC2_SCL_Hight;
	IIC2_SDA_Hight;		
}

void BH1750_Init(void)
{
	IIC2_Init();
}
//����IIC��ʼ�ź�
void IIC2_Start(void)
{
	SDA2_OUT();     //sda�����
	IIC2_SCL_Hight;
	IIC2_SDA_Hight;
	delay_us(4);
	IIC2_SDA_Low;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC2_SCL_Low;//ǯסI2C���ߣ�׼�����ͻ��������
}	  
//����IICֹͣ�ź�
void IIC2_Stop(void)
{
	SDA2_OUT();//sda�����
	IIC2_SCL_Low;
  IIC2_SDA_Low;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
  IIC2_SCL_Hight;
	IIC2_SDA_Hight;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC2_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA2_IN();      //SDA����Ϊ����  
	IIC2_SDA_Hight;delay_us(1);
	IIC2_SCL_Hight;delay_us(1);	 
	while(READ_SDA2)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC2_Stop();
			return 1;
		}
	}
  IIC2_SCL_Low;	//ʱ�����0   
	return 0;  
} 
//����ACKӦ��
void IIC2_Ack(void)
{
	IIC2_SCL_Low;
	SDA2_OUT();
	IIC2_SDA_Low;
	delay_us(2);
	IIC2_SCL_Hight;
	delay_us(2);
	IIC2_SCL_Low;
}
//������ACKӦ��		    
void IIC2_NAck(void)
{
	IIC2_SCL_Low;
	SDA2_OUT();
	IIC2_SDA_Hight;
	delay_us(2);
	IIC2_SCL_Hight;
	delay_us(2);
	IIC2_SCL_Low;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC2_Send_Byte(uint8_t txd)
{                        
  uint8_t t,dat;  
	SDA2_OUT(); 	    
	IIC2_SCL_Low;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
     //   IIC_SDA=(txd&0x80)>>7;
			dat = (txd&0x80)>>7;
      if(dat) 
			{
				IIC2_SDA_Hight;	
			}
			else
			{
				IIC2_SDA_Low;
			}
      txd<<=1; 	  
			delay_us(2);   
			IIC2_SCL_Hight;
			delay_us(2); 
			IIC2_SCL_Low;
			delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC2_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	SDA2_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
		IIC2_SCL_Low;
		delay_us(2);
		IIC2_SCL_Hight;
		receive<<=1;
		if(READ_SDA2)receive++;   
		delay_us(1); 
   }					 
   if (!ack)
       IIC2_NAck();//����nACK
   else
       IIC2_Ack(); //����ACK   
   return receive;
}

void Cmd_Write_BH1750(uint8_t cmd)
{
    IIC2_Start();                  //��ʼ�ź�
    IIC2_Send_Byte(BH1750_Addr+0);   //�����豸��ַ+д�ź�
//		while(IIC2_Wait_Ack());
  	IIC2_Wait_Ack();
    IIC2_Send_Byte(cmd);    //�ڲ��Ĵ�����ַ
//		while(IIC2_Wait_Ack());
		IIC2_Wait_Ack();
    //BH1750_SendByte(REG_data);    //�ڲ��Ĵ������ݣ�
    IIC2_Stop();                   //����ֹͣ�ź�
		delay_ms(5);
}
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 //power on
	Cmd_Write_BH1750(BH1750_RSET);	//clear
	Cmd_Write_BH1750(BH1750_ONE);  //һ��H�ֱ���ģʽ������120ms��֮���Զ��ϵ�ģʽ  
}
void Read_BH1750(void)
{   	
	IIC2_Start();                 //��ʼ�ź�
	IIC2_Send_Byte(BH1750_Addr+1);//�����豸��ַ+���ź�
//	while(IIC2_Wait_Ack());
	IIC2_Wait_Ack();
	BUF[0]=IIC2_Read_Byte(1);     //����ACK
	BUF[1]=IIC2_Read_Byte(0);     //����NACK

	IIC2_Start();//ֹͣ�ź�
	delay_ms(5);
}
void Convert_BH1750(void)
{	
	result=BUF[0];
	result=(result<<8)+BUF[1];  //�ϳ����ݣ�����������	
	result_lx=(float)result/1.2;
}

void TaskBH1750(void)
{
	 Start_BH1750();  //power on
   delay_ms(180);   //delay 180ms
   Read_BH1750();     //read BUF?
   Convert_BH1750(); //ת�������result_lx	
}

