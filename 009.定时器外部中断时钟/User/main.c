#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Timer.h"
#include "Sensor.h"

uint16_t timerCnt = 0; // 定时器计数

int main(void)
{
	OLED_Init();			 // 初始化OLED模块
	Sensor_Init(GPIO_Pin_0); // 初始化光敏传感器模块
	Timer_Init();			 // 初始化定时器中断模块

	OLED_ShowString(1, 1, "Number:"); // 显示 触发外部中断次数
	OLED_ShowString(2, 1, "Count:");  // 显示 定时器的当前计数器数值

	while (1)
	{
		OLED_ShowNum(1, 8, timerCnt, 5);
		OLED_ShowNum(2, 7, Timer_GetCounter(), 5);
	}
}
