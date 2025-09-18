#include "stm32f10x.h"

extern int16_t encoderCnt; // 对射红外传感器计数

/**
 * @description: 旋转计数器引脚与输入捕获配置初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void Encoder_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // TIM3

    // 初始化 GPIO
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;     // GPIO模式,上拉输入模式
    GPIO_Init(GPIOA, &gpioStructure);

    // 配置时基单元
    // IC的ARR设置为最大值,防止计数溢出
    TIM_TimeBaseInitTypeDef timBaseStructure;
    timBaseStructure.TIM_Prescaler = 1 - 1;                // 预分频器,即PSC的值
    timBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式,选择向上计数
    timBaseStructure.TIM_Period = 65536 - 1;               // 计数周期,即ARR的值
    timBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频,选择不分频,此参数用于配置滤波器时钟，不影响时基单元功能
    timBaseStructure.TIM_RepetitionCounter = 0;            // 重复计数器,高级定时器才会用到
    TIM_TimeBaseInit(TIM3, &timBaseStructure);

    // PWMI模式初始化
    TIM_ICInitTypeDef icInitStructure;
    TIM_ICStructInit(&icInitStructure); // 使用默认数值进行结构体初始化

    icInitStructure.TIM_Channel = TIM_Channel_1; // 选择配置定时器通道1
    icInitStructure.TIM_ICFilter = 0xF;          // 输入滤波器参数,可以过滤信号抖动
    TIM_ICInit(TIM3, &icInitStructure);          // 将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道
    icInitStructure.TIM_Channel = TIM_Channel_2; // 选择配置定时器通道2
    icInitStructure.TIM_ICFilter = 0xF;          // 输入滤波器参数,可以过滤信号抖动
    TIM_ICInit(TIM3, &icInitStructure);          // 将结构体变量交给TIM_ICInit，配置TIM3的输入捕获通道

    // 编码器接口配置
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    // 配置编码器模式 以及两个输入通道是否反相
    // 注意此时参数的Rising和Falling不代表上升沿和下降沿,而是代表是否反相
    // 此函数必须在输入捕获初始化之后进行,否则输入捕获的配置会覆盖此函数的部分配置

    // TIM使能,定时器开始运行
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @description: 获取旋转计数器计数值
 * @return: {*}
 */
int16_t Encoder_GetNum(void)
{
    int16_t temCnt = TIM_GetCounter(TIM3);
    TIM_SetCounter(TIM3, 0);
    return temCnt;
}
