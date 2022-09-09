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

//初始化IIC2
void IIC2_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  //GPIOB6,B7初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	IIC2_SCL_Hight;
	IIC2_SDA_Hight;		
}

void BH1750_Init(void)
{
	IIC2_Init();
}
//产生IIC起始信号
void IIC2_Start(void)
{
	SDA2_OUT();     //sda线输出
	IIC2_SCL_Hight;
	IIC2_SDA_Hight;
	delay_us(4);
	IIC2_SDA_Low;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC2_SCL_Low;//钳住I2C总线，准备发送或接收数据
}	  
//产生IIC停止信号
void IIC2_Stop(void)
{
	SDA2_OUT();//sda线输出
	IIC2_SCL_Low;
  IIC2_SDA_Low;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
  IIC2_SCL_Hight;
	IIC2_SDA_Hight;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC2_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA2_IN();      //SDA设置为输入  
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
  IIC2_SCL_Low;	//时钟输出0   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC2_Send_Byte(uint8_t txd)
{                        
  uint8_t t,dat;  
	SDA2_OUT(); 	    
	IIC2_SCL_Low;//拉低时钟开始数据传输
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
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC2_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	SDA2_IN();//SDA设置为输入
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
       IIC2_NAck();//发送nACK
   else
       IIC2_Ack(); //发送ACK   
   return receive;
}

void Cmd_Write_BH1750(uint8_t cmd)
{
    IIC2_Start();                  //起始信号
    IIC2_Send_Byte(BH1750_Addr+0);   //发送设备地址+写信号
//		while(IIC2_Wait_Ack());
  	IIC2_Wait_Ack();
    IIC2_Send_Byte(cmd);    //内部寄存器地址
//		while(IIC2_Wait_Ack());
		IIC2_Wait_Ack();
    //BH1750_SendByte(REG_data);    //内部寄存器数据，
    IIC2_Stop();                   //发送停止信号
		delay_ms(5);
}
void Start_BH1750(void)
{
	Cmd_Write_BH1750(BH1750_ON);	 //power on
	Cmd_Write_BH1750(BH1750_RSET);	//clear
	Cmd_Write_BH1750(BH1750_ONE);  //一次H分辨率模式，至少120ms，之后自动断电模式  
}
void Read_BH1750(void)
{   	
	IIC2_Start();                 //起始信号
	IIC2_Send_Byte(BH1750_Addr+1);//发送设备地址+读信号
//	while(IIC2_Wait_Ack());
	IIC2_Wait_Ack();
	BUF[0]=IIC2_Read_Byte(1);     //发送ACK
	BUF[1]=IIC2_Read_Byte(0);     //发送NACK

	IIC2_Start();//停止信号
	delay_ms(5);
}
void Convert_BH1750(void)
{	
	result=BUF[0];
	result=(result<<8)+BUF[1];  //合成数据，即光照数据	
	result_lx=(float)result/1.2;
}

void TaskBH1750(void)
{
	 Start_BH1750();  //power on
   delay_ms(180);   //delay 180ms
   Read_BH1750();     //read BUF?
   Convert_BH1750(); //转换结果至result_lx	
}

