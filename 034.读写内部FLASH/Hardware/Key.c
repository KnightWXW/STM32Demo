#include "stm32f10x.h"
#include "Delay.h"

/*
    按键释放时 为 高电平，此时电平为 GPIO 的电平
    按键按下时 为 低电平，此时电平为 GND 的电平
*/

/**
 * @description: Key 初始化
 * @return: {*}
 */
void Key_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 初始化 GPIO
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11; // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;       // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;           // GPIO模式,上拉输入模式
    GPIO_Init(GPIOB, &gpioStructure);
}

/**
 * @description: 获取按键的键码值
 * @return: {*}
 */
uint16_t Key_GetNum()
{
    uint16_t keyNum = 0;
    // 消抖处理
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        Delay_ms(20); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);  // 等待按键松手
        Delay_ms(20); // 延时消抖
        keyNum = 1;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
    {
        Delay_ms(20); // 延时消抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);   // 等待按键松手
        Delay_ms(20); // 延时消抖
        keyNum = 2;
    }
    return keyNum;
}
