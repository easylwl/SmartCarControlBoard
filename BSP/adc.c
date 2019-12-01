#include "M_Global.h"
#include "adc.h"
#include "Task_VoiceControl.h"
#include "pwm.h"
#include "optoswitch.h"
volatile unsigned char voiceadc_dma_flag = 0;



OS_STK Stk_adc[TASK_TEST_ADC_STK_SIZE];

#if 0
void Voice_adc_Dma_Init(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	

		 
		 
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 

	  DMA_DeInit(DMA2_Stream1);
		
	  while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//�ȴ�DMA������ 
		
	  /* ���� DMA Stream */
	  DMA_InitStructure.DMA_Channel = DMA_Channel_2;	//ͨ��ѡ��
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);//DMA�����ַ
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(&voice_adc_data);//DMA �洢��0��ַ
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//����ģʽ���洢��
	  DMA_InitStructure.DMA_BufferSize = VOICE_ADC_BUFF_SIZE;//���ݴ����� 
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ���:16λ
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢�����ݳ���:16λ
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ѭ��ģʽ
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�е����ȼ�
	  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;		 
	  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	  
	  DMA_Init(DMA2_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream	
	  
	  DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
	  //����DMA�ж�
	  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
      DMA_Cmd(DMA2_Stream7, DISABLE);  
	  
}
#endif

//初始化ADC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PA2 通道2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
  
  ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
}				  





//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	u16 adcdata=0;
	//设置指定ADC的规则组通道，一个序列，采样时间
		    
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待上一次转换结束
  	adcdata=ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
  	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度		
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	return adcdata;
}

#define ADC_Threshold  120
void Task_Adc(void *pdata)
{
	u16 voice_adc_data;
	//Adc_Init();
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
    ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待上一次转换结束
	
	while (1)
	{
		voice_adc_data=Get_Adc(ADC_Channel_2);
		if (speaker_st==Speaker_idle && voice_adc_data>ADC_Threshold)
			{
				speaker_st=Speaker_playingBtVoice;
				SetMoter(MouthMoter,499);	
			}
		else if(speaker_st==Speaker_playingBtVoice && voice_adc_data<=ADC_Threshold)
			{
			speaker_st=Speaker_idle;
			SetMoter(MouthMoter,100);
			while (GetOptoSwitch(MOUTHOPTO)!=Bit_RESET)
				{
					OSTimeDlyHMSM(0, 0,0, 5);
				}
			SetMoter(MouthMoter,0);	
			}		
		
		OSTimeDlyHMSM(0, 0, 0, 10);//10ms
	}
}








