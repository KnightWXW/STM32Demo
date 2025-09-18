#include "stm32f10x.h"

/**
 * @description: IC 初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void IC_Init(uint16_t GPIO_Pin)
{
    // 开始时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // TIM3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA

    // GPIO初始化
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;     // GPIO模式,上拉输入模式
    GPIO_Init(GPIOA, &gpioStructure);

    // 配置时钟源为 内部时钟源
    TIM_InternalClockConfig(TIM3);

    // 配置时基单元
    // IC的ARR设置为最大值,防止计数溢出
    TIM_TimeBaseInitTypeDef timBaseStructure;
    timBaseStructure.TIM_Prescaler = 72 - 1;               // 预分频器,即PSC的值
    timBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式,选择向上计数
    timBaseStructure.TIM_Period = 65536 - 1;               // 计数周期,即ARR的值
    timBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频,选择不分频,此参数用于配置滤波器时钟，不影响时基单元功能
    timBaseStructure.TIM_RepetitionCounter = 0;            // 重复计数器,高级定时器才会用到
    TIM_TimeBaseInit(TIM3, &timBaseStructure);

    // PWMI模式初始化
    // TIM_PWMIConfig: 可以 同时 配置 两个通道
    // TIM_ICInit: 只能 一次配置 一个通道
    TIM_ICInitTypeDef icInitStructure;
    icInitStructure.TIM_Channel = TIM_Channel_1;                // 选择配置定时器通道1
    icInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     // 极性,选择为上升沿触发捕获
    icInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 输入信号交叉,选择直通,不交叉
    icInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // 捕获预分频,选择不分频,每次信号都触发捕获
    icInitStructure.TIM_ICFilter = 0xF;                         // 输入滤波器参数,可以过滤信号抖动
    TIM_PWMIConfig(TIM3, &icInitStructure);                     // 将结构体变量交给TIM_PWMIConfig，配置TIM3的输入捕获通道
                                                                // 此函数同时会把另一个通道配置为相反的配置，实现PWMI模式

    // 选择触发源及从模式
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);    // 触发源选择TI1FP1
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 从模式选择复位,即TI1产生上升沿时，会触发CNT归零

    // TIM使能,定时器开始运行
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @description: 获取输入捕获频率
 * @return: {*}
 */
uint32_t IC_GetFreq(void)
{
    // 测周法得到频率 fx = fc / N, 这里不执行 +1 的操作也可以
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

/**
 * @description: 获取输入捕获占空比
 * @return: {*}
 */
uint32_t IC_GetDuty(void)
{
    // CCR2 存储 高电平计数
    // CCR1 存储 满周期计数
    // 占空比 Duty = CCR2 / CCR1 * 100, 不执行 +1 的操作也可
    return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
