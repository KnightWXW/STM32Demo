#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Encoder.h"
#include "Timer.h"
#include "Delay.h"

int16_t speed; // 记录旋转编码器的速度

int main(void)
{
	OLED_Init();						   // 初始化 OLED模块
	Timer_Init();						   // 初始化 TIM模块
	Encoder_Init(GPIO_Pin_6 | GPIO_Pin_7); // 初始化 旋转编码器模块

	OLED_ShowString(1, 1, "Speed:"); // 显示 旋转编码器 Speed

	while (1)
	{
		OLED_ShowSignedNum(1, 7, speed, 5); // 不断刷新显示编码器测得的最新速度
	}
}
