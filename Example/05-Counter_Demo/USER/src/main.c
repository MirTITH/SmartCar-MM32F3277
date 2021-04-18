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
// 
// �����̳�ʼ������ɿƼ� MM32F3277 ���İ��ϵ� TIM_3/4 ������ʱ����Ϊ�������ɼ�����
// �����������ڶ�Ӧ�� TIM_3-B04/B05  TIM_4-B06/B07
// ���� SOFT_SIM_ENCODER ����ʱ ��ʼ���� A0/1 ����������Ϊģ��������������
// �� A0/1 ���ڶ�Ӧ�� TIM_3-B04/B05 ���� TIM_4-B06/B07
// 
// ��¼�����̺� ��������ϵ��ɼ��Ӵ�������ı������ɼ���Ϣ
// 
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// 
// *************************** ����˵�� ***************************

// **************************** �궨�� ****************************
#define COUNTER1_TIM		TIM_3
#define COUNTER1_PLUS		TIM_3_ENC1_B04
#define COUNTER1_DIR		B5

#define COUNTER2_TIM		TIM_4
#define COUNTER2_PLUS		TIM_4_ENC1_B06
#define COUNTER2_DIR		B7

#define SOFT_SIM_ENCODER			1												// 1-����ģ����� ʹ��GPIOģ����������� 0-�ر�ģ����� ʹ�ñ�����ʵ�����
#if SOFT_SIM_ENCODER
#define SOFT_PLUS_TIM		TIM_5
#define SOFT_PLUS_PIN		TIM_5_CH1_A00
#define SOFT_DIR_PIN		A1
#endif
// **************************** �궨�� ****************************

// **************************** �������� ****************************
int16 counter1_data = 0;
int16 counter2_data = 0;
// **************************** �������� ****************************

// **************************** �������� ****************************
#if SOFT_SIM_ENCODER
void soft_dir_positive (void);
void soft_dir_inverted (void);
#endif

int main(void)
{
	board_init(true);																// ��ʼ�� debug �������

	tim_counter_init(COUNTER1_TIM, COUNTER1_PLUS);									// ��ʼ���ⲿ����������ɼ�
	tim_counter_init(COUNTER2_TIM, COUNTER2_PLUS);									// ��ʼ���ⲿ����������ɼ�

	gpio_init(COUNTER1_DIR, GPI, 1, GPI_PULL_UP);
	gpio_init(COUNTER2_DIR, GPI, 1, GPI_PULL_UP);

	tim_interrupt_init_ms(TIM_8, 10, 0x00, 0x00);

#if SOFT_SIM_ENCODER
	pwm_init(SOFT_PLUS_TIM, SOFT_PLUS_PIN, 5000, PWM_DUTY_MAX/100*50);				// ��ʼ�� GPIO ����ģ�����������
	gpio_init(SOFT_DIR_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);							// ��ʼ�� GPIO ����ģ�����������
#endif

	while(1)
	{
#if SOFT_SIM_ENCODER
		soft_dir_positive();
		printf("\r\nENCODER_L encoder counter: %d.", counter1_data);				// ��������ɼ�������
		printf("\r\nENCODER_R encoder counter: %d.", counter2_data);				// ��������ɼ�������
		systick_delay_ms(1000);														// ʱ���� 1000ms

		soft_dir_inverted();
		printf("\r\nENCODER_L encoder counter: %d.", counter1_data);				// ��������ɼ�������
		printf("\r\nENCODER_R encoder counter: %d.", counter2_data);				// ��������ɼ�������
		systick_delay_ms(1000);														// ʱ���� 1000ms
#else
		encoder_count = tim_encoder_get_count(ENCODER_L);							// �ɼ���Ӧ����������
		tim_encoder_rst(ENCODER_L);													// �����Ӧ����
		printf("\r\nENCODER_L encoder counter: %d.", encoder_count);				// ��������ɼ�������

		encoder_count = tim_encoder_get_count(ENCODER_R);							// �ɼ���Ӧ����������
		tim_encoder_rst(ENCODER_R);													// �����Ӧ����
		printf("\r\nENCODER_R encoder counter: %d.", encoder_count);				// ��������ɼ�������

		systick_delay_ms(1000);														// ʱ���� 1000ms
#endif
	}
}

void pit_handler (void)																// �������� TIM8_UP_IRQHandler �е���
{
	if(gpio_get(COUNTER1_DIR))
		counter1_data = tim_counter_get_count(COUNTER1_TIM);
	else
		counter1_data = -tim_counter_get_count(COUNTER1_TIM);
	tim_counter_rst(COUNTER1_TIM);

	if(gpio_get(COUNTER2_DIR))
		counter2_data = tim_counter_get_count(COUNTER2_TIM);
	else
		counter2_data = -tim_counter_get_count(COUNTER2_TIM);
	tim_counter_rst(COUNTER2_TIM);
}

#if SOFT_SIM_ENCODER
void soft_dir_positive (void)
{
	gpio_set(SOFT_DIR_PIN, 0);
}

void soft_dir_inverted (void)
{
	gpio_set(SOFT_DIR_PIN, 1);
}
#endif
// **************************** �������� ****************************
