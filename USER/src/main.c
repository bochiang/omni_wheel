#include"include.h"

int time_flag=0; // ms 计时变量
int count1=0;
int count2=0;
int count3=0;
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
	
	TIM_Counter_Init();
	USART1_Config();
  while(1)
  {
    if ( time_flag == 1 ) /* 10ms 时间到 */
    {
      time_flag = 0;
			USART1_printf( USART1, "count1=%d; count2=%d; count3=%d;\r\n", count1, count2, count3);
    }  
			
  }
}
