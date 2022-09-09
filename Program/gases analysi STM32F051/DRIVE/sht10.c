/*------------------------------------------------------------------
Ϊ��֤������������0.1�棬SHTxx �ļ���ʱ
�䲻Ҫ����10%�����磬��Ӧ12bit ���Ȳ�����
ÿ��������2 �β�������
 ------------------------------------------------------------------*/
 /* Includes ------------------------------------------------------------------*/
#include "sht10.h"
#include "delay.h"
#include "stdio.h"
#include "HMI.h"
#include "PM2_5.h"
/* Private typedef -----------------------------------------------------------*/
 
 
/* Private variables ---------------------------------------------------------*/
enum {TEMP,HUMI};
 
float TempResult; //�¶�ת�����
float HumResult; //ʪ��ת�����

//=======================��������ʾ����==========================================
////////////��������ʾ����  ʵʱ�¶������ʪ��///////////////////////////
//printf("_____________________Temperature:%2.1fC  Humidity:%2.1f%%\r\n",TempResult,HumResult);
float HMI_TempResult; //�¶�
float HMI_HumResult; //ʪ�� 
//===============================================================================  
  
/* Private define ------------------------------------------------------------*/
#define SDA_H()         GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define SDA_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define SCK_H()         GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define SCK_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_6)
//��SDA����
#define SDA_R()         GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)

/* Private macro -------------------------------------------------------------*/
#define noACK 0         //��Ӧ��
#define ACK   1         //Ӧ��

#define STATUS_REG_W    0x06   //000   0011    0 	��״̬�Ĵ���
#define STATUS_REG_R    0x07   //000   0011    1	д״̬�Ĵ���
#define MEASURE_TEMP    0x03   //000   0001    1 �¶Ȳ���
#define MEASURE_HUMI    0x05   //000   0010    1	ʪ�Ȳ���
#define RESET           0x1e   //000   1111    0	��λ


/* Private function prototypes -----------------------------------------------*/
void SHT10_Config(void);
void SHT10_SDAIn(void);
void SHT10_SDAOut(void);
void SHT10_Delay(void);
uint8_t SHT10_WriteByte(uint8_t value);
uint8_t SHT10_ReadByte(uint8_t Ack);
void SHT10_Start(void);
void SHT10_ConReset(void);
uint8_t SHT10_Measure(uint16_t* pValue, uint8_t* pCheckSum, uint8_t mode);
void SHT10_Cal(uint16_t Temp,uint16_t Hum, float* pTempValue,float* pHumValue);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ��ʪ��ת��
  * @param  None
  * @retval None
  */

void SHT10_Datachange(void)
{
	uint16_t TempValue;//�¶Ƚ��  16bit
	uint16_t HumValue;//ʪ�Ƚ�� 16bit
	uint8_t CheckValue = 0x00;    //У��ֵ
	uint8_t error = 0x00;    //����
	
	//	printf("����ת��!\n");
			//SHT10 ����
		SHT10_ConReset();
		//����¶Ⱥ�ʪ�����ݣ�16λ��ʽ
		error += SHT10_Measure(&TempValue,&CheckValue,TEMP);
		error += SHT10_Measure(&HumValue,&CheckValue,HUMI);
		//�¶�ʪ�ȼ��㣬��������ʽ
		SHT10_Cal(TempValue ,HumValue,&TempResult,&HumResult);
		//ͨ������������¶Ⱥ�ʪ������
		//printf("_____________________�¶� %2.1fC ʪ�� %2.1f%%\n",TempResult,HumResult);
}
/**
  * @brief  ��ʱ����
  * @param  ��
  * @retval ��
  */
void SHT10_Delay(void)
{
    //��ʱ������i�д��޸�
//    for(uint16_t i = 500 ; i > 0 ; i--)
//    {
//        ;
//    }
	delay_us(10);
}

/**
  * @brief  ��ʼ��SHT10 IO��
  * @param  None
  * @retval None
  */
