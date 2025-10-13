#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Delay.h"
#include "MPU6050.h"

uint8_t ID;						// 定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ; // 定义用于存放各个数据的变量

int main(void)
{
	OLED_Init();							 // 初始化 OLED模块
	MPU6050_Init(GPIO_Pin_10 | GPIO_Pin_11); // MPU6050初始化

	/*显示ID号*/
	OLED_ShowString(1, 1, "ID:"); // 显示静态字符串
	ID = MPU6050_GetID();		  // 获取MPU6050的ID号
	OLED_ShowHexNum(1, 4, ID, 2); // OLED显示ID号

	while (1)
	{
		// 获取MPU6050的数据
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		// OLED显示数据
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}
