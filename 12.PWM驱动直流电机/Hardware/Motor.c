#include "stm32f10x.h"
#include "PWM.h"

/**
 * @description: 初始化舵机的GPIO 与 PWM 模块
 * @return: {*}
 */
void Motor_Init(uint16_t GPIO_PinA, uint16_t GPIO_PinB)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

    // GPIO初始化
    // (1) 初始化TIM输出比较通道的引脚 为 复用推挽输出模式
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // GPIO模式,复用推挽输出模式
    gpioStructure.GPIO_Pin = GPIO_PinA;          // GPIO引脚,PINA 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    GPIO_Init(GPIOA, &gpioStructure);

    // (2) 初始化方向控制的两个引脚 为 开漏输出模式
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // GPIO模式,开漏输出模式
    gpioStructure.GPIO_Pin = GPIO_PinB;          // GPIO引脚,PINB 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    GPIO_Init(GPIOA, &gpioStructure);

    // PWM初始化
    PWM_Init();
}

/**
 * @description: 设置直流电机的的速度值
 * @param {int16_t} speed
 * @return: {*}
 */
void Motor_SetSpeed(int16_t speed)
{
    if (speed > 0)
    {
        // 正转
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        PWM_SetCompare3(speed);
    }
    else
    {
        // 反转
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
        PWM_SetCompare3(-speed);
    }
}
