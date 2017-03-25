#include "stm32f10x.h"
#include "timer_init.h"
#include "nvic.h"

volatile u32 time; // ms 计时变量

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无	
 */
int main(void)
{
	/* 配置系统时钟为 72M */  
	SystemInit();


	/* TIM6 定时配置 */
	NVIC_Configuration();
  TIM6_Configuration();
	
	TIM3_PWM_Init();
	PWM1_duty(500);
  PWM2_duty(250);
  PWM3_duty(125);
	
  while(1)
  {
    if ( time == 1000 ) /* 1s 时间到 */
    {
      time = 0;
    }        
  }
}
