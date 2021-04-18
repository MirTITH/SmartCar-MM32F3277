/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				main
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#include "headfile.h"

// *************************** ����˵�� ***************************
// 
// ������Ҫ׼����ɿƼ� MM32F3277 ���İ�һ��
// 
// ����������Ҫ׼����ɿƼ� CMSIS-DAP���������� �� ARM���������� һ��
// ����������Ҫ׼�� USBתTTL ģ��һ��
// 
// �����̳�ʼ������ɿƼ� MM32F3277 ���İ��ϵ� UART1-A9/10
// ��ʼ���� TIM1 �����ж� ���ȼ��θ� 0x01
// ��ʼ���� D0 ��Ϊ�ⲿ�ж����� ���ȼ����  0x00
// ��ʼ���� D1 ��ΪGPIO����
// ��ʼ���� B13/H2 ΪGPIO��� ��������LED��Ϊ״̬��ʾ
// 
// ��¼�����̺�ɼ� B13 ��ʶ��LED��˸ �� LED �� TIM1 �ж��п�����˸
// ���� D0 �����󴥷��ⲿ�ж� ��ʱ H2 ��ʶ�� LED ���� B13 ��LEDֹͣ��˸
// ���� D1 ��������ⲿ�ж��˳� H2 Ϩ�� B13 ������˸
// 
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// 
// *************************** ����˵�� ***************************

// **************************** �궨�� ****************************
#define PIT_TIM						TIM_1

#define LED1						B13
#define LED2						H2
#define EXTI_KEY					D0
#define IO_KEY						D1

#define EXTI_KEY_INT				EXTI0_IRQn
#define PIT_TIM_INT					TIM1_UP_IRQn
// **************************** �궨�� ****************************

// **************************** �������� ****************************
uint8 uart1_get_buffer;
uint8 uart2_get_buffer;
bool uart2_interrupt_flag = false;
// **************************** �������� ****************************

// **************************** �������� ****************************
int main(void)
{
	board_init(true);																// ��ʼ�� debug �������

	gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);									// ��ʼ��ΪGPIO���� ��� Ĭ�ϸߵ�ƽ
	gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);									// ��ʼ��ΪGPIO���� ��� Ĭ�ϸߵ�ƽ
	gpio_init(IO_KEY, GPI, GPIO_HIGH, GPI_FLOATING_IN);								// ��ʼ��ΪGPIO���� ���� ����

	exti_interrupt_init(EXTI_KEY, EXTI_Trigger_Rising, 0x00, 0x01);					// ��ʼ��Ϊ�ⲿ�ж����� �����ش��� ���ȼ� 0x01 �θ�
	tim_interrupt_init(PIT_TIM, 1, 0x00, 0x00);										// ��ʼ��TIMΪ�����ж� ����1Hz ���ȼ� 0x00 ���

	nvic_init(EXTI_KEY_INT, 0x00, 0x00, ENABLE);									// �޸� EXTI_KEY_INT �ж����ȼ�Ϊ 0x00 ���
	nvic_init(PIT_TIM_INT, 0x00, 0x01, ENABLE);										// �޸� PIT_TIM_INT �ж����ȼ�Ϊ 0x01 �θ�

	while(1)
	{
	}
}

void exti_interrupt_handler (void)													// �ⲿ�жϴ����� �� EXTI02_IRQHandler �е���
{
	uint8 io_state = 1;																// ������ȡ IO_KEY ��״̬
	gpio_set(LED2, GPIO_LOW);														// ���� LED2 ��LED
	while(io_state)																	// �ж� IO_KEY �Ƿ���
	{
		// ��ʱ�����ⲿ�ж����ȼ���TIM���ڶ�ʱ���жϸ�
		// ���޷�����TIM�����ж�
		// ��D7ֹͣ��˸
		io_state = gpio_get(IO_KEY);												// ��ȡ IO_KEY ״̬
	}
	gpio_set(LED2, GPIO_HIGH);														// Ϩ�� LED2
}

void tim_interrupt_handler (void)													// TIM�����жϴ����� �� TIM1_UP_IRQHandler �е���
{
	static bool io_state = GPIO_LOW;												// LED1 ��״̬��־
	gpio_set(LED1, io_state);														// ���� LED1 ״̬
	io_state = !io_state;															// ��ת LED1 ״̬��־
}
// **************************** �������� ****************************
