#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Key.h"
#include "PWM.h"
#include "IC.h"
#include "Delay.h"

uint16_t keyNum = 0; // 按键的键值
int16_t speed = 0;	 // 直流电机的速度

int main(void)
{
	OLED_Init();		  // 初始化OLED模块
	PWM_Init(GPIO_Pin_0); // 初始化PWM模块
	IC_Init(GPIO_Pin_6);  // 初始化IC模块

	OLED_ShowString(1, 1, "Freq:00000Hz"); // 1行1列显示字符串Freq:00000Hz

	// 使用PWM模块提供输入捕获的测试信号
	PWM_SetPrescaler(720 - 1); // PWM频率Freq = 72M / (PSC + 1) / 100
	PWM_SetCompare1(50);	   // PWM占空比Duty = CCR / 100

	while (1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5); // 不断刷新显示输入捕获测得的频率
	}
}
