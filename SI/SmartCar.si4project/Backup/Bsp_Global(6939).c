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
#include "exchangespeaker.h"
#include "optoswitch.h"
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
	//ExchangeSpeaker_Configuration();  //切换声源
	NVIC_Configuration();
	//USART_232_Configuration();	
	//RS232_DMA_Init();
	//CAN1_Configuration();
	//CAN2_Configuration();
	//Adc_Init();
	//qj005_uart_init();
	//Sensor_Init();	
	//Motor_pwm_init(500-1,84-1); //84M/84=1Mhz 的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.       
 	//OptoSwitch_Init();
}

