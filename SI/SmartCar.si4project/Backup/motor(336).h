#ifndef _MOTOR_H
#define _MOTOR_H
#include "M_Global.h"
	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


typedef enum {
	MOTOR_ST_STOP   =((uint8_t)0x00),
	MOTOR_ST_ACC ,
	MOTOR_ST_RUN,
	MOTOR_ST_DEC
}MOTOR_SPEED_STATUS;  //马达的当前状态



typedef enum {
	MOTOR_IDLE        =((uint8_t)0x00),
	MOTOR_CW       ,   //向前注射
	MOTOR_CCW         //后退
} MOTOR_DIRECTION;



typedef struct {
	MOTOR_DIRECTION MotorDir;    //马达运行的方向
	MOTOR_SPEED_STATUS MotorSpeedStatus;  //马达的速度状态
    uint16_t MotorRuningSpeed;   //马达当前运行的速度
    uint16_t Motor2Speed;     //马达达到运转的速度
    uint8_t  PreABnum;   //之前的AB值
    uint32_t PositionABCounter; //电机的当前位置计数 
    uint32_t Motor2MoveStep;  //马达要运行的步数
    uint8_t ABEncoder_Error;  //马达错误标志
}MOTOR_PARAM;





void Motor_pwm_init(u32 arr);
void MotorInit(void);


//speed 速度  1-100
void SetMoter(int moternum,int speed);
void Goahead(uint32_t speed , uint32_t steps);
void GoaBack(uint32_t speed , uint32_t steps);
void ClockWiseRotate(uint32_t speed , uint32_t steps);
void CounterClockWiseRotate(uint32_t speed , uint32_t steps);
void MoveLeft(uint32_t speed , uint32_t steps);
void MoveRight(uint32_t speed , uint32_t steps);



#endif
