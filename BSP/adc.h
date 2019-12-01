#include "stm32f4xx.h"
#ifndef __ADC_H
#define __ADC_H	

#define TASK_TEST_ADC_STK_SIZE	400


void Task_Adc(void *pdata);						   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值  

void Voice_adc_Dma_Init(void);

extern OS_STK Stk_adc[TASK_TEST_ADC_STK_SIZE];

#endif 















