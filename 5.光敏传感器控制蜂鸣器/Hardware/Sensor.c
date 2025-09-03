#include "stm32f10x.h"

/**
 * @description: 光敏传感器初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void Sensor_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化 GPIO
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;     // GPIO模式,上拉输入模式
    GPIO_Init(GPIOA, &gpioStructure);
}

/**
 * @description: 获取光敏传感器当前状态
 * @param {GPIO_TypeDef*} GPIO
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
uint8_t Sensor_GetNum(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin)
{
    return GPIO_ReadInputDataBit(GPIO, GPIO_Pin);
}
