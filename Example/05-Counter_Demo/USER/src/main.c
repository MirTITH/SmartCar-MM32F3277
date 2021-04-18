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
// 本例程初始化了逐飞科技 MM32F3277 核心板上的 TIM_3/4 两个定时器作为编码器采集输入
// 将编码器接在对应的 TIM_3-B04/B05  TIM_4-B06/B07
// 开启 SOFT_SIM_ENCODER 定义时 初始化了 A0/1 两个引脚作为模拟编码器波形输出
// 将 A0/1 接在对应的 TIM_3-B04/B05 或者 TIM_4-B06/B07
// 
// 烧录本例程后 接线完毕上电后可见从串口输出的编码器采集信息
// 
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
// 
// *************************** 例程说明 ***************************

// **************************** 宏定义 ****************************
#define COUNTER1_TIM		TIM_3
#define COUNTER1_PLUS		TIM_3_ENC1_B04
#define COUNTER1_DIR		B5

#define COUNTER2_TIM		TIM_4
#define COUNTER2_PLUS		TIM_4_ENC1_B06
#define COUNTER2_DIR		B7

#define SOFT_SIM_ENCODER			1												// 1-开启模拟测试 使用GPIO模拟编码器波形 0-关闭模拟测试 使用编码器实物测试
#if SOFT_SIM_ENCODER
#define SOFT_PLUS_TIM		TIM_5
#define SOFT_PLUS_PIN		TIM_5_CH1_A00
#define SOFT_DIR_PIN		A1
#endif
// **************************** 宏定义 ****************************

// **************************** 变量定义 ****************************
int16 counter1_data = 0;
int16 counter2_data = 0;
// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
#if SOFT_SIM_ENCODER
void soft_dir_positive (void);
void soft_dir_inverted (void);
#endif

int main(void)
{
	board_init(true);																// 初始化 debug 输出串口

	tim_counter_init(COUNTER1_TIM, COUNTER1_PLUS);									// 初始化外部输入计数器采集
	tim_counter_init(COUNTER2_TIM, COUNTER2_PLUS);									// 初始化外部输入计数器采集

	gpio_init(COUNTER1_DIR, GPI, 1, GPI_PULL_UP);
	gpio_init(COUNTER2_DIR, GPI, 1, GPI_PULL_UP);

	tim_interrupt_init_ms(TIM_8, 10, 0x00, 0x00);

#if SOFT_SIM_ENCODER
	pwm_init(SOFT_PLUS_TIM, SOFT_PLUS_PIN, 5000, PWM_DUTY_MAX/100*50);				// 初始化 GPIO 输入模拟编码器波形
	gpio_init(SOFT_DIR_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);							// 初始化 GPIO 输入模拟编码器波形
#endif

	while(1)
	{
#if SOFT_SIM_ENCODER
		soft_dir_positive();
		printf("\r\nENCODER_L encoder counter: %d.", counter1_data);				// 串口输出采集到数据
		printf("\r\nENCODER_R encoder counter: %d.", counter2_data);				// 串口输出采集到数据
		systick_delay_ms(1000);														// 时间间隔 1000ms

		soft_dir_inverted();
		printf("\r\nENCODER_L encoder counter: %d.", counter1_data);				// 串口输出采集到数据
		printf("\r\nENCODER_R encoder counter: %d.", counter2_data);				// 串口输出采集到数据
		systick_delay_ms(1000);														// 时间间隔 1000ms
#else
		encoder_count = tim_encoder_get_count(ENCODER_L);							// 采集对应编码器数据
		tim_encoder_rst(ENCODER_L);													// 清除对应计数
		printf("\r\nENCODER_L encoder counter: %d.", encoder_count);				// 串口输出采集到数据

		encoder_count = tim_encoder_get_count(ENCODER_R);							// 采集对应编码器数据
		tim_encoder_rst(ENCODER_R);													// 清除对应计数
		printf("\r\nENCODER_R encoder counter: %d.", encoder_count);				// 串口输出采集到数据

		systick_delay_ms(1000);														// 时间间隔 1000ms
#endif
	}
}

void pit_handler (void)																// 本函数在 TIM8_UP_IRQHandler 中调用
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
// **************************** 代码区域 ****************************
