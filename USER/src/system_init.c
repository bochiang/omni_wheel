#include "system_init.h"

/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.TIM_Period=1000;		 								/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (720 - 1);				    /* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM6, ENABLE);																		/* 开启时钟 */   
}

/*
 * 函数名：TIM3_GPIO_Config
 * 描述  ：配置TIM3复用输出PWM时用到的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1经过2倍频后作为TIM3的时钟源等于36MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 18 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;                      //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 3;	               //设置预分频：4预分频，即为18MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	               //设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	       //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;	                       //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //当定时器计数值小于CCR1_Val时为高电平

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	               //使能通道1

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM2 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;	                       //设置通道2的电平跳变值，输出另外一个占空比的PWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	               //使能通道2

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM3 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;	                       //设置通道3的电平跳变值，输出另外一个占空比的PWM

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	               //使能通道3

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			       // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                                       //使能定时器3	
}

/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：TIM3 输出PWM信号初始化，只要调用这个函数
 *         TIM3的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM3_PWM_Init(void)
{
	TIM3_GPIO_Config();
	TIM3_Mode_Config();	
}

void TIM_Counter_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructuer;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); ///使能TIM1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); ///使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); ///使能TIM4时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructuer.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_12;
	GPIO_InitStructuer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructuer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructuer);
	
	GPIO_InitStructuer.GPIO_Pin =  GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructuer);

	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=0; //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1
	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted, 5);//5次采样滤波  外部时钟模式2
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM1,0);//设置计数初值
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
	
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=0; //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM1
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted, 5);//5次采样滤波  外部时钟模式2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM2,0);//设置计数初值
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
	
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=0; //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM1
	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted, 5);//5次采样滤波  外部时钟模式2
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM4,0);//设置计数初值
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
}

void Count_get(int *count1, int *count2, int *count3)
{
	TIM_Cmd(TIM1,DISABLE); 	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1)
	{
		*count1	= TIM_GetCounter(TIM1);
	}
	else
	{
		*count1	= -TIM_GetCounter(TIM1);
	}
	TIM_SetCounter(TIM1, 0);
	TIM_Cmd(TIM1,ENABLE);
	
	TIM_Cmd(TIM2,DISABLE); 
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == 1)
	{
		*count2	= TIM_GetCounter(TIM2);
	}
	else
	{
		*count2	= -TIM_GetCounter(TIM2);
	}
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_Cmd(TIM3,DISABLE);
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == 1)
	{
		*count3	= TIM_GetCounter(TIM4);
	}
	else
	{
		*count3	= -TIM_GetCounter(TIM4);
	}
	TIM_SetCounter(TIM4, 0); 
	TIM_Cmd(TIM3,ENABLE);
}

void Gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    //PWM3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;  //PWM1
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4; //PWM2
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;     //Encoder1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;     //Encoder2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;     //Encoder3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


void PWM3_duty(int duty)
{
	if(duty>0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		TIM3->CCR1 = duty;
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		duty = -duty;
		TIM3->CCR1 = duty;
	}
}

void PWM2_duty(int duty)
{
  if(duty>0)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_4);
		TIM3->CCR2 = duty;
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_4);
		duty = -duty;
		TIM3->CCR2 = duty;
	}
}

void PWM1_duty(int duty)
{
  if(duty>0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		TIM3->CCR3 = duty;
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		duty = -duty;
		TIM3->CCR3 = duty;
	}
}
