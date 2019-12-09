#include "M_Global.h"
#include <stdio.h>
#include <stdlib.h> //
#include <time.h>
#include "exchangeSpeaker.h"
#include	 "Task_VoiceControl.h"
#include "main.h"
#include "motor.h"
#include "SCI.h"
#include "optoswitch.h"
int current_play_level;
OS_EVENT   *sqMp3PlayMsg;
#define random(x) (rand()%x)
OS_STK Stk_Task_VoiceCtrl[TASK_VOICECTRL_STK_SIZE];

#define MP3_ISBUSY			GPIOB , GPIO_Pin_5



void play_mp3()
{

}


SPEAKER_STATUS speaker_st=Speaker_idle;

/***********************************************************************
函数名称：void Task_VoiceCtrl(void *pdata)
功    能：
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void Task_VoiceControl(void *pdata)
{		 
    unsigned  char  os_err;
//	u8 _isPlayingMp3=0;
    sqMp3PlayMsg=OSMboxCreate(0);

    
	VOICECONTROLCMD voice_cmd;
	OSTimeDlyHMSM(0, 0, 3, 0);//3000ms
	OSSemPost(TaskStartSem);
	while(1)
	{  
		VOICECONTROLCMD *ss;
		ss=OSMboxPend(sqMp3PlayMsg,500,&os_err);   //等待5秒
		if (os_err==OS_ERR_NONE)
			{
			voice_cmd=*ss;
			if (voice_cmd==Star_Play_Mp3 )
				{
				//检测MP3播放状态
				if (GPIO_ReadInputDataBit(MP3_ISBUSY)==Bit_SET) 
					{
						if(next_play_level > current_play_level)
						{
							next_play_level = current_play_level;
							play_mp3();
						}
						else
						{
							next_play_level = 100;
						}
					}
				}

		else
			{
			if (speaker_st!=Speaker_idle &&(GPIO_ReadInputDataBit(MP3_ISBUSY)==Bit_SET) )
				{
				speaker_st=Speaker_idle;
				SetBTVoice;
				}
			}
		}
		OSTimeDlyHMSM(0, 0, 0, 100);//100ms
	}
}


//Íâ²¿ÖÐ¶Ï0·þÎñ³ÌÐò
VOICECONTROLCMD finishmp3=Mp3_play_finish;
void EXTI9_5_IRQHandler(void)
{
//	OS_CPU_SR cpu_sr;
	OSIntEnter();
	OSTimeDlyHMSM(0, 0, 0, 10);//10ms
	if (GPIO_ReadInputDataBit(MP3_ISBUSY)==Bit_SET)  
	{
		//SetMoter(MouthMoter,0);	
		OSMboxPost(sqMp3PlayMsg,&finishmp3);
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line5); //Çå³ýLINE0ÉÏµÄÖÐ¶Ï±êÖ¾Î» 
	OSIntExit();//中断退出，通知ucos，（该句必须加）   
}	 



