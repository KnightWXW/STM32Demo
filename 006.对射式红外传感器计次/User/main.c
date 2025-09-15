#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Sensor.h"
#include "Key.h"
#include "Delay.h"

uint16_t sensorCnt = 0; // 对射红外传感器计数

int main(void)
{
	OLED_Init();					   // 初始化OLED模块
	Key_Init(GPIO_Pin_1);			   // 初始化按键模块
	Sensor_Init(GPIO_Pin_8);		   // 初始化对射红外传感器模块
	Sensor_EXTI_Init(); 			   // 初始化对射红外传感器外部中断模块

	OLED_ShowString(1, 1, "Count:"); // 显示字符串Count:
	while (1)
	{
		uint16_t keyNum = Key_GetNum(GPIOB, GPIO_Pin_1);
		if (keyNum == 1)
		{
			sensorCnt = 0;
		}
		OLED_ShowNum(1, 7, Sensor_GetNum(), 5);
	}
}
