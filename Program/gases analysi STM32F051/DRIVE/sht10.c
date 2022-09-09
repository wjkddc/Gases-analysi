/*------------------------------------------------------------------
为保证自身温升低于0.1℃，SHTxx 的激活时
间不要超过10%（例如，对应12bit 精度测量，
每秒最多进行2 次测量）。
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
 
float TempResult; //温度转换结果
float HumResult; //湿度转换结果

//=======================传感器显示数据==========================================
////////////传感器显示数据  实时温度与相对湿度///////////////////////////
//printf("_____________________Temperature:%2.1fC  Humidity:%2.1f%%\r\n",TempResult,HumResult);
float HMI_TempResult; //温度
float HMI_HumResult; //湿度 
//===============================================================================  
  
/* Private define ------------------------------------------------------------*/
#define SDA_H()         GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define SDA_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define SCK_H()         GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define SCK_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_6)
//读SDA数据
#define SDA_R()         GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)

/* Private macro -------------------------------------------------------------*/
#define noACK 0         //无应答
#define ACK   1         //应答

#define STATUS_REG_W    0x06   //000   0011    0 	读状态寄存器
#define STATUS_REG_R    0x07   //000   0011    1	写状态寄存器
#define MEASURE_TEMP    0x03   //000   0001    1 温度测量
#define MEASURE_HUMI    0x05   //000   0010    1	湿度测量
#define RESET           0x1e   //000   1111    0	软复位


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
  * @brief  温湿度转换
  * @param  None
  * @retval None
  */

void SHT10_Datachange(void)
{
	uint16_t TempValue;//温度结果  16bit
	uint16_t HumValue;//湿度结果 16bit
	uint8_t CheckValue = 0x00;    //校验值
	uint8_t error = 0x00;    //错误
	
	//	printf("启动转换!\n");
			//SHT10 连接
		SHT10_ConReset();
		//获得温度和湿度数据，16位格式
		error += SHT10_Measure(&TempValue,&CheckValue,TEMP);
		error += SHT10_Measure(&HumValue,&CheckValue,HUMI);
		//温度湿度计算，浮点数形式
		SHT10_Cal(TempValue ,HumValue,&TempResult,&HumResult);
		//通过串口输出，温度和湿度数据
		//printf("_____________________温度 %2.1fC 湿度 %2.1f%%\n",TempResult,HumResult);
}
/**
  * @brief  延时函数
  * @param  无
  * @retval 无
  */
void SHT10_Delay(void)
{
    //延时函数，i有待修改
//    for(uint16_t i = 500 ; i > 0 ; i--)
//    {
//        ;
//    }
	delay_us(10);
}

/**
  * @brief  初始化SHT10 IO口
  * @param  None
  * @retval None
  */
