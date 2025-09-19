#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "ADC.h"
#include "Delay.h"

uint16_t ADValue; // 记录ADC值变量
float Voltage;	  // 记录电压变量

int main(void)
{
	OLED_Init();		 // 初始化 OLED模块
	AD_Init(GPIO_Pin_0); // 初始化 ADC模块

	OLED_ShowString(1, 1, "ADCValue:");		 // 显示 ADC数值
	OLED_ShowString(2, 1, "Voltage: 0.00V"); // 显示 电压数值

	while (1)
	{
		ADValue = ADC_GetValue();								 // 获取ADC转换的值
		Voltage = (float)ADValue / 4095 * 3.3;					 // 将AD值线性变换到 0~3.3V 的范围,表示电压
		OLED_ShowNum(1, 10, ADValue, 4);						 // 显示AD值
		OLED_ShowNum(2, 10, Voltage, 1);						 // 显示电压值的整数部分
		OLED_ShowNum(2, 12, (uint16_t)(Voltage * 100) % 100, 2); // 显示电压值的小数部分
		Delay_ms(100);											 // 延时100ms
	}
}
