#include "motor.h"

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void Motor_pwm_init(u32 Freq)
{		 		

	/* TIM1 Configuration ---------------------------------------------------
	 Generate 7 PWM signals with 4 different duty cycles:
	 TIM1 input clock (TIM1CLK) is set to 2 * APB2 clock (PCLK2), since APB2 
	  prescaler is different from 1.   
	  TIM1CLK = 2 * PCLK2  
	  PCLK2 = HCLK / 2 
	  => TIM1CLK = 2 * (HCLK / 2) = HCLK = SystemCoreClock
	 TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
	 SystemCoreClock is set to 168 MHz for STM32F4xx devices
	 
	 The objective is to generate 7 PWM signal at 17.57 KHz:
	   - TIM1_Period = (SystemCoreClock / 17570) - 1
	 The channel 1 and channel 1N duty cycle is set to 50%
	 The channel 2 and channel 2N duty cycle is set to 37.5%
	 The channel 3 and channel 3N duty cycle is set to 25%
	 The channel 4 duty cycle is set to 12.5%
	 The Timer pulse is calculated as follows:
	   - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
	 
	 Note: 
	  SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
	  Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	  function to update SystemCoreClock variable value. Otherwise, any configuration
	  based on this variable will be incorrect. 
	----------------------------------------------------------------------- */
	/* Compute the value to be set in ARR register to generate signal frequency at 17.57 Khz */
	u32 TimerPeriod = (SystemCoreClock / Freq ) - 1;
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
	//Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
	/* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
	//Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
	/* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
	//Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
	/* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
	//Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);



	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOA8复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOA9复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_TIM1); //GPIOA10复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOA11复用为定时器1	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;           //GPIOA8-11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA9

	TIM_TimeBaseStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=TimerPeriod;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4
	
	//TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
 
  	//TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 

	//先关闭pwm
	//TIM_SetCompare1(TIM1,0);	//修改比较值，修改占空比
	//TIM_SetCompare2(TIM1,0);	//修改比较值，修改占空比
	//TIM_SetCompare3(TIM1,0);	//修改比较值，修改占空比
	//TIM_SetCompare4(TIM1,0);	//修改比较值，修改占空比
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
								  
}  

void MotorGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;           //GPIOA8-11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE

	
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
	Motor_pwm_init(5000);
}


