#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init(); // 初始化 OLED模块

	Serial_Init(GPIO_Pin_9); // 串口初始化

	// 串口基本函数
	// (1) 发送字节数据
	Serial_SendByte(0x41); // 串口发送一个字节数据0x41
	// (2) 发送字节数据
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45}; // 定义数组
	Serial_SendArray(MyArray, 4);				  // 串口发送一个数组
	// (3) 发送字节数据
	Serial_SendString("\r\nNum="); // 串口发送字符串
	// (4) 发送字节数据
	Serial_SendNumber(123, 3); // 串口发送数字

	while (1)
	{
	}
}
