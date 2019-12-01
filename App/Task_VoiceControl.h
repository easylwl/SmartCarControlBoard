/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#ifndef _TASK_VOICECONTROL_H_
#define _TASK_VOICECONTROL_H_

#define TASK_VOICECTRL_STK_SIZE 400
extern OS_STK Stk_Task_VoiceCtrl[TASK_VOICECTRL_STK_SIZE];
extern OS_EVENT   *sqMp3PlayMsg;

	typedef enum SpeakerStatus
	{
		Speaker_idle=0,
		Speaker_playingMP3,
		Speaker_finishMP3,
		Speaker_playingBtVoice
		
	}SPEAKER_STATUS;

	typedef enum VoiceControlCmd
	{
		Finish_Mp3_CMD=0,
		Star_Play_Mp3,
		Mp3_play_finish,
		BT_To_play,
		Stop_Voice,
	}VOICECONTROLCMD;


extern SPEAKER_STATUS speaker_st;
	
	extern void Task_VoiceControl(void *pdata);
	 extern void PlayTfCardMp3File(u16 fileIndex);
	extern void play_mp3(void);
#endif

