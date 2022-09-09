/* LCD 通讯接口 ----------------------------------------------------------*/
/* USART1_Tx GPIOA, GPIO_Pin_9-----------------------------------------------*/
/* USART1_Rx GPIOA, GPIO_Pin_10----------------------------------------------*/
/*系统校时:      2015-18-01(5) 10:0:0
  EE 00 00 06 00 0f 0c 12 10 00 00 05 ff 00 00 00 00 00 00 00 EF             */
/*9600bps ------------------------------------------------------------------*/

#include "stdio.h"
#include "gpio.h"
#include "crc16.h"
#include "uart1.h"
#include "delay.h"


/* ---------------------------------------------*/
uint8_t Uart1_Rx_Sta; //正确数据开始起点
uint8_t Uart1_Rx = 0; //接收数据索引号
uint8_t Uart1_Tx = 0;   //发送数据索引号
/* ----------------------------------------------------------------*/
uint16_t Tx1Buffer[TX1BUFFERSIZE] = {0xEE,0x00,0x00,0x1E,0x00,0x64,0x00,0x00,0x00,0x00,
																					0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};//实时开灯

/* Buffer used for reception */
uint16_t Rx1Buffer[RX1BUFFERSIZE]=
{0xEE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEF};
//-帧--中---中---功---地---数---数---数---数---数---数---数---数---数---数---数---数---数--CRC高-CRC低-帧
//-头--继---继---能---址---据---据---据---据---据---据---据---据---据---据---据---据---据--CRC高-CRC低-尾		
//-----域---数---码---
	

////UART1接收中断配置
static void USART1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
}

//USART1 GPIO配置、工作模式配置、9600 8-N-1
void USART1_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	USART_InitTypeDef USART_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);//开启GPIOA的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);//配置映射外设管脚
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);//配置映射外设管脚  
	//USART1 Tx(PA.9)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_NOPULL ;//;GPIO_PuPd_UP
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART1 Rx(PA.10)
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_NOPULL ;//;GPIO_PuPd_UP
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART1 Mode             
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送、接收使能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
	USART_Init(USART1,&USART_InitStructure);	
	//使能串口1接收中断	
	USART1_NVIC_Config();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1,ENABLE);	//使能USART2	
	USART_ClearFlag(USART1, USART_FLAG_TC);
}


void Uart1_TRx(uint8_t ms)
{
	static uint8_t cnt = 0;
	static uint8_t id = 0;	
	if(Uart1_Rx > 0)
	{
		if(cnt != Uart1_Rx)
		{
			cnt = Uart1_Rx;
			id = 0;
		}
		else
		{
			if(id < 20)
			{
				id += ms;
				if(id >= 20)
				{
					Uart1_Rx_Sta = 1;		
				}
			}
		}
	}
	else
	{
		cnt = 0;
	}
}
   
void USART1_IRQHandler(void)
{
	uint8_t Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET ){       
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除中断		

		//UART1_send_byte(Res);
	
		Res = USART_ReceiveData(USART1);
		if(Res==0x55)
		{
			Uart1_Rx=0;
			Rx1Buffer[Uart1_Rx]=Res;
		}
		else{
			Uart1_Rx++;
			Rx1Buffer[Uart1_Rx]=Res;
		}
		Uart1_Rx &= 0xFF;	
		
//		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET ){
//				USART_ClearFlag(USART1,USART_FLAG_ORE);//读SR
//				USART_ReceiveData(USART1);//读DR							
//		  }
		}
}


void UART1_send_byte(uint8_t byte) //发送1字节数据
{
 while(!((USART1->ISR)&(1<<7)));
 USART1->TDR=byte;	
}		

void UART1_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART1->ISR)&(1<<7)));//等待发送完
		USART1->TDR= *Buffer;
		Buffer++;
		Length--;
	}
}

void USART1_SendTxBuf(uint16_t transmitsize,uint16_t *Data)
{ 
	uint8_t i; 
	uint16_t crc_temp;
	crc_temp = CRC16(Tx1Buffer , 18); //MSG_CRC(LCD_TxBuf , 18);
	Data[18]=crc_temp>>8; //CRC H
	Data[19]=crc_temp;		 //CRC L
	//-----------------------
	for(i=0;i<transmitsize;i++)
	{
		USART_SendData(USART1,Data[i]);//aUSARTRF433TxBuf[i]
		/* Loop until the end of transmission */
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{	
		}
	} 

}

void CLS_array(uint16_t *p,uint8_t lenght) //清除数组
{
	uint8_t i;
	for(i=0;i<lenght;i++)
	{
		p[i] = 0x00;
	}
}


