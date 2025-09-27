#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "DMA_ADC.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();													 // 初始化 OLED模块
	DMA_ADC_Init(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); // 初始化 DMA模块

	OLED_ShowString(1, 1, "ADC0:");
	OLED_ShowString(2, 1, "ADC1:");
	OLED_ShowString(3, 1, "ADC2:");
	OLED_ShowString(4, 1, "ADC3:");

	while (1)
	{
		OLED_ShowNum(1, 6, ADC_Value[0], 4); // 显示转换结果第0个数据
		OLED_ShowNum(2, 6, ADC_Value[1], 4); // 显示转换结果第1个数据
		OLED_ShowNum(3, 6, ADC_Value[2], 4); // 显示转换结果第2个数据
		OLED_ShowNum(4, 6, ADC_Value[3], 4); // 显示转换结果第3个数据

		Delay_ms(100); // 延时100ms
	}
}
