#include "headfile.h"
#include "Timer_us.h"

unsigned long long TimeCountPeriod = 0;


//ʹ��TIM14�жϼ�ʱ
void Timer_us_init()
{
	TIM_enum tim = TIM_7;

	IRQn_Type irq_index[8] = 
	{
		TIM1_UP_IRQn,
		TIM8_UP_IRQn,
		TIM2_IRQn,
		TIM5_IRQn,
		TIM3_IRQn,
		TIM4_IRQn,
		TIM6_IRQn,
		TIM7_IRQn
	};

	TimeCountPeriod = 0;
	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// ʹ��ʱ��
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));	

	TIM7->ARR = 0xffff;
	TIM7->PSC = 119;
	TIM7->DIER |= TIM_DIER_UI;

	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = irq_index[(tim&0x0f)];									// �ж�ѡ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 & 0x03;			// ���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 & 0x03;						// ���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												// ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);																// ��ʼ���ж�����

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// ʹ�ܶ�ʱ��
}

unsigned long long Timer_us_Get()
{
	return TimeCountPeriod * (unsigned long long)65535 + TIM7->CNT;
}