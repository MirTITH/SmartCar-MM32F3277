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
#include "Timer_us.h"

// *************************** ����˵�� ***************************
// 
// ������Ҫ׼����ɿƼ� MM32F3277 ���İ�һ��
// 
// ����������Ҫ׼����ɿƼ� CMSIS-DAP ���������� �� ARM ���������� һ��
// 
// �����̳�ʼ������ɿƼ� MM32F3277 ���İ��ϵ� B13/H2 ����LED��Ӧ������
// ����ʼ����һ��������Ϊ�������� �����ſ��޸�
// 
// ��¼�����̺� �ɼ� B13 ��ӦLED��˸ ����������������ߵ�ƽʱ H2 ����
// 
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// 
// *************************** ����˵�� ***************************

// **************************** �궨�� ****************************
#define LED1				B13
#define LED2				H2
#define KEY					D0
// **************************** �궨�� ****************************

// **************************** �������� ****************************

char time_str[30];
uint32 camera_frame = 0;

// **************************** �������� ****************************

// **************************** �������� ****************************
int main(void)
{
	board_init(true);																// ��ʼ�� debug �������
	seekfree_wireless_init();
	Timer_us_init();
	ips114_init();
	mt9v03x_init();
	systick_delay_ms(1000);

	//�˴���д�û�����(���磺�����ʼ�������)
	gpio_init(KEY, GPI, GPIO_HIGH, GPI_PULL_UP);									// ��ʼ������Ϊ�������� Ĭ�ϸߵ�ƽ
	gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);									// ��ʼ������Ϊ������� Ĭ�ϸߵ�ƽ
	gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);									// ��ʼ������Ϊ������� Ĭ�ϸߵ�ƽ
	// gpio_init(A5, GPO, GPIO_LOW, GPO_PUSH_PULL);
	pwm_init(TIM_2, TIM_2_CH1_A00, 440, PWM_DUTY_MAX / 2);
	//�˴���д�û�����(���磺�����ʼ�������)

	seekfree_wireless_send_buff((uint8 *)"\n");		// ���Ͳ�����Ϣ

	// ips114_display_chinese(0,0,16,chinese_test[0],4,RED);
	// ips114_displayimage032(gImage_qq, 40, 40);
	// ips114_displayimage8660_zoom(gImage_qq, 40, 40, 80, 80);

	while(1)
	{
		// sprintf(time_str, "%lld\n", Timer_us_Get());
		// seekfree_wireless_send_buff(time_str);		// ���Ͳ�����Ϣ
		// systick_delay_ms(10);

	// ips114_displayimage032(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
		if(mt9v03x_finish_flag)
		{	
			camera_frame++;
			// ips114_displayimage032(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
			mt9v03x_finish_flag = 0;
			// if (camera_frame % 250 == 0)
			// {
			// sprintf(time_str, "%lld\n", Timer_us_Get() / 1000);
			// seekfree_wireless_send_buff(time_str);
			// }
		}
		// sprintf(time_str, "%lu\n", camera_frame);
		// seekfree_wireless_send_buff(time_str);
		
	}
}
// **************************** �������� ****************************
