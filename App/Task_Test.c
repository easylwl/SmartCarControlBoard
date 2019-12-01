/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/

#include "M_Global.h"
#include "Task_Test.h"
#include "led.h"   
#include "SCI.h"
#include "yidaosensor.h"
#include "adc.h"
#include <stdio.h>
#include <stdlib.h> //
#include <time.h>
#include "exchangeSpeaker.h"
#include "Task_VoiceControl.h"
#include "main.h"

#define random(x) (rand()%x)
extern u8 qj005_play_state;
u8 is_play_end = 0;
OS_STK Stk_Task_LED[TASK_TEST_LED_STK_SIZE];
//OS_STK Stk_Task_RS485[TASK_TEST_RS485_STK_SIZE];
//OS_EVENT  *sem_RS485_rec_flag;			//RS485接收完一桢数据信号量定义
OS_STK Stk_Task_RS232[TASK_TEST_RS232_STK_SIZE];
//OS_EVENT  *sem_RS232_rec_flag;			//RS232接收完一桢数据信号量定义
OS_EVENT   *sqSendMsg;
//OS_STK Stk_Task_CAN1[TASK_TEST_CAN1_STK_SIZE];
OS_STK Stk_sensor[TASK_TEST_CAN2_STK_SIZE];
YindaoSensor sensorv;
VOICECONTROLCMD cmdtest = Star_Play_Mp3;

//OS_EVENT  *sem_CAN1_rec_flag;			//CAN1接收完一桢数据信号量定义
//OS_EVENT  *sem_CAN2_rec_flag;			//CAN2接收完一桢数据信号量定义

