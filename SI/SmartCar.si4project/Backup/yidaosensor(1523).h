/***********************************************************************
文件名称：LED.h
功    能：led  IO初始化
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
#include "stdint.h"
#ifndef _YINDAOSENSOR_H_
#define _YINDAOSENSOR_H_


#define CUNT_SEN_PRO			GPIOC , GPIO_Pin_6
#define CUNT_SEN_VOT1			GPIOC , GPIO_Pin_7
#define CUNT_SEN_VOT2			GPIOC , GPIO_Pin_8
#define CUNT_SEN_VOT3			GPIOC , GPIO_Pin_9

#define BOOB_SEN_LEFT_PRO			GPIOD , GPIO_Pin_12
#define BOOB_SEN_LEFT_VOT			GPIOD , GPIO_Pin_13
#define BOOB_SEN_RIGHT_PRO			GPIOD , GPIO_Pin_8
#define BOOB_SEN_RIGHT_VOT			GPIOD , GPIO_Pin_9

#define HIP_SEN_LEFT_PRO			GPIOD , GPIO_Pin_14
#define HIP_SEN_LEFT_VOT			GPIOD , GPIO_Pin_15
#define HIP_SEN_RIGHT_PRO			GPIOD , GPIO_Pin_10
#define HIP_SEN_RIGHT_VOT			GPIOD , GPIO_Pin_11

typedef struct YindaoSensor
{
	//器官传感器数据
	int8_t current_cunt_index;
	int8_t cunt_pro_touch[20];
	int8_t cunt_pro_touch_frec;
	int8_t cunt_vot1_touch[20];
	int8_t cunt_vot1_touch_frec;
	int8_t cunt_vot2_touch[20];
	int8_t cunt_vot2_touch_frec;
	int8_t cunt_vot3_touch[20];
	int8_t cunt_vot3_touch_frec;
	//胸部
	int8_t boob_left_pro_touch;
	int8_t boob_left_vot_touch;
	int8_t boob_right_pro_touch;
	int8_t boob_right_vot_touch;
	//臀部
	int8_t hip_left_pro_touch;
	int8_t hip_left_vot_touch;
	int8_t hip_right_pro_touch;
	int8_t hip_right_vot_touch;
	//音频
	int voice;
}YindaoSensor;


#endif
