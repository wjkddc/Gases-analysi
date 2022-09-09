/*************************************************************************
 FM31256是一种基于I2C总线、采用铁电体技术的多功能存储芯片。
 除了非易失存储器外，该器件还具有实时时钟、低电压复位、
 看门狗计数器、非易失性事件计数器、可锁定的串行数字标
 识等多种功能。
 
 FM31256的读/写操作
 FM31256作为从机，集成了两个功能不同的部件，每个部件都可以被独立访问。
 一个是存储器，访问时从机地址的位7～4必须被设置为1010B；
 一个是若要访问实时时钟/处理器伴侣，则从机地址的位7～4必须被设置为1101B。
**************************************************************************/
#include "delay.h"
#include "config.h"
#include "FM31256.h"
#include "uart1.h"


uint8_t Updata_FG;////贮存时间更新标志位
uint8_t FM_Datatemp[7]			=	{	0x32,	0x02,	0x0F,	0x06,		0x03,	0x09,	0x10};
		//时间格式：低字节--高字节  秒，分，	时，	星期，	日，		月，年。




static void Start(void)//产生IIC起始信号
{
  IIC_Start();
}
static void Stop(void)//产生IIC停止信号
{
  IIC_Stop() ;
}

static void Ack(void)
{
   IIC_Wait_Ack();
}
static void WriteByte(uint8_t txd)
{
	IIC_Send_Byte(txd) ;
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
static uint8_t ReadByte(void)
{	uint8_t temp;
	uint8_t ack=0;
	temp=IIC_Read_Byte(ack);
	return temp;
}
static void WriteReg(uint8_t RegAdd,uint8_t Wdata)
{
	IIC_Start();  
	IIC_Send_Byte(REG_WRITE);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(RegAdd& 0x1f);//发送高地址	    
	IIC_Wait_Ack(); 
	IIC_Send_Byte(Wdata);           //进入接收模式			   
	IIC_Wait_Ack();	 	   
  IIC_Stop();//产生一个停止条件	    
}

uint8_t FM_ReadReg(uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start();  
	IIC_Send_Byte(REG_WRITE);	   //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr);//发送高地址	    
	IIC_Wait_Ack(); 
	IIC_Start();  	 	   
	IIC_Send_Byte(REG_READ);           //进入接收模式			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();//产生一个停止条件	    
	return temp;
}

uint8_t ReadFRAM(uint16_t Address)
{
    uint8_t RetValue;
    Start();
    WriteByte(IIC_WRITE);
    Ack();
    WriteByte((uint8_t)(Address>>8));
    Ack();
    WriteByte((uint8_t)(Address));
    Ack();
    Start();
    WriteByte(IIC_READ);
    Ack();
    RetValue = ReadByte();
    IIC_SCL_Hight;delay_us(2);//delay_us(10);
		IIC_SDA_Hight;delay_us(2);//delay_us(10);
		IIC_SCL_Low;
    Stop();
    return RetValue;
}

void WriteFRAM(uint16_t Address,uint8_t Wdata)
{
    Start();
    WriteByte(IIC_WRITE);
    Ack();
    WriteByte((uint8_t)(Address>>8));
    Ack();
    WriteByte((uint8_t)(Address));
    Ack();
    WriteByte(Wdata);
    Ack();
    Stop();
}
uint8_t FM31256_ReadOneByte_Page(uint8_t Page,uint16_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
  IIC_Start();  
//	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
	}
//	else 
//		IIC_Send_Byte(Page+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
	IIC_Wait_Ack(); 
  IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA0+1);           //进入接收模式			   
	IIC_Wait_Ack();	 
  temp=IIC_Read_Byte(0);		   
  IIC_Stop();//产生一个停止条件	    
	return temp;
}

void FM31256_WriteOneByte_Page(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
  IIC_Start();  
//	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}
//	else 
//		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	IIC_Wait_Ack();	   
  IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
  IIC_Stop();//产生一个停止条件 
	delay_ms(10);
}



