/***********************************************************************
文件名称：LED.h
�?   能：led  IO初始�?
编写时间�?013.4.25
�?�?人：
�?   意：
***********************************************************************/
#include "stdint.h"
#include "stdint.h"

#ifndef _OPTOSWITCH_H_
#define _OPTOSWITCH_H_

#define CUNT_MOUTH_OPTO			GPIOB , GPIO_Pin_0
#define CUNT_EYE_OPTO			GPIOB , GPIO_Pin_1

#define MOUTHOPTO 0
#define EYEOPTO  1

uint8_t GetOptoSwitch(unsigned char ch); //��ȡ���ͨ����ֵ,ch=0 ��ͣ�ch=1 �۾�
void OptoSwitch_Init(void);
#endif
