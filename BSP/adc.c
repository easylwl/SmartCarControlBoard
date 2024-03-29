#include "M_Global.h"
#include "adc.h"
#include "Task_VoiceControl.h"
#include "motor.h"
#include "optoswitch.h"
volatile unsigned char voiceadc_dma_flag = 0;



OS_STK Stk_adc[TASK_TEST_ADC_STK_SIZE];

#if 0
void Voice_adc_Dma_Init(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	

		 
		 
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 

	  DMA_DeInit(DMA2_Stream1);
		
	  while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//等待DMA可配置 
		
	  /* 配置 DMA Stream */
	  DMA_InitStructure.DMA_Channel = DMA_Channel_2;	//通道选择
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);//DMA外设地址
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(&voice_adc_data);//DMA 存储器0地址
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设模式到存储器
	  DMA_InitStructure.DMA_BufferSize = VOICE_ADC_BUFF_SIZE;//数据传输量 
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:16位
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度:16位
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 循环模式
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//中等优先级
	  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;		 
	  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	  
	  DMA_Init(DMA2_Stream1, &DMA_InitStructure);//初始化DMA Stream	
	  
	  DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
	  //配置DMA中断
	  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
      DMA_Cmd(DMA2_Stream7, DISABLE);  
	  
}
#endif

//鍒濆鍖朅DC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//浣胯兘GPIOA鏃堕挓
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //浣胯兘ADC1鏃堕挓

  //鍏堝垵濮嬪寲ADC1閫氶亾5 IO鍙�
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PA2 閫氶亾2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//妯℃嫙杈撳叆
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//涓嶅甫涓婁笅鎷�
  GPIO_Init(GPIOA, &GPIO_InitStructure);//鍒濆鍖�  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1澶嶄綅
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//澶嶄綅缁撴潫	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//鐙珛妯″紡
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//涓や釜閲囨牱闃舵涔嬮棿鐨勫欢杩�5涓椂閽�
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA澶辫兘
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//棰勫垎棰�4鍒嗛銆侫DCCLK=PCLK2/4=84/4=21Mhz,ADC鏃堕挓鏈�濂戒笉瑕佽秴杩�36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//鍒濆鍖�
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12浣嶆ā寮�
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//闈炴壂鎻忔ā寮�	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//鍏抽棴杩炵画杞崲
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//绂佹瑙﹀彂妫�娴嬶紝浣跨敤杞欢瑙﹀彂
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//鍙冲榻�	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1涓浆鎹㈠湪瑙勫垯搴忓垪涓� 涔熷氨鏄彧杞崲瑙勫垯搴忓垪1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC鍒濆鍖�
  
  ADC_Cmd(ADC1, ENABLE);//寮�鍚疉D杞崲鍣�	
}				  





//鑾峰緱ADC鍊�
//ch: @ref ADC_channels 
//閫氶亾鍊� 0~16鍙栧�艰寖鍥翠负锛欰DC_Channel_0~ADC_Channel_16
//杩斿洖鍊�:杞崲缁撴灉
u16 Get_Adc(u8 ch)   
{
	u16 adcdata=0;
	//璁剧疆鎸囧畾ADC鐨勮鍒欑粍閫氶亾锛屼竴涓簭鍒楋紝閲囨牱鏃堕棿
		    
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//绛夊緟涓婁竴娆¤浆鎹㈢粨鏉�
  	adcdata=ADC_GetConversionValue(ADC1);	//杩斿洖鏈�杩戜竴娆DC1瑙勫垯缁勭殑杞崲缁撴灉
  	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC閫氶亾,480涓懆鏈�,鎻愰珮閲囨牱鏃堕棿鍙互鎻愰珮绮剧‘搴�		
	ADC_SoftwareStartConv(ADC1);		//浣胯兘鎸囧畾鐨凙DC1鐨勮蒋浠惰浆鎹㈠惎鍔ㄥ姛鑳�	
	return adcdata;
}

#define ADC_Threshold  120
void Task_Adc(void *pdata)
{
	u16 voice_adc_data;
	//Adc_Init();
	//璁剧疆鎸囧畾ADC鐨勮鍒欑粍閫氶亾锛屼竴涓簭鍒楋紝閲囨牱鏃堕棿
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC閫氶亾,480涓懆鏈�,鎻愰珮閲囨牱鏃堕棿鍙互鎻愰珮绮剧‘搴�			    
    ADC_SoftwareStartConv(ADC1);		//浣胯兘鎸囧畾鐨凙DC1鐨勮蒋浠惰浆鎹㈠惎鍔ㄥ姛鑳�	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//绛夊緟涓婁竴娆¤浆鎹㈢粨鏉�
	
	while (1)
	{
		voice_adc_data=Get_Adc(ADC_Channel_2);

		OSTimeDlyHMSM(0, 0, 0, 10);//10ms
	}
}








