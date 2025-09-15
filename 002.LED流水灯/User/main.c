#include "stm32f10x.h" // Device header
#include "LED.h"
#include "Delay.h"

int main(void)
{
	LED_Init(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2); // 初始化LED模块
	while (1)
	{
		LED_ON(GPIOA, GPIO_Pin_0);
		Delay_ms(500);
		LED_OFF(GPIOA, GPIO_Pin_0);
		Delay_ms(500);
		LED_ON(GPIOA, GPIO_Pin_1);
		Delay_ms(500);
		LED_OFF(GPIOA, GPIO_Pin_1);
		Delay_ms(500);
		LED_ON(GPIOA, GPIO_Pin_2);
		Delay_ms(500);
		LED_OFF(GPIOA, GPIO_Pin_2);
		Delay_ms(500);
	}
}
