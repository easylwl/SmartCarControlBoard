/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include "M_Global.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include "main.h"
#include "motor.h"
#include "Task_Motor.h"

#define random(x) (rand()%x)

OS_STK Stk_Task_MOTOR[TASK_MOTOR_STK_SIZE];
void * ArryOfMotorCtrlMsgQ[MCQ_SIZE];
OS_EVENT   *MotorControlMsgQ;
MOTOR_MISSION MotorMission[MCQ_SIZE];



/***********************************************************************
函数名称：void Task_SenSor(void *pdata)
功    能：
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void Task_Motor(void *pdata)
{		 
    unsigned  char  os_err;
	MOTOR_MISSION *MotorMission_p;
	char  Running=0;
	MotorControlMsgQ=OSQCreate((void**)&ArryOfMotorCtrlMsgQ[0],MCQ_SIZE);
	OSSemPost(TaskStartSem);
	while(1)
	{  
		MotorMission_p=OSQPend((OS_EVENT*  )MotorControlMsgQ,   
                (INT32U     )0,
                (INT8U*     )&os_err);
		
		if (Running==0)
		{
			if (MotorMission_p->Speed)
		}
		
		OSTimeDlyHMSM(0, 0, 0, 50);//50ms
	}
}