/***********************************************************************
函数名称：void Task_LED(void *pdata)
功    能：
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void Task_LED(void *pdata)
{		 
    while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 100);//
		GPIO_ToggleBits(LED1);
		OSTimeDlyHMSM(0, 0, 0, 100);//
		GPIO_ToggleBits(LED2);
		OSTimeDlyHMSM(0, 0, 0, 100);//
		GPIO_ToggleBits(LED3);
		OSTimeDlyHMSM(0, 0, 0, 100);//
		GPIO_ToggleBits(LED4);
	}
}

void set_mp3_message(int r, char* file_name,  u8 play_level)
{
	memset(next_mp3_file,0,30);
	next_play_level = play_level;
	
	if(r >= 0)
	{
		sprintf(next_mp3_file, "%s%03d", file_name, r);
	}
	else
	{
		sprintf(next_mp3_file, "%s", file_name);
	}
}

/***********************************************************************
函数名称：void Task_RS485(void *pdata)
功    能：
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/

void Task_RS232(void *pdata)
{		 
//   unsigned  char  os_err=0;
	sqSendMsg=OSMboxCreate(0);
	memset(&sensorv, 1, sizeof(sensorv));
	sensorv.current_cunt_index = 0;
	sensorv.cunt_pro_touch_frec = 0;
	sensorv.cunt_vot1_touch_frec = 0;
	sensorv.cunt_vot2_touch_frec = 0;
	sensorv.cunt_vot3_touch_frec = 0;
	uint64_t sex_go_on_seconds = 0;//
	int sex_go_gap_remain_seconds = 0;//
	int sex_go_gap_seconds = 60;//
	int cunt_depth_last = 0;//
	int cunt_depth_current = 0;//
	int max_cunt_depth = 4;//
	int screaming_cooling_remain_seconds = 0;//
	int screaming_cooling_seconds = 30;//
	int orgasm_cooling_remain_seconds = 0;//
	int orgasm_cooling_seconds = 90;//
	int loop_rate = 20;//每秒钟循环了多少次
	int sex_start_segment_seconds = 60;//
//	u8 play_end_count = 0;//记录播放mp3结束后的时间，比如记录播放mp3结束2秒后切换到播放手机蓝牙
//	u8 mouth_play_count = 0;//嘴巴动作执行的周期数
//	u8 eyelids_play_count = 60;//眼睛动作执行的周期数
	int r = -1;
	
	while(1)
	{
		GetSensorV(&sensorv);
		//OSMboxPost(sqSendMsg,&sensorv);
		r = -1;
	
		//OSMboxPend(sqSendMsg,0,&os_err);	 //持续等待sqSendMsg信号量有效	
		//continue;
		/*
		if(eyelids_play_count > 0)

		sensorv=OSMboxPend(sqSendMsg,0,&os_err);	 //持续等待sqSendMsg信号量有效	

		{
			eyelids_play_count -= 1;
		}
		
		if(next_play_level == 50)
		{
			mouth_play_count += 1;
		}
		//假如嘴巴张合时间为500毫秒
		if(mouth_play_count >= 10 && next_play_level == 50)
		{
			next_play_level = 100;
		}
		
		if(is_play_end == 1)
		{
			play_end_count += 1;
		}
		//2秒后切换音频信号
		if(play_end_count >= 40)
		{
			is_play_end = 0;
			SetBTVoice;//此处为将音频信号切换到手机蓝牙
		}
		
		if(qj005_play_state == 2)
		{
			next_play_level = 100;
			qj005_play_state = 0;
			is_play_end = 1;
			play_end_count = 0;
		}
		*/
		
		
				
		if(sensorv.cunt_pro_touch[sensorv.current_cunt_index] == Bit_RESET)
		{
			cunt_depth_current = 1;
		}
		
		if(sensorv.cunt_vot1_touch[sensorv.current_cunt_index] == Bit_RESET)
		{
			cunt_depth_current = 2;
		}
		
		if(sensorv.cunt_vot2_touch[sensorv.current_cunt_index] == Bit_RESET)
		{
			cunt_depth_current = 3;
		}
		
		if(sensorv.cunt_vot3_touch[sensorv.current_cunt_index] == Bit_RESET)
		{
			cunt_depth_current = 4;
		}
		
		if(cunt_depth_current > 0)
		{
			sex_go_gap_remain_seconds = sex_go_gap_seconds * loop_rate;
		}
		
		
		if(cunt_depth_last != cunt_depth_current && cunt_depth_current == max_cunt_depth && orgasm_cooling_remain_seconds == 0 && sex_go_on_seconds > sex_start_segment_seconds * loop_rate)
		{
			if(next_play_level > 8)
			{
				orgasm_cooling_remain_seconds = orgasm_cooling_seconds * loop_rate;
				set_mp3_message(r, "top-out",  8);
			}
		}
		else if(cunt_depth_last != cunt_depth_current && cunt_depth_current == max_cunt_depth && screaming_cooling_remain_seconds == 0) //
		{
			if(next_play_level > 9)
			{
				screaming_cooling_remain_seconds = screaming_cooling_seconds * loop_rate;
				r = (random(9) + 1);					
				set_mp3_message(r, "t",  9);
			}
		}
		else if(sensorv.cunt_pro_touch_frec >= 4 || sensorv.cunt_vot1_touch_frec >= 4 || sensorv.cunt_vot2_touch_frec >= 4 || sensorv.cunt_vot3_touch_frec >= 4)//
		{
			if(next_play_level > 10)
			{
				r = (random(32) + 1);
				set_mp3_message(r, "f",  10);
			}
		}
		else if(sensorv.cunt_pro_touch_frec > 0 || sensorv.cunt_vot1_touch_frec > 0 || sensorv.cunt_vot2_touch_frec > 0 || sensorv.cunt_vot3_touch_frec > 0)
		{
			if(sex_go_on_seconds < sex_start_segment_seconds * loop_rate)
			{
				if(next_play_level > 10)
				{
					r = (random(13) + 1);
					set_mp3_message(r, "s",  10);
				}
			}
			else
			{
				if(next_play_level > 10)
				{
					r = (random(73) + 1);
					set_mp3_message(r, "h",  10);
				}
			}
		}
		else if(sensorv.boob_left_vot_touch  == Bit_RESET || sensorv.boob_right_vot_touch  == Bit_RESET)//
		{
			if(next_play_level > 10)
			{
				r = (random(7) + 1);
				set_mp3_message(r, "gh",  10);
			}
		}
		else if(sensorv.boob_left_pro_touch  == Bit_RESET || sensorv.boob_right_pro_touch  == Bit_RESET)//
		{
			if(next_play_level > 10)
			{
				r = (random(7) + 1);
				set_mp3_message(r, "g",  10);
			}
		}
		
		
		/*
		if(next_play_level == 100 && sensorv.voice > 1000)
		{
			next_play_level = 50;
			mouth_play_count = 0;
			//嘴巴动作
		}
		
		if(eyelids_play_count == 0 && next_play_level >= 50)
		{
			//眨眼动作
			
			if(next_play_level == 100)
			{
				eyelids_play_count = (random(6) + 1) * 20;//闲时：1-6秒眨眼一次
			}
			else if(next_play_level == 50)
			{
				eyelids_play_count = (random(3) + 1) * 20;//聊天：1-3秒眨眼一次
			}
		}
		*/

		
		cunt_depth_last = cunt_depth_current;	
	
		if(sex_go_gap_remain_seconds > 0)
		{
			sex_go_on_seconds += 1;
			//
			if(sex_go_on_seconds > sex_start_segment_seconds * loop_rate && orgasm_cooling_remain_seconds == 0  
				&& sex_go_gap_remain_seconds == sex_go_gap_seconds * loop_rate - 1)
			{
				if(next_play_level > 8)
				{
					orgasm_cooling_remain_seconds = orgasm_cooling_seconds * loop_rate;
					set_mp3_message(r, "top-out",  8);
				}
			}
			
			sex_go_gap_remain_seconds -= 1;
			//
			if(sex_go_gap_remain_seconds == 0)
			{
				sex_go_on_seconds = 0;
			}
		}
		//
		if(screaming_cooling_remain_seconds > 0)
		{
			screaming_cooling_remain_seconds -= 1;
		}
		//
		if(orgasm_cooling_remain_seconds > 0)
		{
			orgasm_cooling_remain_seconds -= 1;
		}

		if(next_play_level < 100)
		{
			OSMboxPost(sqMp3PlayMsg, &cmdtest);
		}
		
		OSTimeDlyHMSM(0, 0, 0, 50);//50ms
	}
}

/***********************************************************************
函数名称：void Task_SenSor(void *pdata)
功    能：
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void Task_SenSor(void *pdata)
{		 
    unsigned  char  os_err;
	memset(&sensorv,0,sizeof(sensorv));
	OSSemPend(TaskStartSem,0, &os_err);
	while(1)
	{  
		GetSensorV(&sensorv);
		OSMboxPost(sqSendMsg,&sensorv);
		/*if (sensorv.boob_left_pro_touch==Bit_RESET ||
			sensorv.boob_left_vot_touch==Bit_RESET ||
			sensorv.boob_right_pro_touch==Bit_RESET ||
			sensorv.boob_right_vot_touch==Bit_RESET ||
			
			sensorv.hip_left_pro_touch==Bit_RESET ||
			sensorv.hip_left_vot_touch==Bit_RESET ||
			sensorv.hip_right_pro_touch==Bit_RESET ||
			sensorv.hip_right_vot_touch==Bit_RESET 
			)  
			OSMboxPost(sqMp3PlayMsg,&cmdtest);
		*/
		OSTimeDlyHMSM(0, 0, 0, 50);//50ms
	}
}
