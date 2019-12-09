/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include "M_Global.h"
#include "led.h"   
#include "SCI.h"
#include "NVIC.h"  
#include "yidaosensor.h"
#include "adc.h"
#include "motor.h"
#include "steeringengine.h"
#include "exchangespeaker.h"
#include "optoswitch.h"

void UsbLedLight(u8 onoff)
{
	if (onoff>0)
	{
		GPIO_SetBits(GPIOE, (GPIO_Pin_2 | GPIO_Pin_3));	
	}
	else 
	{
		GPIO_ResetBits(GPIOE, (GPIO_Pin_2 | GPIO_Pin_3));	
	}
}
void UsbLedConfiguration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);						 
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE, (GPIO_Pin_2 | GPIO_Pin_3));		
	UsbLedLight(0);
}



/*
 * 功能：
 *       板级初始化
 * 参数：
 *       无
 * 返回：
 *       无
 * 说明：
 *       可用与模块中初始化的尽量使用模块初始化
 */
void BSP_Init(void)
{
	//LED_Configuration();
	UsbLedConfiguration();  
	NVIC_Configuration();
	USART1_Configuration();	
	//USART3_Configuration();
	//RS232_DMA_Init();
	//CAN1_Configuration();
	//CAN2_Configuration();
	//Adc_Init();
	//qj005_uart_init();
	//Sensor_Init();	
	//Motor_pwm_init(500-1,84-1); //84M/84=1Mhz 的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.       
 	//OptoSwitch_Init();
 	MotorInit();
	SteeringEngineInit();
	
}