void SHT10_gpio(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    //使能GPIOA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA ,ENABLE);
    //PA7 SDA 推挽输出
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//PA6 SCK 推挽输出
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// ; GPIO_PuPd_UP GPIO_PuPd_DOWN  GPIO_PuPd_UP
		GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
  * @brief  配置为输入状态
  * @param  None
  * @retval None 
  */
void SHT10_SDAIn(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //PA7 SDA 浮动输入，外部有上拉电阻
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//		GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;//GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/**
  * @brief  配置为输出状态
  * @param  None
  * @retval None
  */
void SHT10_SDAOut(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	//PA7 SDA 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  配置为输出状态
  * @param  写数据
  * @retval 应答
  */
uint8_t SHT10_WriteByte(uint8_t value)
{ 
    uint8_t i,error=0;  
    //SDA输出
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
    
    //SDA输入
    SHT10_SDAIn();
    
    SCK_H();                            
    error = SDA_R();   //读应答位                 
    SCK_L(); 
    
    return error;                 
}

/**
  * @brief  读数据
  * @param  应答
  * @retval 返回数据
  */
uint8_t SHT10_ReadByte(uint8_t Ack)
{ 
    uint8_t i,val=0;
    //输入状态
    SHT10_SDAIn();  
    
    for (i=0x80;i>0;i/=2)         
    { 
        SHT10_Delay();  
        SCK_H();   
        SHT10_Delay();  
        if (SDA_R()) 
            val=(val | i);        //读数据
        SCK_L();                                           
    }
    
    //输出状态
    SHT10_SDAOut();  
    if(Ack)
        SDA_L();                //应答为低电平
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
  * @brief  启动
  * @param  无
  * @retval 无
  */
void SHT10_Start(void)
{  
   //SDA输出
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
  * @brief  重新连接
  * @param  无
  * @retval 无
  */
void SHT10_ConReset(void)
{
    uint8_t i;
    //输出
    SHT10_SDAOut();
    
    SDA_H();    //输出高电平
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
  * @brief  软件重启
  * @param  无
  * @retval 无
  */
uint8_t SHT10_SoftReset(void)
{ 
    uint8_t error=0;  
    SHT10_ConReset();              
    error += SHT10_WriteByte(RESET);       
    return error;                    
}

/**
  * @brief  温度或湿度测量
  * @param  温度或者湿度指针数据，校验值指针，模式
  * @retval 错误
  */
uint8_t SHT10_Measure(uint16_t* pValue, uint8_t* pCheckSum, uint8_t mode)
{ 
    uint8_t error=0;
    
    uint8_t Value_H = 0;
    uint8_t Value_L = 0;
    
    //启动
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
    
    //SDA读状态
    SHT10_SDAIn();
    //等待转换完成，代码有待优化
    while(SDA_R())
    {
        ;
    }

    Value_H = SHT10_ReadByte(ACK);    //读高位
    Value_L = SHT10_ReadByte(ACK);    //读低位
		
    *pCheckSum = SHT10_ReadByte(noACK);  //读校验结果
    
    //返回结果
    *pValue = (Value_H << 8) | Value_L;    
	
    return error;
}


/**
  * @brief  计算温度和湿度数据
  * @param  温度数据 湿度数据 温度结果 湿度结果
  * @retval 无
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
    
		float RH_Lin; //湿度线性值
    float RH_True; //湿度真实值
	
	  //温度结果，换算                 
    Temp_C = d1 + d2 * Temp;   
    //RH线性结果
    RH_Lin = C1 + C2 * Hum + C3 * Hum *Hum;
    RH_True = (Temp_C - 25) * (T1 + T2 * Hum) + RH_Lin;
    //限定范围
    if( RH_True > 100 ) RH_True = 100; 
    if( RH_True < 0.01) RH_True = 0.01;
    
    *pTempValue = Temp_C;
    *pHumValue = RH_True;
    
}

void HMIdisplay_TempHumResult(void)
{
	uint8_t Temp_gei,Temp_shi,Temp_bai;//温度 个 十 百位
	uint8_t Hum_gei,Hum_shi,Hum_bai;//湿度个 十 百位
	
	if(HMI_TempResult!= TempResult) //再次数据不相等时，刷新显示
	{
		HMI_TempResult = TempResult;
		//数值更新
		Temp_bai = ((int)(HMI_TempResult*10))/100;
		Temp_shi = ((int)(HMI_TempResult*10))/10%10;
		Temp_gei = ((int)(HMI_TempResult*10))%10;
		sprintf(Data_tmp,"p2.pic=%d",Temp_bai+6);//图片路径偏移6位数
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p1.pic=%d",Temp_shi+6);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p0.pic=%d",Temp_gei+6);
		HMI_sendform(USART1,Data_tmp);
		
		//图标显示
		if((HMI_TempResult<=27)&&(HMI_TempResult>=18))
		{
			sprintf(Data_tmp,"p10.pic=%d",36);//温度良
			HMI_sendform(USART1,Data_tmp);
		}
		else{
			sprintf(Data_tmp,"p10.pic=%d",39);//温度良失效
			HMI_sendform(USART1,Data_tmp);
		}	
	}
	if(HMI_HumResult != HumResult) //再次数据不相等时，刷新显示
	{
		HMI_HumResult  = HumResult;
		//数值更新
		Hum_bai = (int)(HMI_HumResult*10)/100;
		Hum_shi	= (int)(HMI_HumResult*10)/10%10;
		Hum_gei = (int)(HMI_HumResult*10)%10;
		sprintf(Data_tmp,"p19.pic=%d",Hum_bai+16);//图片路径偏移16位数
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p4.pic=%d",Hum_shi+16);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"p3.pic=%d",Hum_gei+16);
		HMI_sendform(USART1,Data_tmp);
		
		//图标显示
		if((HMI_HumResult>=70))
		{
			sprintf(Data_tmp,"p14.pic=%d",42);//湿度大
			HMI_sendform(USART1,Data_tmp);
		}
		else{
					sprintf(Data_tmp,"p14.pic=%d",39);////湿度大失效
					HMI_sendform(USART1,Data_tmp);
				}		
	}
	//图标显示		
	if((HMI_TempResult>28)||(HMI_HumResult>70)||(HMI_PM2_5_IAQI>=100))
	{
		sprintf(Data_tmp,"p9.pic=%d",38);//通风
		HMI_sendform(USART1,Data_tmp);
	}
	else{
				sprintf(Data_tmp,"p9.pic=%d",39);//通风失效
				HMI_sendform(USART1,Data_tmp);
			}


}



