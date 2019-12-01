/***********************************************************************
文件名称：LED.h
功    能：led  IO初始化
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/

#ifndef _EXCHANGESPEAKER_H_
#define _EXCHANGESPEAKER_H_


#define SetMP3Voice 	 GPIO_ResetBits(GPIOA,GPIO_Pin_8); //MP3��Դ
#define SetBTVoice 	 GPIO_SetBits(GPIOA,GPIO_Pin_8);   //������Դ
void ExchangeSpeaker_Configuration(void);

#endif
