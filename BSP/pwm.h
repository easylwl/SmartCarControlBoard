#ifndef _PWM_H
#define _PWM_H
#include "M_Global.h"
	

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#define EyeMoter 0
#define MouthMoter 1

void Motor_pwm_init(u32 arr,u32 psc);


//speed �ٶ�  1-100
void SetMoter(int moternum,int speed);


#endif
