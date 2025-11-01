#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Serial.h"

uint8_t RxData; // 接收串口数据的变量

int main(void)
{
	OLED_Init();						  // 初始化 OLED模块
	Serial_Init(GPIO_Pin_10, GPIO_Pin_9); // 初始化 Serial模块
	
	OLED_ShowString(1, 1, "RxData:");

	while (1)
	{
		if (Serial_GetRxFlag() == 1) // 检查串口接收数据的标志位
		{
			RxData = Serial_GetRxData();	  // 获取串口接收的数据
			Serial_SendByte(RxData);		  // 串口将收到的数据回传回去,用于测试
			OLED_ShowHexNum(1, 8, RxData, 2); // 显示串口接收的数据
		}
	}
}
