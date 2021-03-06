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
#include "Timer_us.h"

// *************************** 例程说明 ***************************
// 
// 测试需要准备逐飞科技 MM32F3277 核心板一块
// 
// 调试下载需要准备逐飞科技 CMSIS-DAP 调试下载器 或 ARM 调试下载器 一个
// 
// 本例程初始化了逐飞科技 MM32F3277 核心板上的 B13/H2 两个LED对应的引脚
// 并初始化了一个引脚作为按键输入 该引脚可修改
// 
// 烧录本例程后 可见 B13 对应LED闪烁 按键输入引脚输入高电平时 H2 亮起
// 
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
// 
// *************************** 例程说明 ***************************

// **************************** 宏定义 ****************************
#define LED1				B13
#define LED2				H2
#define KEY					D0
// **************************** 宏定义 ****************************

// **************************** 变量定义 ****************************


uint32 camera_frame = 0;
float a = 0;

// **************************** 变量定义 ****************************

// **************************** 代码区域 ****************************
int main(void)
{
	board_init(true);																// 初始化 debug 输出串口
	seekfree_wireless_init();
	Timer_us_init();
	ips114_init();
	mt9v03x_init();
	systick_delay_ms(1000);

	//此处编写用户代码(例如：外设初始化代码等)
	gpio_init(KEY, GPI, GPIO_HIGH, GPI_PULL_UP);									// 初始化引脚为上拉输入 默认高电平
	gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);									// 初始化引脚为推挽输出 默认高电平
	gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);									// 初始化引脚为推挽输出 默认高电平
	// tim_interrupt_init_ms(TIM_2, 1000, 0, 1);
	// pwm_init(TIM_2, TIM_2_CH1_A00, 440, PWM_DUTY_MAX / 2);
	//此处编写用户代码(例如：外设初始化代码等)

	// seekfree_wireless_send_buff((uint8 *)"\n");		// 发送测试信息

	// ips114_display_chinese(0,0,16,chinese_test[0],4,RED);
	// ips114_displayimage032(gImage_qq, 40, 40);
	// ips114_displayimage8660_zoom(gImage_qq, 40, 40, 80, 80);

	while(1)
	{
		// a += 0.1;
		// sprintf(time_str, "%lld\n", Timer_us_Get());
		// seekfree_wireless_send_buff(time_str);		// 发送测试信息
		// systick_delay_ms(10);

	ips114_displayimage032(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
		printf("a\n");
		// if(mt9v03x_finish_flag)
		// {	
		// 	camera_frame++;
		// 	// ips114_displayimage032(mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
		// 	mt9v03x_finish_flag = 0;
		// 	// if (camera_frame % 250 == 0)
		// 	// {
		// 	// sprintf(time_str, "%lld\n", Timer_us_Get() / 1000);
		// 	// seekfree_wireless_send_buff(time_str);
		// 	// }
		// }
		// sprintf(time_str, "%lu\n", camera_frame);
		// seekfree_wireless_send_buff(time_str);
		
	}
}
// **************************** 代码区域 ****************************
