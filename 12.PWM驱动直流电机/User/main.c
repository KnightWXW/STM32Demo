#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Key.h"
#include "Motor.h"
#include "Delay.h"

uint16_t keyNum = 0; // 按键的键值
int16_t speed = 0;	 // 直流电机的速度

int main(void)
{
	OLED_Init();									 // 初始化OLED模块
	Key_Init(GPIO_Pin_10);							 // 初始化Key模块
	Motor_Init(GPIO_Pin_2, GPIO_Pin_4 | GPIO_Pin_5); // 初始化PWM模块

	OLED_ShowString(1, 1, "Speed:"); // 显示字符串Angle:

	while (1)
	{
		keyNum = Key_GetNum(GPIOB, GPIO_Pin_10);
		if (keyNum == 1)
		{
			speed += 20;
			if (speed > 100)
			{
				speed = -100;
			}
			Motor_SetSpeed(speed);
			OLED_ShowSignedNum(1, 7, speed, 3);
		}
	}
}
