#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Key.h"
#include "BKP.h"
#include "Delay.h"

uint16_t keyNum;						   // 按键键值
uint16_t writeArray[2] = {0x0000, 0x0000}; // 记录要写入数据的测试数组
uint16_t readArray[2] = {0};			   // 记录要读取数据的测试数组

int main(void)
{
	OLED_Init();		   // 初始化 OLED模块
	Key_Init(GPIO_Pin_10); // 初始化 Key模块
	BKP_Init();			   // 初始化 BKP模块

	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");

	while (1)
	{
		keyNum = Key_GetNum(GPIOB, GPIO_Pin_10);
		if (keyNum == 1)
		{
			writeArray[0]++;
			writeArray[1]++;
			// 写入测试数据到备份寄存器
			BKP_WriteBackupRegister(BKP_DR1, writeArray[0]);
			BKP_WriteBackupRegister(BKP_DR2, writeArray[1]);
			OLED_ShowHexNum(1, 3, writeArray[0], 4);
			OLED_ShowHexNum(1, 8, writeArray[1], 4);
		}
		// 读取备份寄存器的数据
		readArray[0] = BKP_ReadBackupRegister(BKP_DR1);
		readArray[1] = BKP_ReadBackupRegister(BKP_DR2);
		OLED_ShowHexNum(2, 3, readArray[0], 4);
		OLED_ShowHexNum(2, 8, readArray[1], 4);
	}
}
