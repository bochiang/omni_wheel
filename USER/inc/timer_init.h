#ifndef TIMER_INIT_H
#define TIMER_INIT_H

#include "stm32f10x.h"


void TIM6_NVIC_Configuration(void);
void TIM6_Configuration(void);

void TIM3_PWM_Init(void);
void PWM1_duty(int duty);
void PWM2_duty(int duty);
void PWM3_duty(int duty);

#endif	/* TIME_TEST_H */
