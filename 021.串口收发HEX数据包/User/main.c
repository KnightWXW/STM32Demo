#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Key.h"
#include "Serial.h"

uint8_t KeyNum; // 接收按键键码的变量

int main(void)
{
	OLED_Init();						  // 初始化 OLED模块
	Key_Init(GPIO_Pin_1);				  // 初始化 Key模块
	Serial_Init(GPIO_Pin_10, GPIO_Pin_9); // 初始化 Serial模块

	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");

	// 设置发送数据包数组的初始值
	Serial_TxPacket[0] = 0x01;
	Serial_TxPacket[1] = 0x02;
	Serial_TxPacket[2] = 0x03;
	Serial_TxPacket[3] = 0x04;

	while (1)
	{
		// 按键后,发送数组的数据自增,然后发送数据并显示
		KeyNum = Key_GetNum(GPIOB, GPIO_Pin_1);
		if (KeyNum == 1)
		{
			// 发送数组的数据自增
			Serial_TxPacket[0]++;
			Serial_TxPacket[1]++;
			Serial_TxPacket[2]++;
			Serial_TxPacket[3]++;

			Serial_SendPacket(); // 串口发送数据包Serial_TxPacket

			OLED_ShowHexNum(2, 1, Serial_TxPacket[0], 2); // 显示发送的数据包
			OLED_ShowHexNum(2, 4, Serial_TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, Serial_TxPacket[2], 2);
			OLED_ShowHexNum(2, 10, Serial_TxPacket[3], 2);
		}
		// 如果接收到数据包,显示接收的数据包
		if (Serial_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
		}
	}
}
