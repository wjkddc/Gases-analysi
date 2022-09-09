#include "config.h"


/***********************************************
//FlashоƬѡ��
EE_Driver 	==	1
	1.AT24C512	
EE_Driver 	==	2
	2.FM31256
***********************************************/
#if 	EE_Driver 	==	1
	void EE_SAVE(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite)
	{
		AT24CXX_WriteOneByte_Page(Page,WriteAddr,DataToWrite);
	}
	uint8_t EE_GET(uint8_t Page,uint16_t ReadAddr)
	{
		return	AT24CXX_ReadOneByte_Page(Page,ReadAddr);	
	}
#elif	EE_Driver 	==	2
	void EE_SAVE(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite)
	{
		FM31256_WriteOneByte_Page(Page,WriteAddr,DataToWrite);
	}	
	uint8_t EE_GET(uint8_t Page,uint16_t ReadAddr)
	{
		return FM31256_ReadOneByte_Page(Page,ReadAddr);
	}
#else
    //Ĭ�ϰ汾	
#endif
	
	
	
/******************************************ϵͳ���ݻ���*******************************************
*********************************************Page1***********************************************
  EE_SAVE(EE_Page1,EE_Basic_Addarss+0,Address);    //��ʼ����ַ
  EE_SAVE(EE_Page1,EE_Basic_Addarss+1,Group);      //��ʼ����
	EE_SAVE(EE_Page1,EE_Basic_Addarss+2,Relay_Zone); //��ʼ���м���
	EE_SAVE(EE_Page1,EE_Basic_Addarss+3,Relay_Logic);//��ʼ���м��߼�
	EE_SAVE(EE_Page1,EE_Basic_Addarss+4,Relay_En);   //��ʼ���м̱�־ 
	EE_SAVE(EE_Page1,EE_Basic_Addarss+5,Radar_En);//�״�ʹ��      
	EE_SAVE(EE_Page1,EE_Basic_Addarss+5,Radar_Radio_En); //��ʼ���㲥�״��־ 
	EE_SAVE(EE_Page1,EE_Basic_Addarss+6,Radar_Group_En );//��ʼ�����״��־ 
	EE_SAVE(EE_Page1,EE_Basic_Addarss+7,Radar_One_En );  //��ʼ�������״��־ 
//�㲥��ʱʱ
	EE_SAVE(EE_Page1,EE_Basic_Addarss+20,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+21,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+22,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+23,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+24,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+25,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+26,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+27,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+28,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+29,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+30,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+31,TimeData);
	//�㲥��ʱ����
	RadiO.Light1 = EE_GET(EE_Page1,EE_Basic_Addarss+32); //Tim1+bright
	RadiO.Light2 = EE_GET(EE_Page1,EE_Basic_Addarss+33); //Tim2+bright
	RadiO.Light3 = EE_GET(EE_Page1,EE_Basic_Addarss+34); //Tim3+bright
	RadiO.Light4 = EE_GET(EE_Page1,EE_Basic_Addarss+35); //Tim4+bright
	RadiO.Light5 = EE_GET(EE_Page1,EE_Basic_Addarss+36); //Tim5+bright
	RadiO.Light6 = EE_GET(EE_Page1,EE_Basic_Addarss+37); //Tim6+bright
//�鶨ʱʱ��
	EE_SAVE(EE_Page1,EE_Basic_Addarss+38,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+39,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+40,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+41,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+42,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+43,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+44,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+45,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+46,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+47,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+48,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+49,TimeData);
	//�鶨ʱ����
	GrouP.Light1 = EE_GET(EE_Page1,EE_Basic_Addarss+50); //Tim1+bright
	GrouP.Light2 = EE_GET(EE_Page1,EE_Basic_Addarss+51); //Tim2+bright
	GrouP.Light3 = EE_GET(EE_Page1,EE_Basic_Addarss+52); //Tim3+bright
	GrouP.Light4 = EE_GET(EE_Page1,EE_Basic_Addarss+53); //Tim4+bright
	GrouP.Light5 = EE_GET(EE_Page1,EE_Basic_Addarss+54); //Tim5+bright
	GrouP.Light6 = EE_GET(EE_Page1,EE_Basic_Addarss+55); //Tim6+bright
//���ƶ�ʱʱ��
	EE_SAVE(EE_Page1,EE_Basic_Addarss+56,TimeData);//ʱ
	EE_SAVE(EE_Page1,EE_Basic_Addarss+57,TimeData);//��
	EE_SAVE(EE_Page1,EE_Basic_Addarss+58,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+59,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+60,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+61,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+62,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+63,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+64,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+65,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+66,TimeData);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+67,TimeData);
//���ƶ�ʱ����
	SinglE.Light1 = EE_GET(EE_Page1,EE_Basic_Addarss+68); //Tim1+bright
	SinglE.Light2 = EE_GET(EE_Page1,EE_Basic_Addarss+69); //Tim2+bright
	SinglE.Light3 = EE_GET(EE_Page1,EE_Basic_Addarss+70); //Tim3+bright
	SinglE.Light4 = EE_GET(EE_Page1,EE_Basic_Addarss+71); //Tim4+bright
	SinglE.Light5 = EE_GET(EE_Page1,EE_Basic_Addarss+72); //Tim5+bright
	SinglE.Light6 = EE_GET(EE_Page1,EE_Basic_Addarss+73); //Tim6+bright
	//���Ƶ�μ���ֵ
	EE_SAVE(EE_Page1,EE_Basic_Addarss+74,Alarm_Voltage_Max.uni.dataH);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+75,Alarm_Voltage_Max.uni.dataL);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+76,Alarm_Voltage_Min.uni.dataH);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+77,Alarm_Voltage_Min.uni.dataL);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+78,Alarm_Current_Max.uni.dataH);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+79,Alarm_Current_Max.uni.dataL);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+80,Alarm_Current_Min.uni.dataH);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+81,Alarm_Current_Min.uni.dataL);

	EE_SAVE(EE_Page1,EE_Basic_Addarss+82,Voltage>>8);//��ǰ��ѹ
	EE_SAVE(EE_Page1,EE_Basic_Addarss+83,Voltage);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+84,Current>>8);//��ǰ����
	EE_SAVE(EE_Page1,EE_Basic_Addarss+85,Current);	
	EE_SAVE(EE_Page1,EE_Basic_Addarss+86,LampAlarm_En);
	EE_SAVE(EE_Page1,EE_Basic_Addarss+87,Alarm_Flag);//Alarm_Flag






	EE_SAVE(EE_Page1,EE_Basic_Addarss+100,0x5A);//��ʼ����־λ
	
//����Ʒ�豸оƬ24C16 ID
	AT24C16_Id[0]  = EE_GET(EE_Page1,EE_Basic_Addarss+101);
	AT24C16_Id[1]  = EE_GET(EE_Page1,EE_Basic_Addarss+102);
	AT24C16_Id[2]  = EE_GET(EE_Page1,EE_Basic_Addarss+103);
	AT24C16_Id[3]  = EE_GET(EE_Page1,EE_Basic_Addarss+104);
  AT24C16_Id[4]  = EE_GET(EE_Page1,EE_Basic_Addarss+105);
	AT24C16_Id[5]  = EE_GET(EE_Page1,EE_Basic_Addarss+106);
	AT24C16_Id[6]  = EE_GET(EE_Page1,EE_Basic_Addarss+107);
	AT24C16_Id[7]  = EE_GET(EE_Page1,EE_Basic_Addarss+108);
	AT24C16_Id[8]  = EE_GET(EE_Page1,EE_Basic_Addarss+109);
	AT24C16_Id[9]  = EE_GET(EE_Page1,EE_Basic_Addarss+110);
	AT24C16_Id[10] = EE_GET(EE_Page1,EE_Basic_Addarss+111);
	AT24C16_Id[11] = EE_GET(EE_Page1,EE_Basic_Addarss+112);	

	EE_SAVE(EE_Page1,EE_Basic_Addarss+113,0);//ԭ(Page4,0,0)
	
	FM31256_WriteOneByte_Page(0xa0,114,0x01);//����ʱ����±�־λ
	
	
	EE_SAVE(EE_Page1,EE_Basic_Addarss+115,LampCRC_Err.uni.dataH);	//ģ��CRCУ���H
	EE_SAVE(EE_Page1,EE_Basic_Addarss+116,LampCRC_Err.uni.dataL);//ģ��CRCУ���L
****************************************************************************************************/ 
//



/******************************************IAP�������ݻ���*******************************************
	//���µİ汾��
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+0,Uart1_DataBuffer[5]);
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+1,Uart1_DataBuffer[6]);
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+2,Uart1_DataBuffer[7]);
	//���µĹ̼���С
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+3,Uart1_DataBuffer[8]);
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+4,Uart1_DataBuffer[9]);
	//���µĹ̼�CRCУ��
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+5,gsReprogData.CRC16H);
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+6,gsReprogData.CRC16L);
	
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+7,gsReprogData.Finish_Flag);//0:�й̼�����CRCУ����� 1:�й̼�����CRCУ��ͨ�� 2:�̼��Ѹ���
	
	//��ǰϵͳ�汾��
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+8,
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+9,
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+10,
	//��ǰϵͳ�̼���С
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+11
	EE_SAVE(EE_Page1,EE_Reprog_Addarss+12

****************************************************************************************************/ 





