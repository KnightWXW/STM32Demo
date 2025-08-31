#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "Buzzer.h"

int main(void)
{
	Buzzer_Init(GPIO_Pin_0);
	while (1)
	{
		Buzzer_ON(GPIOB, GPIO_Pin_0);
		Delay_ms(100);
		Buzzer_OFF(GPIOB, GPIO_Pin_0);
		Delay_ms(100);
		Buzzer_ON(GPIOB, GPIO_Pin_0);
		Delay_ms(100);
		Buzzer_OFF(GPIOB, GPIO_Pin_0);
		Delay_ms(700);
	}
}
