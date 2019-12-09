#include "motor.h"

//TIM1 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void Motor_pwm_init(u32 Freq)
{		 		
	/* Compute the prescaler value */
	uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) /Freq ) - 1;

	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOA8复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOA9复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1); //GPIOA10复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOA11复用为定时器1	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;           //GPIOA8-11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA9

	TIM_TimeBaseStructure.TIM_Prescaler=PrescalerValue;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=2000;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High;
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OCIdleState =  TIM_OCIdleState_Set;
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器


	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器


	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器


	
	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 

	//先关闭pwm
	/*
	TIM_SetCompare1(TIM1,400);	//修改比较值，修改占空比
	TIM_SetCompare2(TIM1,400);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM1,400);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM1,400);	//修改比较值，修改占空比
	*/
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	
    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
					  
}  

void MotorGpioInit(void)
{	
	EXTI_InitTypeDef   EXTI_InitStructure;
  	GPIO_InitTypeDef   GPIO_InitStructure;
  	NVIC_InitTypeDef   NVIC_InitStructure;
	  /* Enable SYSCFG clock */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  
	//direction gion pin
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTF时钟	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;           //GPIOA8-11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE

	//encoder pina & pinb
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE

	/* Connect EXTI Line0 to PB0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = (EXTI_Line0 | EXTI_Line1);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0E;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0E;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//speed 速度  1-100
void SetMoter(int moternum,int speed)
{
	if (moternum==1) 
		{
		TIM_SetCompare1(TIM1,speed);	//修改比较值，修改占空比
		}
	else 
		{
		TIM_SetCompare2(TIM1,speed);	//修改比较值，修改占空比
		}
}


void MotorInit(void)
{
	MotorGpioInit();
	Motor_pwm_init(20000000);
}



//读取编码器的状态，返回读取值，AB
//0x00,0x01,0x02,0x03   ,高4位A, 低4位为B
uint8_t ReadABOpticalEncoder(void)
{
	uint8_t ret;
	ret=GPIO_ReadInputData(GPIOB)&0X3;
	return (ret);
}




void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Toggle LED1 */
    
    
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}





#define RIGHTBACK GPIO_Pin_7

#define RIGHTFRON GPIO_Pin_8

#define LEFTFRON GPIO_Pin_9

#define LEFTBACK GPIO_Pin_10

void GoAhead(uint32_t speed , uint32_t steps)
{
	GPIO_SetBits(GPIOE,  ( GPIO_Pin_8 | GPIO_Pin_7 ));
	GPIO_ResetBits(GPIOE,  (GPIO_Pin_9 | GPIO_Pin_10));
	TIM_SetCompare1(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare2(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM1,speed);	//修改比较值，修改占空比
}

void GoaBack(uint32_t speed , uint32_t steps)
{
	GPIO_ResetBits(GPIOE,  ( GPIO_Pin_8 | GPIO_Pin_7 ));
	GPIO_SetBits(GPIOE,  (GPIO_Pin_9 | GPIO_Pin_10));
	TIM_SetCompare1(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare2(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM1,speed);	//修改比较值，修改占空比
}

void ClockWiseRotate(uint32_t speed , uint32_t steps)
{
	GPIO_ResetBits(GPIOE,  ( GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 ));
	TIM_SetCompare1(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare2(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM1,speed);	//修改比较值，修改占空比
}

void CounterClockWiseRotate(uint32_t speed , uint32_t steps)
{
	GPIO_SetBits(GPIOE,  ( GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 ));
	TIM_SetCompare1(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare2(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM1,speed);	//修改比较值，修改占空比
}

void MoveLeft(uint32_t speed , uint32_t steps)
{
	GPIO_ResetBits(GPIOE,  ( GPIO_Pin_8 | GPIO_Pin_9 ));
	GPIO_SetBits(GPIOE,  (GPIO_Pin_7 | GPIO_Pin_10));
	TIM_SetCompare1(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare2(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM1,speed);	//修改比较值，修改占空比
}

void MoveRight(uint32_t speed , uint32_t steps)
{
	GPIO_SetBits(GPIOE,  ( GPIO_Pin_8 | GPIO_Pin_9 ));
	GPIO_ResetBits(GPIOE,  (GPIO_Pin_7 | GPIO_Pin_10));
	TIM_SetCompare1(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare2(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM1,speed);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM1,speed);	//修改比较值，修改占空比
}

