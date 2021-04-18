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
// �����̳�ʼ������ɿƼ� MM32F3277 ���İ��ϵ� UART1-A9/10 ��Ϊ debug ���
// 
// ��¼�����̺� �ɼ� UART1 ����ĵ�����Ϣ ÿ�ν� flash ���ݶ�����ӡ Ȼ���1��д���ȥ
// 
// ���µĹ��̻��߹����ƶ���λ�����ִ�����²���
// ��һ�� �ر��������д򿪵��ļ�
// �ڶ��� project->clean  �ȴ��·�����������
// 
// *************************** ����˵�� ***************************

// **************************** �궨�� ****************************
#define SECTION_INDEX		FLASH_SECTION_127										// ���һ������
#define PAGE_INDEX			FLASH_PAGE_3											// ���һ��ҳ
#define DATA_SIZE			1024/sizeof(uint32)										// 32bit���ݴ洢 �������1024/4��С
// **************************** �궨�� ****************************

// **************************** �������� ****************************
uint16 temp_data;
uint32 data_buffer[DATA_SIZE];
uint8 *data_point = (uint8 *)data_buffer;
// **************************** �������� ****************************

// **************************** �������� ****************************
int main(void)
{
	board_init(true);																// ��ʼ�� debug �������

	if(flash_check(SECTION_INDEX, PAGE_INDEX))										// ���Flash�Ƿ�������
	{
		flash_page_read(SECTION_INDEX, PAGE_INDEX, data_buffer, DATA_SIZE);			// ���������е�����
		printf("\r\nFlash section%d page%d data:", SECTION_INDEX, PAGE_INDEX);		// ׼����ӡ����
		for(temp_data = 0; temp_data<1024; temp_data++)								// 8bit��ӡ
		{
			if(temp_data % 8 == 0)													// ÿ8�����ݻ���
				printf("\r\n");
			printf("0x%x\t", *(data_point+temp_data));								// hex��ʽ���
			*(data_point+temp_data) = *(data_point+temp_data)+1;					// ��������������
		}
		flash_erase_page(SECTION_INDEX, PAGE_INDEX);								// ��������
		flash_page_program(SECTION_INDEX, PAGE_INDEX, data_buffer, DATA_SIZE);		// �ѻ�����������д��ȥ
	}
	else
	{
		memset(data_buffer, 0x55, sizeof(data_buffer));								// ���������������
		flash_erase_page(SECTION_INDEX, PAGE_INDEX);								// ����ҳ
		flash_page_program(SECTION_INDEX, PAGE_INDEX, data_buffer, DATA_SIZE);		// д�뻺������
		flash_page_read(SECTION_INDEX, PAGE_INDEX, data_buffer, DATA_SIZE);			// ������
		printf("\r\nFlash section%d page%d data:", SECTION_INDEX, PAGE_INDEX);		// ׼����ӡ����
		for(temp_data = 0; temp_data<1024; temp_data++)								// 8bit��ӡ
		{
			if(temp_data % 8 == 0)													// ÿ8�����ݻ���
				printf("\r\n");
			printf("0x%o\t", *(data_point+temp_data));								// hex��ʽ���
		}
	}

	while(1)
	{
	}
}
// **************************** �������� ****************************
