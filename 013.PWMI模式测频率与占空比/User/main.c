#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();		  // 初始化OLED模块
	PWM_Init(GPIO_Pin_0); // 初始化PWM模块
	IC_Init(GPIO_Pin_6);  // 初始化IC模块

	OLED_ShowString(1, 1, "Freq:00000Hz"); // 1行1列 显示频率    Freq:00000Hz
	OLED_ShowString(2, 1, "Duty:00%");	   // 1行1列 显示占空比  Duty:00%

	// 使用PWM模块提供输入捕获的测试信号
	PWM_SetPrescaler(720 - 1); // PWM频率    Freq = 72M / (PSC + 1) / 100
	PWM_SetCompare1(50);	   // PWM占空比  Duty = CCR / 100

	while (1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5); // 刷新显示输入捕获测得的频率
		OLED_ShowNum(2, 6, IC_GetDuty(), 2); // 刷新显示输入捕获测得的占空比
	}
}
