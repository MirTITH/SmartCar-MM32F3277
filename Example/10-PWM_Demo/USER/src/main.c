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
// �����̳�ʼ������ɿƼ� MM32F3277 ���İ��ϵ� A0/1/2/3 ��Ϊ TIM2 �� PWM ���
// �벻Ҫʹ�ñ��������������������������
// 
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// 
// *************************** ����˵�� ***************************

// **************************** �궨�� ****************************
#define PWM_TIM				TIM_2
#define PWM_CH1				TIM_2_CH1_A00
#define PWM_CH2				TIM_2_CH2_A01
#define PWM_CH3				TIM_2_CH3_A02
#define PWM_CH4				TIM_2_CH4_A03
// **************************** �궨�� ****************************

// **************************** �������� ****************************
uint16 duty;
bool output_dir = false;
// **************************** �������� ****************************

// **************************** �������� ****************************
int main(void)
{
	board_init(true);																// ��ʼ�� debug �������

	pwm_init(PWM_TIM, PWM_CH1, 10000, 0);											// ��ʼ��TIM2 Ƶ��10KHz ��ʼռ�ձ�Ϊ 0/PWM_DUTY_MAX*100%
	pwm_init(PWM_TIM, PWM_CH2, 10000, 0);											// ��ʼ��TIM2 Ƶ��10KHz ��ʼռ�ձ�Ϊ 0/PWM_DUTY_MAX*100%
	pwm_init(PWM_TIM, PWM_CH3, 10000, 0);											// ��ʼ��TIM2 Ƶ��10KHz ��ʼռ�ձ�Ϊ 0/PWM_DUTY_MAX*100%
	pwm_init(PWM_TIM, PWM_CH4, 10000, 0);											// ��ʼ��TIM2 Ƶ��10KHz ��ʼռ�ձ�Ϊ 0/PWM_DUTY_MAX*100%

	while(1)
	{
		if(output_dir)																// ��ֹ���ϵ����PWM��������˫��ͨ
		{
			pwm_duty_updata(PWM_TIM, PWM_CH1, duty);								// ͨ�� 1 ���
			pwm_duty_updata(PWM_TIM, PWM_CH2, 0);									// ͨ�� 2 ֹͣ
			pwm_duty_updata(PWM_TIM, PWM_CH3, duty);								// ͨ�� 3 ���
			pwm_duty_updata(PWM_TIM, PWM_CH4, 0);									// ͨ�� 4 ֹͣ

			duty += 100;															// ����ռ�ձ�
			if(duty > PWM_DUTY_MAX/2)												// ռ�ձȲ����� 50%
			{
				duty = 0;															// ռ�ձ����
				output_dir = false;													// ��ת���
			}
			systick_delay_ms(20);
		}
		else
		{
			pwm_duty_updata(PWM_TIM, PWM_CH1, 0);									// ͨ�� 1 ֹͣ
			pwm_duty_updata(PWM_TIM, PWM_CH2, duty);								// ͨ�� 2 ���
			pwm_duty_updata(PWM_TIM, PWM_CH3, 0);									// ͨ�� 3 ֹͣ
			pwm_duty_updata(PWM_TIM, PWM_CH4, duty);								// ͨ�� 4 ���

			duty += 100;															// ����ռ�ձ�
			if(duty > PWM_DUTY_MAX/2)												// ռ�ձȲ����� 50%
			{
				duty = 0;															// ռ�ձ����
				output_dir = true;													// ��ת���
			}
			systick_delay_ms(20);
		}
	}
}
// **************************** �������� ****************************
