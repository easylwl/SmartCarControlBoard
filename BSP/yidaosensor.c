/***********************************************************************
文件名称：yindaosensor.C
功    能：
编写时间：2017.4.20
编 写 人：
注    意：
***********************************************************************/

#include "yidaosensor.h"
#include "M_CMSIS_P.h" 
#include "adc.h" 

unsigned char yindaosensor_rec_flag = 0;
unsigned char CAN2_data[8];
unsigned char can2_rec_flag = 0;

void Sensor_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the  Clock */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE); 						 
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE); 						 
	
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void GetSensorV(YindaoSensor * yindaosensor)
{
	
	int total_read_count = 10;
	int read_count = total_read_count;
	int valided_count = 7;
	int cunt_pro_touch_bit_set_count = 0;
	int cunt_vot1_touch_bit_set_count = 0;
	int cunt_vot2_touch_bit_set_count = 0;
	int cunt_vot3_touch_bit_set_count = 0;
	u32 voice_val = 0;
	int last_cunt_index = yindaosensor->current_cunt_index - 1;
	
	if(last_cunt_index < 0)
	{
		last_cunt_index = 19;
	}
	
	int next_cunt_index = yindaosensor->current_cunt_index + 1;
	
	if(next_cunt_index > 19)
	{
		next_cunt_index = 0;
	}
	
	if(yindaosensor->cunt_pro_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_pro_touch[last_cunt_index])
	{
		yindaosensor->cunt_pro_touch_frec -= 1;
	}
	
	if(yindaosensor->cunt_pro_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_pro_touch[next_cunt_index])
	{
		yindaosensor->cunt_pro_touch_frec -= 1;
	}
	
	if(yindaosensor->cunt_vot1_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot1_touch[last_cunt_index])
	{
		yindaosensor->cunt_vot1_touch_frec -= 1;
	}
	
	if(yindaosensor->cunt_vot1_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot1_touch[next_cunt_index])
	{
		yindaosensor->cunt_vot1_touch_frec -= 1;
	}
	
	if(yindaosensor->cunt_vot2_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot2_touch[last_cunt_index])
	{
		yindaosensor->cunt_vot2_touch_frec -= 1;
	}
	
	if(yindaosensor->cunt_vot2_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot2_touch[next_cunt_index])
	{
		yindaosensor->cunt_vot2_touch_frec -= 1;
	}
	
	if(yindaosensor->cunt_vot3_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot3_touch[last_cunt_index])
	{
		yindaosensor->cunt_vot3_touch_frec -= 1;
	}
	
	if(yindaosensor->cunt_vot3_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot3_touch[next_cunt_index])
	{
		yindaosensor->cunt_vot3_touch_frec -= 1;
	}
	
	while(read_count > 0)
	{
		read_count -= 1;
		//Bit_RESET：灯亮；Bit_SET：灯灭
		if (GPIO_ReadInputDataBit(CUNT_SEN_PRO) == Bit_RESET) 
		{
			cunt_pro_touch_bit_set_count += 1;
		}
		
		if (GPIO_ReadInputDataBit(CUNT_SEN_VOT1) == Bit_RESET) 
		{
			cunt_vot1_touch_bit_set_count += 1;
		}
		
		if (GPIO_ReadInputDataBit(CUNT_SEN_VOT2) == Bit_RESET) 
		{
			cunt_vot2_touch_bit_set_count += 1;
		}
		
		if (GPIO_ReadInputDataBit(CUNT_SEN_VOT3) == Bit_RESET) 
		{
			cunt_vot3_touch_bit_set_count += 1;
		}
		
		//voice_val += Get_Adc(ADC_Channel_2);//adc采集，这行代码一直没有返回，所以暂时注释掉
		OSTimeDlyHMSM(0, 0, 0, 1);
	}
	//器官
	if (cunt_pro_touch_bit_set_count >= valided_count) 
	{
		yindaosensor->cunt_pro_touch[yindaosensor->current_cunt_index] = Bit_RESET;
	}
	else  
	{
		yindaosensor->cunt_pro_touch[yindaosensor->current_cunt_index] = Bit_SET;
	}
	
	if (cunt_vot1_touch_bit_set_count >= valided_count) 
	{
		yindaosensor->cunt_vot1_touch[yindaosensor->current_cunt_index] = Bit_RESET;
	}
	else  
	{
		yindaosensor->cunt_vot1_touch[yindaosensor->current_cunt_index] = Bit_SET;
	}
	
	if (cunt_vot2_touch_bit_set_count >= valided_count) 
	{
		yindaosensor->cunt_vot2_touch[yindaosensor->current_cunt_index] = Bit_RESET;
	}
	else  
	{
		yindaosensor->cunt_vot2_touch[yindaosensor->current_cunt_index] = Bit_SET;
	}
	
	if (cunt_vot3_touch_bit_set_count >= valided_count) 
	{
		yindaosensor->cunt_vot3_touch[yindaosensor->current_cunt_index] = Bit_RESET;
	}
	else  
	{
		yindaosensor->cunt_vot3_touch[yindaosensor->current_cunt_index] = Bit_SET;
	}
	
	//胸部
	if (GPIO_ReadInputDataBit(BOOB_SEN_LEFT_PRO) == Bit_RESET) 
	{
		yindaosensor->boob_left_pro_touch = Bit_RESET;
	}
	else  
	{	
		yindaosensor->boob_left_pro_touch = Bit_SET;
	}
	
	if (GPIO_ReadInputDataBit(BOOB_SEN_LEFT_VOT) == Bit_RESET) 
	{
		yindaosensor->boob_left_vot_touch = Bit_RESET;
	}
	else  
	{
		yindaosensor->boob_left_vot_touch = Bit_SET;
	}
	
	if (GPIO_ReadInputDataBit(BOOB_SEN_RIGHT_PRO) == Bit_RESET) 
	{
		yindaosensor->boob_right_pro_touch = Bit_RESET;
	}
	else  
	{
		yindaosensor->boob_right_pro_touch = Bit_SET;
	}
	
	if (GPIO_ReadInputDataBit(BOOB_SEN_RIGHT_VOT) == Bit_RESET) 
	{
		yindaosensor->boob_right_vot_touch = Bit_RESET;
	}
	else  
	{
		yindaosensor->boob_right_vot_touch = Bit_SET;
	}
	//臀部
	/*if (GPIO_ReadInputDataBit(HIP_SEN_LEFT_PRO) == Bit_SET) 
	{
		yindaosensor->hip_left_pro_touch = Bit_SET;
	}
	else  
	{
		yindaosensor->hip_left_pro_touch = Bit_RESET;
	}
	
	if (GPIO_ReadInputDataBit(HIP_SEN_LEFT_VOT) == Bit_SET) 
	{
		yindaosensor->hip_left_vot_touch = Bit_SET;
	}
	else  
	{
		yindaosensor->hip_left_vot_touch = Bit_RESET;
	}
	
	if (GPIO_ReadInputDataBit(HIP_SEN_RIGHT_PRO) == Bit_SET) 
	{
		yindaosensor->hip_right_pro_touch = Bit_SET;
	}
	else  
	{
		yindaosensor->hip_right_pro_touch = Bit_RESET;
	}
	
	if (GPIO_ReadInputDataBit(HIP_SEN_RIGHT_VOT) == Bit_SET) 
	{
		yindaosensor->hip_right_vot_touch = Bit_SET;
	}
	else  
	{
		yindaosensor->hip_right_vot_touch = Bit_RESET;
	}
	*/
	
	//音频
	yindaosensor->voice = voice_val / total_read_count;
	
	if(yindaosensor->cunt_pro_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_pro_touch[last_cunt_index])
	{
		yindaosensor->cunt_pro_touch_frec += 1;
	}
	
	if(yindaosensor->cunt_pro_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_pro_touch[next_cunt_index])
	{
		yindaosensor->cunt_pro_touch_frec += 1;
	}
	
	if(yindaosensor->cunt_vot1_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot1_touch[last_cunt_index])
	{
		yindaosensor->cunt_vot1_touch_frec += 1;
	}
	
	if(yindaosensor->cunt_vot1_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot1_touch[next_cunt_index])
	{
		yindaosensor->cunt_vot1_touch_frec += 1;
	}
	
	if(yindaosensor->cunt_vot2_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot2_touch[last_cunt_index])
	{
		yindaosensor->cunt_vot2_touch_frec += 1;
	}
	
	if(yindaosensor->cunt_vot2_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot2_touch[next_cunt_index])
	{
		yindaosensor->cunt_vot2_touch_frec += 1;
	}
	
	if(yindaosensor->cunt_vot3_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot3_touch[last_cunt_index])
	{
		yindaosensor->cunt_vot3_touch_frec += 1;
	}
	
	if(yindaosensor->cunt_vot3_touch[yindaosensor->current_cunt_index] != yindaosensor->cunt_vot3_touch[next_cunt_index])
	{
		yindaosensor->cunt_vot3_touch_frec += 1;
	}
	
	yindaosensor->current_cunt_index += 1;
	
	if(yindaosensor->current_cunt_index > 19)
	{
		yindaosensor->current_cunt_index = 0;
	}
}

