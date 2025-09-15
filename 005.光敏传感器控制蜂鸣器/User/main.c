#include "stm32f10x.h" // Device header
#include "Buzzer.h"
#include "Sensor.h"
#include "Delay.h"

int main(void)
{
	Buzzer_Init(GPIO_Pin_0);  // 初始化蜂鸣器模块
	Sensor_Init(GPIO_Pin_10); // 初始化红外对射传感器模块
	while (1)
	{
		uint8_t num = Sensor_GetNum(GPIOA, GPIO_Pin_10); // 获取光敏传感器当前状态
		if (num == 1)
		{
			Buzzer_ON(GPIOB, GPIO_Pin_0); // 高电平时,光源被遮挡,蜂鸣器鸣叫
		}
		else
		{
			Buzzer_OFF(GPIOB, GPIO_Pin_0); // 低电平时,光源未遮挡,蜂鸣器不鸣叫
		}
	}
}
