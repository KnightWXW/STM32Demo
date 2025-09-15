#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Encoder.h"
#include "Key.h"
#include "Delay.h"

int16_t encoderCnt = 0; // 旋转计数器计数

int main(void)
{
	OLED_Init();						   // 初始化OLED模块
	Key_Init(GPIO_Pin_10);				   // 初始化按键模块
	Encoder_Init(GPIO_Pin_0 | GPIO_Pin_1); // 初始化旋转计数器模块
	Encoder_EXTI_Init();				   // 初始化旋转计数器外部中断模块

	OLED_ShowString(1, 1, "Count:"); // 显示字符串Count:
	while (1)
	{
		uint16_t keyNum = Key_GetNum(GPIOB, GPIO_Pin_10);
		if (keyNum == 1)
		{
			encoderCnt = 0;
		}
		encoderCnt += Encoder_GetNum();
		OLED_ShowSignedNum(1, 7, encoderCnt, 5);
	}
}
