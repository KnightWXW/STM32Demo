#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Delay.h"

int main(void)
{
	LED_Init();
	while(1)
	{
		LED_ON(GPIOA, GPIO_Pin_1);
		Delay_ms(1000);
		LED_OFF(GPIOA, GPIO_Pin_1);
		Delay_ms(1000);
	}
}
