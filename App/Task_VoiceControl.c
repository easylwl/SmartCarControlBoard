#include "M_Global.h"
#include <stdio.h>
#include <stdlib.h> //
#include <time.h>
#include "exchangeSpeaker.h"
#include	 "Task_VoiceControl.h"
#include "main.h"
#include "pwm.h"
#include "SCI.h"
#include "optoswitch.h"
int current_play_level;
OS_EVENT   *sqMp3PlayMsg;
#define random(x) (rand()%x)
OS_STK Stk_Task_VoiceCtrl[TASK_VOICECTRL_STK_SIZE];

#define MP3_ISBUSY			GPIOB , GPIO_Pin_5



void play_mp3()
{
	if(next_play_level < 100)
	{
		char current_mp3_file[30];
		current_play_level = next_play_level;
		strcpy(current_mp3_file, next_mp3_file);
		send_mp3_command(0x42, current_mp3_file);
		SetMP3Voice;  //切换MP3声源
		SetMoter(MouthMoter,499);		
		speaker_st = Speaker_playingMP3;
	}
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
			else if (voice_cmd==Mp3_play_finish )
				{
				//检测MP3播放状态
				if (GPIO_ReadInputDataBit(MP3_ISBUSY)==Bit_SET) 
					{
					speaker_st=Speaker_finishMP3;
					SetMoter(MouthMoter,100);
					while (GetOptoSwitch(0)!=Bit_RESET)
						{
							OSTimeDlyHMSM(0, 0,0, 5);
						}
					SetMoter(MouthMoter,0);	
					}
				}
			else if (voice_cmd==BT_To_play)
				{
				if (speaker_st==Speaker_idle) 
					{
					speaker_st=Speaker_playingBtVoice;
					SetMoter(MouthMoter,499);
					}
				}
			else if (voice_cmd==Stop_Voice)
				{
				speaker_st=Speaker_idle;
				SetMoter(MouthMoter,100);
				while (GetOptoSwitch(MOUTHOPTO)!=Bit_RESET)
					{
						OSTimeDlyHMSM(0, 0,0, 5);
					}
				SetMoter(MouthMoter,0);	
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



