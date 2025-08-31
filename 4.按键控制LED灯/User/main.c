#include "stm32f10x.h" // Device header
#include "LED.h"
#include "Key.h"
#include "Delay.h"

uint16_t num; // 按键键码值

int main(void)
{
	LED_Init(GPIO_Pin_1);
	Key_Init(GPIO_Pin_1);
	while (1)
	{
		num = Key_GetNum(GPIOB, GPIO_Pin_1);
		if (num == 1)
		{
			LED_Turn(GPIOA, GPIO_Pin_1);
		}
	}
}
