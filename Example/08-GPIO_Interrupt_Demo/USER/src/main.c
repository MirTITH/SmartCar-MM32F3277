/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				main
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#include "headfile.h"

// *************************** 例程说明 ***************************
// 
// 测试需要准备逐飞科技 MM32F3277 核心板一块
// 
// 调试下载需要准备逐飞科技 CMSIS-DAP调试下载器 或 ARM调试下载器 一个
// 
// 本例程初始化了逐飞科技 MM32F3277 核心板上的 D0 作为外部中断输入
// 
// 烧录本例程后 可见 UART1 输出的调试指导信息
// 
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
// 
// *************************** 例程说明 ***************************

// **************************** 宏定义 ****************************
#define INTERRUPT_INPUT_KEY			D0
// **************************** 宏定义 ****************************

// **************************** 变量定义 ****************************
bool exti_interrupt_flag = false;													// 这个变量在 isr.c 的 EXTIx_IRQHandler 中修改
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
int main(void)
{
	board_init(true);																// 初始化 debug 输出串口

	exti_interrupt_init(INTERRUPT_INPUT_KEY, EXTI_Trigger_Rising, 0, 1);			// 初始化外部中断输入 上升沿触发
	printf("\r\nExti interrupt test running.");										// 输出信息

	while(1)
	{
		if(exti_interrupt_flag)														// 发生了中断
		{
			printf("\r\nExti interrupt.");											// 输出信息
			exti_interrupt_flag = false;											// 清空标志
		}
	}
}

void exti_interrupt_handler (void)													// 这个函数在 isr.c 的 EXTIx_IRQHandler 中调用
{
	exti_interrupt_flag = true;														// exti 中断标志置位
}
// **************************** 代码区域 ****************************