void Fm31256_init(void)
{
	WriteReg(0x01,0x80);//停止时钟
	WriteReg(CONTROL,0x00);	//启动时钟 
	//时钟更新	
	Updata_FG=FM31256_ReadOneByte_Page(0xA0,EE_Basic_Addarss+114);
	if(Updata_FG !=0X55)
	{
		Set_Realtime(FM_Datatemp);//FM_Datatemp设置FM31256 内部时钟	
		FM31256_WriteOneByte_Page(0xA0,EE_Basic_Addarss+114,0X55);//贮存时间更新标志位
	}	
//WatchDog_Enable(0x1e);//看门狗使能 3000ms	
	WatchDog_Disable();
//--芯片测试--------------------------
//	FM31256_WriteOneByte_Page(0xa0,114,0x54);
//	for(i=0;i<10;i++)
//	{
//		temp=FM31256_ReadOneByte_Page(EE_Page1,FM31_Addarss+101+i);
//		UART1_send_byte(temp);
//	}	
//temp=	ReadFRAM(0x7d);
//temp=FM31256_ReadOneByte_Page(EE_Page1,114);
//UART1_send_byte(temp);
}


void Set_Realtime(uint8_t *realtime)//时间格式：低字节--高字节  秒，分，时，星期，日，月，年。
{
    uint8_t i;
//		WriteReg(0x0B,0x04);	//伴侣控制
//		WriteReg(FLAGS,0x00);   //清除标志位
		WriteReg(FLAGS,0x02);   //允许时间写操作
    for(i=0;i<7;i++)
		{
			WriteReg(0x02+i,hex_to_bcd(realtime[i]));//
			delay_us(10);
		}
    WriteReg(FLAGS,0x00);     //向芯片更新时间
		//WriteReg(FLAGS,0x01);   //允许时间读操作
}

//void Uart1_Set_Realtime(void) //通过串口授时 参考《集中器终端通信协议V0.4》
//{
//	FM_Datatemp[0]=Uart1_DataBuffer[10];	//秒
//	FM_Datatemp[1]=Uart1_DataBuffer[9];		//，分
//	FM_Datatemp[2]=Uart1_DataBuffer[8];		//，时
//	FM_Datatemp[3]=Uart1_DataBuffer[11];	//，星期
//	FM_Datatemp[4]=Uart1_DataBuffer[7];		//，日
//	FM_Datatemp[5]=Uart1_DataBuffer[6];		//，月
//	FM_Datatemp[6]=Uart1_DataBuffer[5];		//，年
//	Set_Realtime(FM_Datatemp);
//}


void Get_Realtime(uint8_t *realtime)//获取时间 格式：低字节--高字节  秒，分，时，星期，日，月，年。
{
    uint8_t i;
//    WriteReg(FLAGS,0x00);          //
    WriteReg(FLAGS,0x01); 	delay_us(50);         //允许时间读操作
    for(i=0;i<7;i++)
		{
			realtime[i]=bcd_to_hex(FM_ReadReg(0x02+i));//
			delay_us(5);
		}	
    WriteReg(FLAGS,0x00);          //芯片时间更新至用户寄存器
}
void WatchDog_Enable(uint8_t timer)//看门狗使能
{
	timer=timer | 0x80;           
  WriteReg(0x0A,timer);         //重置看门狗计数器的数值	
}
void WatchDog_Clear(void)//喂狗 
{
    WriteReg(0x09,0x0A);          
}
void WatchDog_Disable(void)
{
    WriteReg(0x0A,0x00);
}













//=========================================
/*;
* Name: hex_to_bcd;
* Description: 十六进制数转换成BCD码格式;
* Calls: None;
* Input: 需要转换的十六进制数;
* Outputs: 转换后的BCD码
*/
//=======================================
uint8_t hex_to_bcd(uint8_t condata)
{    
	uint8_t temp;    
	uint8_t temp0;    
	uint8_t temp10;    
	temp10=condata/10;    
	temp0=condata%10;    
	temp=(temp10<<4)|temp0;
	return(temp);
}
//=======================================
/*
* Name: bcd_to_hex;
* Description: BCD码格式转换成十六进制数;
* Calls: None//调用：无;
* Input: 需要转换的BCD码;
* Outputs: 转换后的十六进制数
*/
//======================================
uint8_t bcd_to_hex(uint8_t condata)
{
	uint8_t temp;    
	uint8_t temp0;    
	uint8_t temp10;    
	temp10=(condata&0xF0)>>4;    
	temp0=condata&0x0F;    
	temp=temp10*10+temp0;    
	return(temp);
}




