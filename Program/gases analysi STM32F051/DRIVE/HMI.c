//智能串口HMI
//TJC4024T032_011R
//3.2in
//400*240
#include "HMI.h"
#include "config.h"	//工程配制参数
#include "delay.h"
#include "rtc.h"
#include "uart1.h"
#include "gpio.h"
#include "sht10.h" //温湿度
#include "adc.h"	 //空气质量 天然气
#include "PM2_5.h" //PM2.5

uint8_t upDatatime_flag=0;//更新日期与时间标志位
RTC_TimeTypeDef RTC_TimeStruct;//实时时间
RTC_DateTypeDef RTC_DateStruct;//实时时期

char Data_tmp[30];//数据缓存
uint8_t Lcd_page=0;


//========================================================================
void HMISend_s(USART_TypeDef* USARTx,char *buf1) //字符串发送函数
{
	uint8_t i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USARTx,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	i++;
		}
	 else 
		return ;
	}
}	
	
void HMISend_b(USART_TypeDef* USARTx,char k)	 //字节发送函数
{		 
	uint8_t i;
	for(i=0;i<3;i++)
	{
	 if(k!=0)
		{
			USART_SendData(USARTx,k);  //发送一个字节
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
		return ;
	 } 
} 
void HMI_sendform(USART_TypeDef* USARTx, char *Data)
{
	HMISend_s(USARTx,Data);//字符串发送函数
	HMISend_b(USARTx,0XFF);	 //固定结尾后缀0XFF
}

//55 AA 02 00 04 06 FF
//5A A5 03 32 30 31 36 2D 31 31 2D 30 33 28 34 29 20 32 33 3A 32 30 7C EF EF EF 
//      	  2  0  1  6 -   1  1  -  0  3  (  4  )     2  3  :  2  0
//55 AA 03 32 30 31 36 2D 31 31 2D 30 33 28 34 29 20 32 33 3A 32 35 3A 31 32 7C 7C EF EF EF 
void page_sele(void)// 页面标记
{
	if((Rx1Buffer[0]==0x55)&&(Rx1Buffer[1]==0xAA))
	{
		if(Rx1Buffer[2]==0x02)
		{
			if(Rx1Buffer[4]==0x00)
			{
				Lcd_page=0;//MENU
			}
			else 
			if(Rx1Buffer[4]==0x01)
			{
				Lcd_page=1;//SET
			}
			else  
			if(Rx1Buffer[4]==0x02)
			{
				Lcd_page=2;//WIFI
			}
			else  if(Rx1Buffer[4]==0x03)
			{
				Lcd_page=3;//NET
			}
			else 
			if(Rx1Buffer[4]==0x04)
			{
				Lcd_page=4;//KEY
			}
		}
			else if(Rx1Buffer[2]==0x03)
						{
							setTime.year	= (Rx1Buffer[5]-0x30)*10+ (Rx1Buffer[6]-0x30);
							setTime.month = (Rx1Buffer[8]-0x30)*10+ (Rx1Buffer[9]-0x30);
							setTime.day 	= (Rx1Buffer[11]-0x30)*10+ (Rx1Buffer[12]-0x30);
							setTime.week 	= (Rx1Buffer[14]-0x30);
							setTime.hour	= (Rx1Buffer[17]-0x30)*10+ (Rx1Buffer[18]-0x30);
							setTime.min		= (Rx1Buffer[20]-0x30)*10+ (Rx1Buffer[21]-0x30);
							upDatatime_flag=1;//设置时间标志位
						}
						else if(Rx1Buffer[2]==0x04)//wifi
									{
										
									}
									else if(Rx1Buffer[2]==0x01)//set
												{
													
												}
	}
}

void HMI_display(void)
{
	switch(Lcd_page)
	{
		case 0://MENU
			{
				//==========================实时温度与相对湿度========================================	
				HMIdisplay_TempHumResult();
				//==========================气体传感器================================================
				HMIdisplay_Gases();
				//==========================PM2.5传感器===============================================
				HMIdisplay_PM2_5();
				if(upDatatime_flag==1)
				{
					upDatatime_flag=0;//清除时间标志位
					RTC_Set_Time(setTime.hour,setTime.min,0,RTC_H12_AM);//设置时间					
					RTC_Set_Date(setTime.year,setTime.month,setTime.day,setTime.week);//设置日期	
				}				
				//==========系统时间 日期===========================================================
#if		EE_Driver		== 	1	//(AT24Cx)	
				RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//获取实时日期
				RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获取实时时间	
				sprintf(Data_tmp,"t3.txt=\"20%02d-%02d-%02d(%d) %02d:%02d:%02d\"",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay,\
																																	RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);	
				HMI_sendform(USART1,Data_tmp);				
#elif EE_Driver		== 	2	//(铁电FM32256)
				Get_Realtime(FM_Datatemp);
				sprintf(Data_tmp,"t3.txt=\"20%02d-%02d-%02d(%d) %02d:%02d:%02d\"",FM_Datatemp[6],FM_Datatemp[5],FM_Datatemp[4],FM_Datatemp[3],\
																																	FM_Datatemp[2],FM_Datatemp[1],FM_Datatemp[0]);
				HMI_sendform(USART1,Data_tmp);
#else
				//默认版本	
#endif				
			}
			break;
		case 1:////SET
			break;
		case 2:////WIFI
			break;
		case 3://NET
			break;
		case 4://KEY
			break;
		default :
			break;
	}	
}



	//=====================Sending data====================	
//	HMISends("t0.txt=\"春风不度玉门关\"");
//	HMI_sendform(USART1, char *Data)
	//=====================Receive data====================		



