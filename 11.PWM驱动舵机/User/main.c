#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Key.h"
#include "Servo.h"
#include "Delay.h"

uint16_t keyNum = 0; // 按键的键值
float angle = 0.0;	 // 舵机的角度

int main(void)
{
	OLED_Init();		   // 初始化OLED模块
	Key_Init(GPIO_Pin_10); // 初始化Key模块
	Servo_Init();		   // 初始化PWM模块

	OLED_ShowString(1, 1, "Angle:"); // 显示字符串Angle:

	while (1)
	{
		keyNum = Key_GetNum(GPIOB, GPIO_Pin_10);
		if (keyNum == 1)
		{
			angle += 30;
			if (angle > 180)
			{
				angle = 0;
			}
			Servo_SetAngle(angle);
			OLED_ShowNum(1, 7, angle, 3);
		}
	}
}
