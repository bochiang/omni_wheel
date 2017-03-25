#include "nvic.h"

/*
 * ��������NVIC_Configuration
 * ����  ���ж����ȼ�����
 * ����  ����
 * ���  ����	
 */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef    NVIC_InitStructure; 					   //�������������жϵĽṹ�����

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //�ж����ȼ�����  ��ռʽ���ȼ�������Ϊ2λ����Ӧ���ȼ�ռ2λ
	
	//����1�ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			   //ָ���ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;             //ָ����Ӧ���ȼ���1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	               //���ж�
	NVIC_Init(&NVIC_InitStructure);							       //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	//TIM6�ж����ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
