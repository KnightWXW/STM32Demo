/*
 * @Author: KnightWXW knight960903@gmail.com
 * @Date: 2025-08-31 10:05:35
 * @LastEditors: KnightWXW knight960903@gmail.com
 * @LastEditTime: 2025-09-04 23:10:40
 * @FilePath: \STM32Demo\STM32Demo\2.LED流水灯\Hardware\LED.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "stm32f10x.h"

/* 
    如果LED正极接电源正极,LED负极接GPIO接口，此时为低电平触发
    如果LED正极接GPIO接口,LED负极接电源负极，此时为高电平触发
*/

/**
 * @description: LED初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void LED_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化 GPIO
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,GPIO_Pin 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // GPIO模式,推挽输出模式
    GPIO_Init(GPIOA, &gpioStructure);
}

/**
 * @description: LED 灭灯
 * @param {GPIO_TypeDef*} GPIO
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void LED_OFF(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin)
{
    // LED 亮 为低电平触发,灯灭时需要引脚输出高电平
    GPIO_SetBits(GPIO, GPIO_Pin);
}

/**
 * @description: LED 亮灯
 * @param {GPIO_TypeDef*} GPIO
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void LED_ON(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin)
{
    // LED 亮 为低电平触发,灯亮时需要引脚输出低电平
    GPIO_ResetBits(GPIO, GPIO_Pin);
}
