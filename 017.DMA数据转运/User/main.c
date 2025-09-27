#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "DMA.h"
#include "Delay.h"

uint16_t dmaCount;							// 记录转运次数
uint8_t dataA[] = {0x01, 0x02, 0x03, 0x04}; // 定义测试数组DataA，为数据源
uint8_t dataB[] = {0, 0, 0, 0};				// 定义测试数组DataB，为数据目的地

int main(void)
{
	dmaCount = 4;					  // 设置转运次数为 4
	OLED_Init();					  // 初始化 OLED模块
	MyDMA_Init((uint32_t)dataA, (uint32_t)dataB, dmaCount); // 初始化 DMA模块

	OLED_ShowString(1, 1, "DataA");
	OLED_ShowString(3, 1, "DataB");
	// 显示两个数组首地址
	OLED_ShowHexNum(1, 8, (uint32_t)dataA, 8);
	OLED_ShowHexNum(3, 8, (uint32_t)dataB, 8);
	while (1)
	{
		// 变换测试数据数组A
		dataA[0]++;
		dataA[1]++;
		dataA[2]++;
		dataA[3]++;

		// 显示DMA之前的数组dataA
		OLED_ShowHexNum(2, 1, dataA[0], 2);
		OLED_ShowHexNum(2, 4, dataA[1], 2);
		OLED_ShowHexNum(2, 7, dataA[2], 2);
		OLED_ShowHexNum(2, 10, dataA[3], 2);
		// 显示DMA之前的数组dataB
		OLED_ShowHexNum(4, 1, dataB[0], 2);
		OLED_ShowHexNum(4, 4, dataB[1], 2);
		OLED_ShowHexNum(4, 7, dataB[2], 2);
		OLED_ShowHexNum(4, 10, dataB[3], 2);
		Delay_ms(1000);	  // 延时1s，观察转运前的现象
		MyDMA_Transfer(); // 利用DMA,DataA转运到DataB转运数组
		// 显示DMA之后的数组dataA
		OLED_ShowHexNum(2, 1, dataA[0], 2);
		OLED_ShowHexNum(2, 4, dataA[1], 2);
		OLED_ShowHexNum(2, 7, dataA[2], 2);
		OLED_ShowHexNum(2, 10, dataA[3], 2);
		// 显示DMA之后的数组dataB
		OLED_ShowHexNum(4, 1, dataB[0], 2);
		OLED_ShowHexNum(4, 4, dataB[1], 2);
		OLED_ShowHexNum(4, 7, dataB[2], 2);
		OLED_ShowHexNum(4, 10, dataB[3], 2);
		Delay_ms(1000); // 延时1s，观察转运后的现象
	}
}
