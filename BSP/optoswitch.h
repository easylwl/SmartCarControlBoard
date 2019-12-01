/***********************************************************************
鏂囦欢鍚嶇О锛歀ED.h
鍔?   鑳斤細led  IO鍒濆鍖?
缂栧啓鏃堕棿锛?013.4.25
缂?鍐?浜猴細
娉?   鎰忥細
***********************************************************************/
#include "stdint.h"
#include "stdint.h"

#ifndef _OPTOSWITCH_H_
#define _OPTOSWITCH_H_

#define CUNT_MOUTH_OPTO			GPIOB , GPIO_Pin_0
#define CUNT_EYE_OPTO			GPIOB , GPIO_Pin_1

#define MOUTHOPTO 0
#define EYEOPTO  1

uint8_t GetOptoSwitch(unsigned char ch); //获取光电通道的值,ch=0 嘴巴，ch=1 眼睛
void OptoSwitch_Init(void);
#endif
