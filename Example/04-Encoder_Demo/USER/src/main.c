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
#define ENCODER_L					TIM_3
#define ENCODER_L_A					TIM_3_ENC1_B04
#define ENCODER_L_B					TIM_3_ENC2_B05
#define ENCODER_R					TIM_4
#define ENCODER_R_A					TIM_4_ENC1_B06
#define ENCODER_R_B					TIM_4_ENC2_B07

#define SOFT_SIM_ENCODER			1												// 1-����ģ����� ʹ��GPIOģ����������� 0-�ر�ģ����� ʹ�ñ�����ʵ�����
#if SOFT_SIM_ENCODER
#define SOFT_SIM_ENCODER_A_PIN		A0
#define SOFT_SIM_ENCODER_B_PIN		A1
#endif
// **************************** �궨�� ****************************

// **************************** �������� ****************************
int16 encoder_count = 0;
// **************************** �������� ****************************

// **************************** �������� ****************************
#if SOFT_SIM_ENCODER
void soft_sim_encoder_positive (uint8 loop_count);
void soft_sim_encoder_inverted (uint8 loop_count);
#endif

int main(void)
{
	board_init(true);																// ��ʼ�� debug �������

	tim_encoder_init(ENCODER_L, ENCODER_L_A, ENCODER_L_B);							// ��ʼ�������������ɼ�
	tim_encoder_init(ENCODER_R, ENCODER_R_A, ENCODER_R_B);							// ��ʼ�������������ɼ�
#if SOFT_SIM_ENCODER
	gpio_init(SOFT_SIM_ENCODER_A_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);				// ��ʼ�� GPIO ����ģ�����������
	gpio_init(SOFT_SIM_ENCODER_B_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);				// ��ʼ�� GPIO ����ģ�����������
#endif

	while(1)
	{
#if SOFT_SIM_ENCODER
		soft_sim_encoder_positive(20);												// ģ��������������ת����20�� ��80������

		encoder_count = tim_encoder_get_count(ENCODER_L);							// �ɼ���Ӧ����������
		tim_encoder_rst(ENCODER_L);													// �����Ӧ����
		printf("\r\nENCODER_L encoder counter: %d.", encoder_count);				// ��������ɼ�������

		encoder_count = tim_encoder_get_count(ENCODER_R);							// �ɼ���Ӧ����������
		tim_encoder_rst(ENCODER_R);													// �����Ӧ����
		printf("\r\nENCODER_R encoder counter: %d.", encoder_count);				// ��������ɼ�������

		systick_delay_ms(1000);														// ʱ���� 1000ms

		soft_sim_encoder_inverted(20);												// ģ��������������ת����20�� ��80������

		encoder_count = tim_encoder_get_count(ENCODER_L);							// �ɼ���Ӧ����������
		tim_encoder_rst(ENCODER_L);													// �����Ӧ����
		printf("\r\nENCODER_L encoder counter: %d.", encoder_count);				// ��������ɼ�������

		encoder_count = tim_encoder_get_count(ENCODER_R);							// �ɼ���Ӧ����������
		tim_encoder_rst(ENCODER_R);													// �����Ӧ����
		printf("\r\nENCODER_R encoder counter: %d.", encoder_count);				// ��������ɼ�������

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

#if SOFT_SIM_ENCODER
void soft_sim_encoder_positive (uint8 loop_count)
{
	while(loop_count--)
	{
		gpio_set(SOFT_SIM_ENCODER_A_PIN, 1);
		systick_delay_ms(10);
		gpio_set(SOFT_SIM_ENCODER_B_PIN, 1);
		systick_delay_ms(10);
		gpio_set(SOFT_SIM_ENCODER_A_PIN, 0);
		systick_delay_ms(10);
		gpio_set(SOFT_SIM_ENCODER_B_PIN, 0);
		systick_delay_ms(10);
	}
}

void soft_sim_encoder_inverted (uint8 loop_count)
{
	while(loop_count--)
	{
		gpio_set(SOFT_SIM_ENCODER_B_PIN, 1);
		systick_delay_ms(10);
		gpio_set(SOFT_SIM_ENCODER_A_PIN, 1);
		systick_delay_ms(10);
		gpio_set(SOFT_SIM_ENCODER_B_PIN, 0);
		systick_delay_ms(10);
		gpio_set(SOFT_SIM_ENCODER_A_PIN, 0);
		systick_delay_ms(10);
	}
}
#endif
// **************************** �������� ****************************
