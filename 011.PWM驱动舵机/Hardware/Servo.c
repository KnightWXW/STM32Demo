#include "stm32f10x.h"
#include "PWM.h"

/**
 * @description: 初始化舵机的GPIO 与 PWM 模块
 * @return: {*}
 */
void Servo_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // GPIO模式,复用推挽输出模式
    gpioStructure.GPIO_Pin = GPIO_Pin_1;         // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    GPIO_Init(GPIOA, &gpioStructure);

    // PWM初始化
    PWM_Init();
}

/**
 * @description: 设置舵机的角度值
 * @param {float} angle
 * @return: {*}
 */
void Servo_SetAngle(float angle)
{
    // 设置占空比，角度线性变换，对应到舵机要求的占空比范围上
    PWM_SetCompare2(angle / 180 * 2000 + 500);
}