void SHT10_gpio(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    //ʹ��GPIOAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA ,ENABLE);
    //PA7 SDA �������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//PA6 SCK �������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// ; GPIO_PuPd_UP GPIO_PuPd_DOWN  GPIO_PuPd_UP
		GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
  * @brief  ����Ϊ����״̬
  * @param  None
  * @retval None 
  */
void SHT10_SDAIn(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //PA7 SDA �������룬�ⲿ����������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//		GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/**
  * @brief  ����Ϊ���״̬
  * @param  None
  * @retval None
  */
void SHT10_SDAOut(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	//PA7 SDA �������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  ����Ϊ���״̬
  * @param  д����
  * @retval Ӧ��
  */
uint8_t SHT10_WriteByte(uint8_t value)
{ 
    uint8_t i,error=0;  
    //SDA���
    SHT10_SDAOut();
    for( i = 0x80 ; i>0 ; i/=2)             
    { 
        if ( i & value) 
            SDA_H();            
        else 
            SDA_L();
        SHT10_Delay();                       
        SCK_H();                          
        SHT10_Delay();                       
        SCK_L();
        SHT10_Delay();                     
    }
    
    //SDA����
    SHT10_SDAIn();
    
    SCK_H();                            
    error = SDA_R();   //��Ӧ��λ                 
    SCK_L(); 
    
    return error;                 
}

/**
  * @brief  ������
  * @param  Ӧ��
  * @retval ��������
  */
uint8_t SHT10_ReadByte(uint8_t Ack)
{ 
    uint8_t i,val=0;
    //����״̬
    SHT10_SDAIn();  
    
    for (i=0x80;i>0;i/=2)         
    { 
        SHT10_Delay();  
        SCK_H();   
        SHT10_Delay();  
        if (SDA_R()) 
            val=(val | i);        //������
        SCK_L();                                           
    }
    
    //���״̬
    SHT10_SDAOut();  
    if(Ack)
        SDA_L();                //Ӧ��Ϊ�͵�ƽ
    else
        SDA_H();

    SHT10_Delay();  
    SCK_H();                     
    SHT10_Delay();  
    SCK_L();
    SHT10_Delay();                                              
    return val;
}

/**
  * @brief  ����
  * @param  ��
  * @retval ��
  */
void SHT10_Start(void)
{  
   //SDA���
    SHT10_SDAOut();
   
    SCK_L();                   
    SHT10_Delay();          
    SCK_H();
    SHT10_Delay();          
    SDA_L();
    SHT10_Delay();          
    SCK_L();  
    SHT10_Delay();          
    SCK_H();
    SHT10_Delay();          
    SDA_H();                   
    SHT10_Delay();          
    SCK_L();                   
}

/**
  * @brief  ��������
  * @param  ��
  * @retval ��
  */
void SHT10_ConReset(void)
{
    uint8_t i;
    //���
    SHT10_SDAOut();
    
    SDA_H();    //����ߵ�ƽ
    SCK_L();
    
    for(i = 0 ; i < 9 ; i++)                  
    { 
        SCK_H();
        SHT10_Delay();
        SCK_L();
        SHT10_Delay();
    }
    
    SHT10_Start();                   
}

/**
  * @brief  �������
  * @param  ��
  * @retval ��
  */
uint8_t SHT10_SoftReset(void)
{ 
    uint8_t error=0;  
    SHT10_ConReset();              
    error += SHT10_WriteByte(RESET);       
    return error;                    
}

/**
  * @brief  �¶Ȼ�ʪ�Ȳ���
  * @param  �¶Ȼ���ʪ��ָ�����ݣ�У��ֵָ�룬ģʽ
  * @retval ����
  */
uint8_t SHT10_Measure(uint16_t* pValue, uint8_t* pCheckSum, uint8_t mode)
{ 
    uint8_t error=0;
    
    uint8_t Value_H = 0;
    uint8_t Value_L = 0;
    
    //����
    SHT10_Start();                
    switch(mode)
    {                     
    case TEMP: 
        error += SHT10_WriteByte(MEASURE_TEMP); 
        break;
    case HUMI: 
        error += SHT10_WriteByte(MEASURE_HUMI); 
        break;
    default: 
        break;         
    }
    
    //SDA��״̬
    SHT10_SDAIn();
    //�ȴ�ת����ɣ������д��Ż�
    while(SDA_R())
    {
        ;
    }

    Value_H = SHT10_ReadByte(ACK);    //����λ
    Value_L = SHT10_ReadByte(ACK);    //����λ
		
    *pCheckSum = SHT10_ReadByte(noACK);  //��У����
    
    //���ؽ��
    *pValue = (Value_H << 8) | Value_L;    
	
    return error;
}


/**
  * @brief  �����¶Ⱥ�ʪ������
  * @param  �¶����� ʪ������ �¶Ƚ�� ʪ�Ƚ��
  * @retval ��
  */
void SHT10_Cal(uint16_t Temp,uint16_t Hum, float* pTempValue,float* pHumValue)
{ 
    const float d1 = -40.1;
    const float d2 = 0.01;
    float Temp_C;
  
    const float C1 = -2.0468;           
    const float C2 = +0.0367;           
    const float C3 = -0.0000015955;     
    const float T1 = +0.01;             
    const float T2 = +0.00008;          
    
		float RH_Lin; //ʪ������ֵ
    float RH_True; //ʪ����ʵֵ
	
	  //�¶Ƚ��������                 
    Temp_C = d1 + d2 * Temp;   
    //RH���Խ��
    RH_Lin = C1 + C2 * Hum + C3 * Hum *Hum;
    RH_True = (Temp_C - 25) * (T1 + T2 * Hum) + RH_Lin;
    //�޶���Χ
    if( RH_True > 100 ) RH_True = 100; 
    if( RH_True < 0.01) RH_True = 0.01;
    
    *pTempValue = Temp_C;
    *pHumValue = RH_True;
    
}

void HMIdisplay_TempHumResult(void)
{
	uint8_t Temp_gei,Temp_shi,Temp_bai;//�¶� �� ʮ ��λ
	uint8_t Hum_gei,Hum_shi,Hum_bai;//ʪ�ȸ� ʮ ��λ
	
	if(HMI_TempResult!= TempResult) //�ٴ����ݲ����ʱ��ˢ����ʾ
	{
		HMI_TempResult = TempResult;
		//��ֵ����
		Temp_bai = ((int)(HMI_TempResult*10))/100;
		Temp_shi = ((int)(HMI_TempResult*10))/10%10;
		Temp_gei = ((int)(HMI_TempResult*10))%10;
		sprintf(Data_tmp,"p2.pic=%d",Temp_bai+6);//ͼƬ·��ƫ��6λ��
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p1.pic=%d",Temp_shi+6);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p0.pic=%d",Temp_gei+6);
		HMI_sendform(USART1,Data_tmp);
		
		//ͼ����ʾ
		if((HMI_TempResult<=27)&&(HMI_TempResult>=18))
		{
			sprintf(Data_tmp,"p10.pic=%d",36);//�¶���
			HMI_sendform(USART1,Data_tmp);
		}
		else{
			sprintf(Data_tmp,"p10.pic=%d",39);//�¶���ʧЧ
			HMI_sendform(USART1,Data_tmp);
		}	
	}
	if(HMI_HumResult != HumResult) //�ٴ����ݲ����ʱ��ˢ����ʾ
	{
		HMI_HumResult  = HumResult;
		//��ֵ����
		Hum_bai = (int)(HMI_HumResult*10)/100;
		Hum_shi	= (int)(HMI_HumResult*10)/10%10;
		Hum_gei = (int)(HMI_HumResult*10)%10;
		sprintf(Data_tmp,"p19.pic=%d",Hum_bai+16);//ͼƬ·��ƫ��16λ��
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p4.pic=%d",Hum_shi+16);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p3.pic=%d",Hum_gei+16);
		HMI_sendform(USART1,Data_tmp);
		
		//ͼ����ʾ
		if((HMI_HumResult>=70))
		{
			sprintf(Data_tmp,"p14.pic=%d",42);//ʪ�ȴ�
			HMI_sendform(USART1,Data_tmp);
		}
		else{
					sprintf(Data_tmp,"p14.pic=%d",39);////ʪ�ȴ�ʧЧ
					HMI_sendform(USART1,Data_tmp);
				}		
	}
	//ͼ����ʾ		
	if((HMI_TempResult>28)||(HMI_HumResult>70)||(HMI_PM2_5_IAQI>=100))
	{
		sprintf(Data_tmp,"p9.pic=%d",38);//ͨ��
		HMI_sendform(USART1,Data_tmp);
	}
	else{
				sprintf(Data_tmp,"p9.pic=%d",39);//ͨ��ʧЧ
				HMI_sendform(USART1,Data_tmp);
			}


}



