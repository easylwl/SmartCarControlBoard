/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#ifndef _APP_TASK_MOTOR_H_
#define _APP_TASK_MOTOR_H_

#define TASK_MOTOR_STK_SIZE	400
#define MCQ_SIZE  5 

extern OS_STK Stk_Task_MOTOR[TASK_MOTOR_STK_SIZE];

extern OS_EVENT  *sem_FinishMove_flag;			//完成运动任务信号定义

extern OS_EVENT   *MotorControlMsgQ;
extern MOTOR_MISSION MotorMission[MCQ_SIZE];

typedef enum {
	MOTOR_HStop        =((uint8_t)0x00),
	MOTOR_Sstop,   //减速停止
	MOTOR_GoAhead,         //front
	MOTOR_GoBack,         //back
	MOTOR_MoveLeft,
	MOTOR_MoveRight,
	MOTOR_CWRotate,
	MOTOR_CCWRotate
} MOTOR_CMD;




typedef struct {
	MOTOR_CMD MotorCmd;    //马达动作指令
	int       Speed;
	int       Movestep;
}MOTOR_MISSION;


void Task_Motor(void *pdata);



#endif
