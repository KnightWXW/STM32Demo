#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "ADC.h"
#include "Delay.h"

uint16_t ad1, ad2, ad3, ad4; // 记录4个AD值变量

int main(void)
{
	OLED_Init();												// 初始化 OLED模块
	AD_Init(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); // 初始化 ADC模块

	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");

	while (1)
	{
		ad1 = ADC_GetValue(ADC_Channel_0); // 单次启动ADC，转换通道0
		ad2 = ADC_GetValue(ADC_Channel_1); // 单次启动ADC，转换通道1
		ad3 = ADC_GetValue(ADC_Channel_2); // 单次启动ADC，转换通道2
		ad4 = ADC_GetValue(ADC_Channel_3); // 单次启动ADC，转换通道3
		OLED_ShowNum(1, 5, ad1, 4);		   // 显示通道0的转换结果ad1
		OLED_ShowNum(2, 5, ad2, 4);		   // 显示通道0的转换结果ad2
		OLED_ShowNum(3, 5, ad3, 4);		   // 显示通道0的转换结果ad3
		OLED_ShowNum(4, 5, ad4, 4);		   // 显示通道0的转换结果ad4
		Delay_ms(100);					   // 延时100ms
	}
}
