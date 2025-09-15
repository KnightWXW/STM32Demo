#include "stm32f10x.h"

extern uint16_t timerCnt; // 定时器计数

/**
 * @description: 定时器初始化
 * @return: {*}
 */
void Timer_Init(void)
{
    // 开启TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 选择内部时钟源
    TIM_InternalClockConfig(TIM3);

    // 时基单元初始化
    TIM_TimeBaseInitTypeDef timerBaseStructure;
    TIM_TimeBaseStructInit(&timerBaseStructure);             // 默认配置初始化时基单元结构体
    timerBaseStructure.TIM_Prescaler = 7200 - 1;             // 预分频器,即PSC的值
    timerBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式,选择向上计数
    timerBaseStructure.TIM_Period = 10000 - 1;               // 计数周期,即ARR的值
    timerBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频,选择不分频,此参数用于配置输入滤波器时钟,不影响时基单元功能
    timerBaseStructure.TIM_RepetitionCounter = 0;            // 重复计数器,高级定时器才会用到
    TIM_TimeBaseInit(TIM3, &timerBaseStructure);

    // 中断输出配置
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    // 清除定时器更新标志位
    // TIM_TimeBaseInit函数末尾,手动产生了更新事件
    // 若不清除此标志位,则开启中断后,会立刻进入一次中断
    // 如果不介意此问题,则不清除此标志位也可

    // 开启更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 开启TIM3的更新中断

    // NVIC配置中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 即抢占优先级范围:0~3,响应优先级范围：0~3
    // 此分组配置在整个工程中仅需调用一次
    // 若有多个中断,可以把此代码放在main函数内,while循环之前
    // 若调用多次配置分组的代码,则后执行的配置会覆盖先执行的配置

    // NVIC初始化
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 2;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

    // 定时器使能
    TIM_Cmd(TIM3, ENABLE); // 使能TIM3,定时器开始运行
}

/**
 * @description: TIM3 中断函数
 * @return: {*}
 *  此函数为中断函数,无需调用,中断触发后自动执行
 *  函数名为预留的指定名称,可以从启动文件复制
 *  请确保函数名正确,不能有任何差异,否则中断函数将不能进入
 */
void TIM3_IRQHandler(void)
{
    // 判断是否是TIM3的更新事件触发的中断
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        timerCnt++;
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        // 清除TIM3更新事件的中断标志位
        // 中断标志位必须清除,否则中断将连续不断地触发,导致主程序卡死
    }
}
