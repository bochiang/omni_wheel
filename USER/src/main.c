#include "stm32f10x.h"
#include "timer_init.h"
#include "nvic.h"

volatile u32 time; // ms ��ʱ����

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����	
 */
int main(void)
{
	/* ����ϵͳʱ��Ϊ 72M */  
	SystemInit();


	/* TIM6 ��ʱ���� */
	NVIC_Configuration();
  TIM6_Configuration();
	
	TIM3_PWM_Init();
	PWM1_duty(500);
  PWM2_duty(250);
  PWM3_duty(125);
	
  while(1)
  {
    if ( time == 1000 ) /* 1s ʱ�䵽 */
    {
      time = 0;
    }        
  }
}
