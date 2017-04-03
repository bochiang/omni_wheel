#include "system_init.h"

/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.TIM_Period=1000;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (720 - 1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM6, ENABLE);																		/* ����ʱ�� */   
}

/*
 * ��������TIM3_GPIO_Config
 * ����  ������TIM3�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����36MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
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
  TIM_TimeBaseStructure.TIM_Period = 999;                      //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 3;	               //����Ԥ��Ƶ��4Ԥ��Ƶ����Ϊ18MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;	               //����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	       //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;	                       //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	               //ʹ��ͨ��1

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM2 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;	                       //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	               //ʹ��ͨ��2

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM3 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;	                       //����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	               //ʹ��ͨ��3

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			       // ʹ��TIM3���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                                       //ʹ�ܶ�ʱ��3	
}

/*
 * ��������TIM3_Mode_Config
 * ����  ��TIM3 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); ///ʹ��TIM1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); ///ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); ///ʹ��TIM4ʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructuer.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_12;
	GPIO_InitStructuer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructuer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructuer);
	
	GPIO_InitStructuer.GPIO_Pin =  GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructuer);

	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=0; //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted, 5);//5�β����˲�  �ⲿʱ��ģʽ2
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM1,0);//���ü�����ֵ
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1
	
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=0; //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted, 5);//5�β����˲�  �ⲿʱ��ģʽ2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM2,0);//���ü�����ֵ
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
	
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=0; //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted, 5);//5�β����˲�  �ⲿʱ��ģʽ2
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM4,0);//���ü�����ֵ
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
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
