/***********************************************************************

***********************************************************************/
#include "exchangeSpeaker.h"
#include "M_Global.h"

//Íâ²¿ÖÐ¶Ï³õÊ¼»¯³ÌÐò
//³õÊ¼»¯PE2~4,PA0ÎªÖÐ¶ÏÊäÈë.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//Ê¹ÄÜSYSCFGÊ±ÖÓ
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);//PB5 
	
  /* ÅäÖÃEXTI_Line5 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;//LINE5
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//ÖÐ¶ÏÊÂ¼þ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //ÉÏÉýÑØ´¥·¢ 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//Ê¹ÄÜLINE0
  EXTI_Init(&EXTI_InitStructure);//ÅäÖÃ


  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//Íâ²¿ÖÐ¶Ï0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//ÇÀÕ¼ÓÅÏÈ¼¶0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//×ÓÓÅÏÈ¼¶2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//Ê¹ÄÜÍâ²¿ÖÐ¶ÏÍ¨µÀ
  NVIC_Init(&NVIC_InitStructure);//ÅäÖÃ
}


void ExchangeSpeaker_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_Exchange Clock */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB , ENABLE); 						 
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//GPIO_WriteBit(GPIOB,GPIO_Pin_6,Bit_RESET);
	//GPIO_WriteBit(GPIOB,GPIO_Pin_7,Bit_RESET);
	//change to bluetooth voice
	SetBTVoice;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//enable bt 8002 voice amplify
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);

	//Busy MP3播放状态
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=  GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Busy 状态中断
	EXTIX_Init();
	
}




