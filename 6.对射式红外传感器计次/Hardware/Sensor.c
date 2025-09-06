#include "stm32f10x.h"

extern uint16_t sensorCnt; // 对射红外传感器计数

/**
 * @description: 光敏传感器初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void Sensor_Init(uint16_t GPIO_Pin)
{
    // 开启 GPIOA 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化 GPIO
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;     // GPIO模式,上拉输入模式
    GPIO_Init(GPIOA, &gpioStructure);
}

/**
 * @description: 初始化EXTI外部中断
 * @return: {*}
 */
void Sensor_EXTI_Init()
{
    // 开启 AFIO 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // AFIO选择中断引脚
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_Pin);

    // 初始化EXTI
    EXTI_InitTypeDef exitStructure;
    EXTI_StructInit(&exitStructure);
    exitStructure.EXTI_Line = EXTI_Line8;                     // 选择配置外部中断的8号线
    exitStructure.EXTI_Mode = EXTI_Mode_Interrupt;            // 指定外部中断线为中断模式
    exitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 指定外部中断线为下降沿触发
    exitStructure.EXTI_LineCmd = ENABLE;                      // 指定外部中断线使能
    EXTI_Init(&exitStructure);                                // 配置EXTI外设初始化

    // NVIC中断分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 配置NVIC为分组2
    // 即抢占优先级范围：0~3，响应优先级范围：0~3
    // 此分组配置在整个工程中仅需调用一次
    // 若有多个中断，可以把此代码放在main函数内，while循环之前
    // 若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置

    // NVIC初始化
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = EXTI9_5_IRQn;        // 选择配置NVIC的 EXTI9_5 通道
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 2; // 指定NVIC线路的抢占优先级为2
    nvicStructure.NVIC_IRQChannelSubPriority = 2;        // 指定NVIC线路的响应优先级为2
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;           // NVIC外设初始化
    NVIC_Init(&nvicStructure);
}

/**
 * @description: EXTI 中断函数
 * @return: {*}
 *      此函数为中断函数，无需调用，中断触发后自动执行
 *      函数名为预留的指定名称，可以从启动文件 startup_stm32f10x_md.s 复制
 *      确保函数名正确，不能有任何差异，否则中断函数将不能进入
 */
void EXTI9_5_IRQHandler(void)
{
    // 判断是否是外部中断 8号通道 触发的中断
    if (EXTI_GetITStatus(EXTI_Line8) == SET)
    {
        // 如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)
        {
            sensorCnt++; // 计数值自增一次
        }
        // 清除外部中断 8号线 的中断标志位
        // 中断标志位必须清除
        // 否则中断将连续不断地触发，导致主程序卡死
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}

/**
 * @description: 获取光敏传感器计数值
 * @return: {*}
 */
uint16_t Sensor_GetNum()
{
    return sensorCnt;
}
