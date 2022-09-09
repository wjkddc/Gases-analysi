/* PM2.5ͨѶ�ӿ� ----------------------------------------------------------*/
/* USARTx_Tx GPIOA, GPIO_Pin_2-----------------------------------------------*/
/* USARTx_Rx GPIOA, GPIO_Pin_3----------------------------------------------*/
/* 9600bps ------------------------------------------------------------------*/
#include "uart1.h"
#include "uart2.h"
#include "tim15.h"
#include "delay.h"
#include <stdio.h>
/* ----------------------------------------------------------------*/
uint16_t Tx2Buffer[TX2BUFFERSIZE];
uint16_t Rx2Buffer[RX2BUFFERSIZE]= {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
																					0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//ʵʱ����;

uint8_t Uart2_Rx_Sta; //��ȷ���ݿ�ʼ���
uint8_t Uart2_Rx = 1; //��������������
uint8_t Uart2_Tx = 0;   //��������������
uint8_t Uart2_sync=0;//���ղ�ͬ����־λ
/* --------------------------------------------*/



//UART2�����ж�����
static void USART2_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

//USART2 GPIO���á�����ģʽ���á�9600 8-N-1
void USART2_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	USART_InitTypeDef USART_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);//����GPIOA������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);//����ӳ������ܽ�
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);//����ӳ������ܽ�   	
//USART2 Tx(PA.2)	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART2 Rx(PA.3)
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART2 Mode
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;	//����ż
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���͡�����ʹ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
	USART_Init(USART2,&USART_InitStructure);	
	//ʹ�ܴ���2�����ж�	
	USART2_NVIC_Config();
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2,ENABLE);	//ʹ��USART2	
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

void USART2_IRQHandler(void)
{
	uint8_t Res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
		{
			USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����ж�		
						
			Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
			
//		USART_SendData(USART1, Res);//��ӡ��ͨѶ����
			//A3---------------------------------------------
			if(Res==0x32) 
			{
				Uart2_Rx=0;
				Rx2Buffer[0]=Res;
			}
			else 
				if((Res==0x3D)&&(Uart2_Rx==0))
				{
					Uart2_Rx=1;
					Rx2Buffer[1]=Res;
					Uart2_sync=1;//�յ�ͬ�����
				}
				else
					if(Uart2_sync==1)
					{
						Uart2_Rx++;	
						Rx2Buffer[Uart2_Rx]=Res;
					}
//			//G1---------------------------------------------
//			if(Res==0x42) 
//			{
//				Uart2_Rx=0;
//				Rx2Buffer[0]=Res;
//			}
//			else 
//				if((Res==0x4D)&&(Uart2_Rx==0))
//				{
//					Uart2_Rx=1;
//					Rx2Buffer[1]=Res;
//					Uart2_sync=1;//�յ�ͬ�����
//				}
//				else
//					if(Uart2_sync==1)
//					{
//						Uart2_Rx++;	
//						Rx2Buffer[Uart2_Rx]=Res;
//					}
					
	  }	
}




void UART2_send_byte(uint8_t byte) //����1�ֽ�����
{
 while(!((USART2->ISR)&(1<<7)));
 USART2->TDR=byte;	
}	

void UART2_Send(uint8_t *Buffer, uint32_t Length)//����һ������
{
	while(Length != 0)
	{
		while(!((USART2->ISR)&(1<<7)));//�ȴ�������
		USART2->TDR= *Buffer;
		Buffer++;
		Length--;
	}
}





