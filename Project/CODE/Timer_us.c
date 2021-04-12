#include "headfile.h"
#include "Timer_us.h"

unsigned long long TimeCountPeriod = 0;


//使用TIM14中断计时
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
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// 使能时钟
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));	

	TIM7->ARR = 0xffff;
	TIM7->PSC = 119;
	TIM7->DIER |= TIM_DIER_UI;

	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = irq_index[(tim&0x0f)];									// 中断选择
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 & 0x03;			// 设置组优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 & 0x03;						// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												// 使能中断
	NVIC_Init(&NVIC_InitStructure);																// 初始化中断配置

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// 使能定时器
}

unsigned long long Timer_us_Get()
{
	return TimeCountPeriod * (unsigned long long)65535 + TIM7->CNT;
}