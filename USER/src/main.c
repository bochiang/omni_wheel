#include"include.h"

int time_flag=0; // ms ��ʱ����
int count1=0;
int count2=0;
int count3=0;
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
	
	TIM_Counter_Init();
	USART1_Config();
  while(1)
  {
    if ( time_flag == 1 ) /* 10ms ʱ�䵽 */
    {
      time_flag = 0;
			USART1_printf( USART1, "count1=%d; count2=%d; count3=%d;\r\n", count1, count2, count3);
    }  
			
  }
}
