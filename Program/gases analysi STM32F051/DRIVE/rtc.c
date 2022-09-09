#include "rtc.h"
#include "gpio.h"
#include "delay.h"

struct Datatime setTime;//����ϵͳʱ�仺�� 20 16 11 03 (04) 12��00

//NVIC_InitTypeDef   NVIC_InitStructure;
//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
ErrorStatus RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}
//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:SUCEE(1),�ɹ�
//ERROR(0),�����ʼ��ģʽʧ�� 
ErrorStatus RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week)
{	
	RTC_DateTypeDef RTC_DateTypeInitStructure;	
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;	
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       1,LSE����ʧ��;
//       2,�����ʼ��ģʽʧ��;
void My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
//	uint16_t retry=0X1FFF; 		
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
		
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x32F0)		
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE ����    
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
			{
//			retry++;
//			delay_ms(10);
			}
//		if(retry==0)return 1;		//LSE ����ʧ��. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ�� 
	  RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��			
		RTC_WaitForSynchro();			
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);

		RTC_Set_Time(12,0,0,RTC_H12_AM);	
		RTC_Set_Date(16,11,9,4);					 
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x32F0);
	}

	RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��
	RTC_WaitForSynchro();
//	RTC_ClearFlag(RTC_FLAG_ALRAF);
//	return 0;
}

//void RTC_IRQHandler( void )
//{
//  if( RTC_GetITStatus( RTC_IT_TS ) != RESET )
//    {    
//      RTC_ClearITPendingBit( RTC_IT_TS );
//    }
//}




 












