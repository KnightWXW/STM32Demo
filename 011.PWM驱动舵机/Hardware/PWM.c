#include "stm32f10x.h"

/**
 * @description: PWM初始化
 * @return: {*}
 *   选择 TIM2的OC2通道(TIM2_CH2),对应的引脚为PA1
 */
void PWM_Init(void)
{
    // 开启 TIM2 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 设置 时钟源 为 内部时钟源
    TIM_InternalClockConfig(TIM2);

    // 配置时基单元
    // 舵机的工作频率 50Hz, 对应ARR为 20000 - 1
    TIM_TimeBaseInitTypeDef timBaseStructure;
    timBaseStructure.TIM_Prescaler = 72 - 1;               // 预分频器,即PSC的值
    timBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式,选择向上计数
    timBaseStructure.TIM_Period = 20000 - 1;               // 计数周期,即ARR的值
    timBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频,选择不分频,此参数用于配置滤波器时钟，不影响时基单元功能
    timBaseStructure.TIM_RepetitionCounter = 0;            // 重复计数器,高级定时器才会用到
    TIM_TimeBaseInit(TIM2, &timBaseStructure);

    // 配置 OC 输出比较单元
    TIM_OCInitTypeDef timOCInitStructure;
    TIM_OCStructInit(&timOCInitStructure); // 使用 默认值 初始化 输出比较单元结构体

    timOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 配置 PWM 输出模式:TIM脉冲宽度调制模式1
    timOCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 配置输出比较使能
    timOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // 配置输出比较极性
    timOCInitStructure.TIM_Pulse = 0x0000;                       // 配置输出比较脉冲宽度,初始的CCR值
    TIM_OC2Init(TIM2, &timOCInitStructure);

    // TIM使能,TIM2开始运行
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @description: 设置PWM 写入的 CCR 的值
 * @param {uint16_t} compareValue [0, 100]
 * @return: {*}
 */
void PWM_SetCompare2(uint16_t compareValue)
{
    TIM_SetCompare2(TIM2, compareValue); // 设置CCR2的值
}
