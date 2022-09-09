#include "stdio.h"
#include "adc.h"
#include "gpio.h"
#include "HMI.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DMA_BUFFER_SIZE     4 
 uint16_t RegularConvData_Tab[DMA_BUFFER_SIZE]={0,0};//缓存数据数组
 uint16_t Gases1[10];
 uint16_t Gases2[10];
 uint16_t	Aver_Gases1;//Gases1平均值
 uint16_t	Aver_Gases2;//Gases2平均值
uint8_t ADC_finish=0;//ADC结束标志
uint8_t AD_index=0; //ADC 转换索引

//=======================传感器显示数据==========================================
////////////气体传感器//////////////////////////////////
//MQ-135空气质量 氨气 硫化物 苯系蒸气（烟雾 其它有害气体）
//MQ-2B 可燃气体 丙烷 烟雾（天然气 可燃蒸气）
//printf("ADC0=%02d   ADC1=%02d\r\n",RegularConvData_Tab[0],RegularConvData_Tab[1]);
uint16_t HMI_gases1;//气体1
uint16_t HMI_gases2;//气体2
//===============================================================================


void ADCInit_gases(void)
{
  ADC_InitTypeDef     ADC_InitStruct;
//	GPIO_InitTypeDef    GPIO_InitStruct;
//	
//	/* Enable  GPIOA clock */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
// /* Configure PA.01  as analog input */
//  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStruct.GPIO_OType =GPIO_OType_OD;
//	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_UP;// GPIO_PuPd_DOWN;//GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIOA, &GPIO_InitStruct);				// PC1,输入时不用设置速率

	 /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
	/* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStruct);
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12 bits 
  ADC_InitStruct.ADC_ContinuousConvMode =ENABLE ; //连续转换模式  DISABLE
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//触发方式
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
  ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;//向上扫描    向下扫描  ADC_ScanDirection_Backward
  ADC_Init(ADC1, &ADC_InitStruct); 
 
//   /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
//   ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_55_5Cycles);  
//   ADC_TempSensorCmd(ENABLE);
  ADC_OverrunModeCmd(ADC1, ENABLE); //使能数据覆盖模式
  /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_0|ADC_Channel_1, ADC_SampleTime_13_5Cycles); //配置采样通道及时间  
//	ADC_VrefintCmd(ENABLE);
	ADC_GetCalibrationFactor(ADC1); /* ADC Calibration 校准*/
  ADC_Cmd(ADC1, ENABLE);  	/* Enable ADC1 */
	/* Wait the ADCEN falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
	ADC_DMACmd(ADC1, ENABLE);	/* 使能DMA*/	
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); //DMA请求模式为循环模式
	/* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
//  ADC_VrefintCmd(ENABLE);
	
	ADC_DMA_Config();
}

void ADC_DMA_Config(void)
{
  DMA_InitTypeDef     DMA_InitStruct; 
	NVIC_InitTypeDef 		nvic_init_structure; 
	/* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	nvic_init_structure.NVIC_IRQChannel = DMA1_Channel1_IRQn;       //??DMA1????  
	nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //????  
	nvic_init_structure.NVIC_IRQChannelPriority = 3;                //?????0  
	NVIC_Init(&nvic_init_structure);  
	
	
	 /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1); //复位
	DMA_InitStruct.DMA_BufferSize =DMA_BUFFER_SIZE;//DMA缓存数组大小
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA方向 外设作为数据源 
	
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&RegularConvData_Tab[0];//缓存数据数组起始地址
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据大小为HalfWord   DMA_MemoryDataSize_Byte
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//;//内存地址递增DMA_MemoryInc_Enable
	
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//取值的外设地址 
	DMA_InitStruct.DMA_PeripheralDataSize =DMA_MemoryDataSize_HalfWord ;//数据大小为HalfWord   DMA_MemoryDataSize_Byte
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址递增禁用  DMA_PeripheralInc_Enable
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular ;//DMA循环模式，即完成后重新开始覆盖
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;//DMA优先级设置为高
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//内存到内存禁用
  DMA_Init(DMA1_Channel1, &DMA_InitStruct);//
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);                  //使能DMA中断
	DMA_ClearITPendingBit(DMA_IT_TC);                                //清除DMA中断
  DMA_Cmd(DMA1_Channel1, ENABLE); /* DMA1 Channel1 enable */
	
}



uint16_t ADC_gases_Analysi(uint16_t *p,uint8_t Leght)
{
	uint8_t i,j;
	uint16_t temp;
	uint16_t Sum=0;
	uint16_t average=0;
	for(i=0;i<Leght-1;i++)	//冒泡法排序
		for(j=i+1;j<Leght;j++)
		{
			if(p[j]<p[i])
			{
				temp=p[j];
				p[j]=p[i];
				p[i]=temp;
			}
		}
//	for(i=0;i<Leght;i++)//---------------------------------打印数组
//	{
//		printf("  %02d",p[i]);
//		if(i==(Leght-1))  printf("\r\n");
//	}
	for(i=2;i<=7;i++)//最大值 最小值两端去掉取平均
	{
		Sum += p[i];
	}
	average=Sum/6;
	return average;
}
							
void DMA1_Channel1_IRQHandler()  
{  
	if(DMA_GetITStatus(DMA_IT_TC))                    
	{  
		if(ADC_finish==0)
		{
			Gases1[AD_index]=RegularConvData_Tab[0];
			Gases2[AD_index]=RegularConvData_Tab[1];
			AD_index++;
		}
		if(AD_index>=10)
		{
			AD_index=0;
			ADC_finish=1;
			//DMA_Cmd(DMA1_Channel1, DISABLE); 重启DMA　数据窜来窜去

//			for(i=0;i<10;i++)//---------------------------------打印数组
//			{
//				if(i==0)  printf("Gases1");
//				printf("  %03d",Gases1[i]);
//				if(i==9)  printf("\r\n");
//			}
//			for(i=0;i<10;i++)//---------------------------------打印数组
//			{
//				if(i==0)  printf("Gases2");
//				printf("  %03d",Gases2[i]);
//				if(i==9)  printf("\r\n");
//			}
		}
	}  
	DMA_ClearITPendingBit(DMA_IT_TC); //清除标志位
}  

//MQ-135空气质量 氨气 硫化物 苯系蒸气（烟雾 其它有害气体）
//MQ-2B 可燃气体 丙烷 烟雾（天然气 可燃蒸气）				
void HMIdisplay_Gases(void)
{
	if((HMI_gases1!= Aver_Gases1)||(HMI_gases2 != Aver_Gases2))//再次数据不相等时，刷新显示
	{
		HMI_gases1	= Aver_Gases1;
		HMI_gases2	= Aver_Gases2;
		sprintf(Data_tmp,"t0.txt=\"%5d\"",HMI_gases1);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"t1.txt=\"%5d\"",HMI_gases2);
		HMI_sendform(USART1,Data_tmp);
	}

}


