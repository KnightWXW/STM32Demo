#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "LED.h"
#include "Serial.h"
#include "string.h"

// 进行验证测试时,发送区命令要进行换行
int main(void)
{
	OLED_Init();						  // 初始化 OLED模块
	LED_Init(GPIO_Pin_1);				  // 初始化 LED模块
	Serial_Init(GPIO_Pin_10, GPIO_Pin_9); // 初始化 Serial模块

	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");

	while (1)
	{
		// 如果接收到数据包
		if (Serial_RxFlag == 1)
		{
			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, Serial_RxPacket);

			// 将收到的数据包与预设的指令对比,以此决定将要执行的操作
			// (1) 如果收到LED_ON指令:
			// 点亮LED,串口回传一个字符串LED_ON_OK,OLED清除指定位置，并显示LED_ON_OK
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED_ON(GPIOA, GPIO_Pin_1);
				Serial_SendString("LED_ON_OK\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_ON_OK");
			}
			// (2) 如果收到LED_OFF指令:
			// 熄灭LED,串口回传一个字符串LED_OFF_OK,OLED清除指定位置，并显示LED_OFF_OK
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED_OFF(GPIOA, GPIO_Pin_1);
				Serial_SendString("LED_OFF_OK\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_OFF_OK");
			}
			// (3) 上述所有条件均不满足,即收到了未知指令:
			// 串口回传一个字符串ERROR_COMMAND,OLED清除指定位置，并显示ERROR_COMMAND
			else
			{
				Serial_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR_COMMAND"); //
			}
			// 处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
			Serial_RxFlag = 0;
		}
	}
}
