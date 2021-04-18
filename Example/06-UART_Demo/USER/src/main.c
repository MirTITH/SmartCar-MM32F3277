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
// �����̳�ʼ������ɿƼ� MM32F3277 ���İ��ϵ� UART1-A9/10 UART2-A2/3
// 
// ��¼�����̺� �ɼ� UART1 ����ĵ���ָ����Ϣ UART2 ʹ���жϽ��շ�ʽ
// 
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// 
// *************************** ����˵�� ***************************

// **************************** �궨�� ****************************
// **************************** �궨�� ****************************

// **************************** �������� ****************************
uint8 uart1_get_buffer;
uint8 uart2_get_buffer;
bool uart2_interrupt_flag = false;													// ��������� isr.c �� UART2_IRQHandler ����λ
// **************************** �������� ****************************

// **************************** �������� ****************************
int main(void)
{
	board_init(true);																// ��ʼ�� debug �������

//	uart_init(UART_1, 115200, UART1_TX_A09, UART1_RX_A10);							// UART1 �Ѿ��� board_init(true); ��ʼ������
	uart_init(UART_2, 115200, UART2_TX_A02, UART2_RX_A03);							// Ĭ�ϳ�ʼ�� UART1 ����֧�� printf ���
	uart_rx_irq(UART_2, ENABLE);													// ʹ�� UART2 �Ľ����ж�

	printf("\r\nSEEKFREE UART Demo printf test.");									// �����Ϣ
	printf("\r\nPlease input 'Y' to start.");										// �����ʾ��Ϣ
	while(uart1_get_buffer != 'Y')													// �ȴ��û����� Y
		uart_getchar(UART_1, &uart1_get_buffer);									// ��ȡ�û�����
	printf("\r\nUART Demo printf test start.");										// ��ʾ��ͨ��

	while(1)
	{
		if(uart_query(UART_1, &uart1_get_buffer))									// ��ѯ�Ƿ��յ�����
			uart_putchar(UART_1, uart1_get_buffer);									// �����յ�������

		if(uart2_interrupt_flag)													// UART2 �����˽����ж�
		{
			uart2_interrupt_flag = false;											// UART2 �жϱ�־��λ
			uart_putchar(UART_2, uart2_get_buffer);									// �����յ�������
		}
	}
}

void uart_interrupt_handler (void)													// ��������� isr.c �� UART2_IRQHandler �е���
{
	uart2_interrupt_flag = true;													// UART2 �жϱ�־��λ
	uart_getchar(UART_2, &uart2_get_buffer);										// ��ȡ����
}
// **************************** �������� ****************************
