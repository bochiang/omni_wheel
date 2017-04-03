#ifndef TIMER_INIT_H
#define TIMER_INIT_H

#include "stm32f10x.h"


void TIM6_NVIC_Configuration(void);
void TIM6_Configuration(void);

void TIM3_PWM_Init(void);
void PWM1_duty(int duty);
void PWM2_duty(int duty);
void PWM3_duty(int duty);

void TIM_Counter_Init(void);
void Count_get(int *count1, int *count2, int *count3);
void Gpio_init(void);

#endif	/* TIME_TEST_H */
