#include "PM2_5.h"
#include "uart1.h"
#include "uart2.h"
#include "delay.h"
#include "HMI.h"


//=======================传感器显示数据==========================
////////////PM2.5传感器/////////////////////////////////
uint16_t HMI_PM2_5; //PM2.5显示
uint16_t HMI_PM2_5_IAQI;//PM2.5 IAQI空气质量分指数
//===============================================================

uint16_t PM1_0_cf;	//CF=1 标准颗粒物
uint16_t PM2_5_cf;	//CF=1 标准颗粒物
uint16_t PM10_cf;	//CF=1 标准颗粒物

uint16_t PM1_0;
uint16_t PM2_5;
uint16_t PM10;

uint16_t Grain0_3;//0.1L空气中直径0.3um颗粒物个数
uint16_t Grain0_5;//0.1L空气中直径0.3um颗粒物个数
uint16_t Grain1_0;//0.1L空气中直径0.3um颗粒物个数
uint16_t Grain2_5;//0.1L空气中直径0.3um颗粒物个数
uint16_t Grain5_0;//0.1L空气中直径0.3um颗粒物个数
uint16_t Grain10;//0.1L空气中直径0.3um颗粒物个数

uint16_t datacrc=0;//校验码

			
const uint16_t IAQI[8]=	 {0,50,100,150,200,300,400,500};//空气质量分指数
const float PM2_5_24H[8]={0,35,75,115,150,250,350,500};//颗粒物<=2.5 24h平均微克每立方米
	
void PM2_5_analysi(uint16_t *p)
{
	uint8_t j;
	datacrc=0;
//	for(j=0;j<32;j++)
//	{
//		printf(" %2x",p[j]);
//		if(j==31) printf("\r\n");
//	}
	for(j=0;j<30;j++)//前30位校验码和
	{
		datacrc += p[j];
	}
//	printf("datacrc：%2x %2x\r\n",datacrc>>8,datacrc&0XFF);
	if(Uart2_sync==1)
	{
		if(datacrc==((p[30]<<8)+p[31]))
		{
			//A3---------------------------------------------------------------------
			//大气环境下	
			PM1_0 = (p[4]<<8) | p[5];//ug/m3
			PM2_5 = (p[6]<<8) | p[7];//ug/m3
			PM10  = (p[8]<<8) | p[9];//ug/m3
			//颗粒物个数
			Grain0_3	=(p[10]<<8) | p[11];//0.1L空气中直径0.3um颗粒物个数
			Grain0_5	=(p[12]<<8) | p[13];//0.1L空气中直径0.3um颗粒物个数
			Grain1_0	=(p[14]<<8) | p[15];//0.1L空气中直径0.3um颗粒物个数
			Grain2_5	=(p[16]<<8) | p[17];//0.1L空气中直径0.3um颗粒物个数
			Grain5_0	=(p[18]<<8) | p[19];//0.1L空气中直径0.3um颗粒物个数
			Grain10		=(p[20]<<8) | p[21];//0.1L空气中直径0.3um颗粒物个数		
				
//		//G1----------------------------------------------------------------------
//		//CF=1 标准颗粒物
//		PM1_0_cf = (p[4]<<8) | p[5];//ug/m3
//		PM2_5_cf = (p[6]<<8) | p[7];//ug/m3
//		PM10_cf  = (p[8]<<8) | p[9];//ug/m3
//		//大气环境下
//		PM1_0 = (p[10]<<8) | p[11];//ug/m3
//		PM2_5 = (p[12]<<8) | p[13];//ug/m3
//		PM10  = (p[14]<<8) | p[15];//ug/m3
//		//颗粒物个数
//		Grain0_3	=(p[16]<<8) | p[17];//0.1L空气中直径0.3um颗粒物个数
//		Grain0_5	=(p[18]<<8) | p[19];//0.1L空气中直径0.3um颗粒物个数
//		Grain1_0	=(p[20]<<8) | p[21];//0.1L空气中直径0.3um颗粒物个数
//		Grain2_5	=(p[22]<<8) | p[23];//0.1L空气中直径0.3um颗粒物个数
//		Grain5_0	=(p[24]<<8) | p[25];//0.1L空气中直径0.3um颗粒物个数
//		Grain10		=(p[26]<<8) | p[27];//0.1L空气中直径0.3um颗粒物个数			
	
//		printf("PM1_0：%3d  PM2_5：%3d  PM10：%3d\r\n",PM1_0,PM2_5,PM10);	

			CLS_array(p,32); //清除数组	
			Uart2_sync=0;
		}
	}
}

uint16_t IAQI_analysi(uint16_t data) //空气质量分指数转换计算
{
	uint8_t i;
	uint16_t IAQI_temp;
	for(i=0;i<8;i++)
	{
	 if((data > PM2_5_24H[i])&&(data < PM2_5_24H[i+1]))
	 {
		 IAQI_temp =((IAQI[i+1]-IAQI[i])/(PM2_5_24H[i+1]-PM2_5_24H[i]))*(data - PM2_5_24H[i])+IAQI[i];
	 }
	 else if(data == PM2_5_24H[i])
					IAQI_temp = IAQI[i];
						else if(data >= PM2_5_24H[7])
									IAQI_temp = IAQI[7];
					
	}
	return  IAQI_temp;
}

void HMIdisplay_PM2_5(void)
{
	uint8_t pm_gei,pm_shi,pm_bai,pm_qian;//PM2.5个 十 百位 千位
	if(HMI_PM2_5 != PM2_5)
	{
		HMI_PM2_5 = PM2_5;
		//Orange_ON;
		pm_qian =  HMI_PM2_5/1000;
		pm_bai  = (HMI_PM2_5%1000)/100;
		pm_shi	= (HMI_PM2_5%100)/10;
		pm_gei  =  HMI_PM2_5%10;
		sprintf(Data_tmp,"p8.pic=%d",pm_qian+26);//图片路径偏移26位数
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p7.pic=%d",pm_bai+26);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p6.pic=%d",pm_shi+26);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p5.pic=%d",pm_gei+26);
		HMI_sendform(USART1,Data_tmp);
		//==========================PM2.5 的IAQI（空气质量分指数）=============================
		HMI_PM2_5_IAQI = IAQI_analysi(HMI_PM2_5);
		sprintf(Data_tmp,"t2.txt=\"%d\"",HMI_PM2_5_IAQI);
		HMI_sendform(USART1,Data_tmp);
		//图标显示
		if((HMI_PM2_5_IAQI>=100))
		{
			sprintf(Data_tmp,"p11.pic=%d",37);//PM2.5严重
			HMI_sendform(USART1,Data_tmp);
		}
		else{
			sprintf(Data_tmp,"p11.pic=%d",39);//PM2.5严重失效
			HMI_sendform(USART1,Data_tmp);
		}
	}



}

