/***********************************************************************
文件名称：yindaosensor.C
�?   能：
编写时间�?017.4.20
�?�?人：
�?   意：
***********************************************************************/
#include "stm32f4xx.h"
#include "optoswitch.h"
#include "M_CMSIS_P.h" 



void OptoSwitch_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the  Clock */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE); 						 
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 );
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//
uint8_t GetOptoSwitch(unsigned char ch)
{
		uint8_t bitstatus = Bit_RESET;
		if (ch==0)  //mouth
		{
			if (GPIO_ReadInputDataBit(CUNT_MOUTH_OPTO) == Bit_RESET) 
			{
				OSTimeDlyHMSM(0, 0, 0, 5);
				if (GPIO_ReadInputDataBit(CUNT_MOUTH_OPTO) == Bit_RESET) 
					bitstatus= Bit_RESET;
			}
			else 
			{
				OSTimeDlyHMSM(0, 0, 0, 5);
				if (GPIO_ReadInputDataBit(CUNT_MOUTH_OPTO) == Bit_SET) 
					bitstatus= Bit_SET;
			}
		}
		else  //eye
		{
			if (GPIO_ReadInputDataBit(CUNT_EYE_OPTO) == Bit_RESET) 
			{
				OSTimeDlyHMSM(0, 0, 0, 5);
				if (GPIO_ReadInputDataBit(CUNT_EYE_OPTO) == Bit_RESET) 
					bitstatus= Bit_RESET;
			}
			else 
			{
				OSTimeDlyHMSM(0, 0, 0, 5);
				if (GPIO_ReadInputDataBit(CUNT_EYE_OPTO) == Bit_SET) 
					bitstatus= Bit_SET;
			}
		}
		return bitstatus;
}

