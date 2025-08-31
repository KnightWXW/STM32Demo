/*
 * @Author: random
 * @Date: 2025-08-28 20:52:38
 * @Last Modified by: random
 * @Last Modified time: Do not Edit
 */
#include "stm32f10x.h" // Device header
#include "LED.h"
#include "Delay.h"

int main(void)
{
	LED_Init(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
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
