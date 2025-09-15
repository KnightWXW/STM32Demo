#include "stm32f10x.h" // Device header
#include "LED.h"
#include "PWM.h"
#include "Delay.h"

int main(void)
{
	LED_Init(GPIO_Pin_0); // 初始化LED模块
	PWM_Init();			  // 初始化PWM模块

	while (1)
	{
		for (int i = 0; i <= 100; i++)
		{
			// 依次将定时器的CCR寄存器设置为0~100，PWM占空比逐渐增大，LED逐渐变亮
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for (int i = 100; i >= 0; i--)
		{
			// 依次将定时器的CCR寄存器设置为100~0，PWM占空比逐渐增小，LED逐渐变暗
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
	}
}
