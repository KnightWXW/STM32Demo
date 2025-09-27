#include "stm32f10x.h"

extern uint16_t dmaCount; // 记录转运次数

/**
 * @description: DMA初始化
 * @param {uint32_t} AddrA
 * @param {uint32_t} AddrB
 * @param {uint16_t} Size
 * @return: {*}
 */
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Count)
{
    uint16_t dmaCount = Count; // 记录转运次数

    // 开启时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // DMA1

    // DMA初始化
    DMA_InitTypeDef dmaInitStructure;
    // 外设单元
    dmaInitStructure.DMA_PeripheralBaseAddr = AddrA;                       // 外设基地址
    dmaInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设地址长度
    dmaInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;         // 外设地址是否自增
    // 存储器单元
    dmaInitStructure.DMA_MemoryBaseAddr = AddrB;                   // 存储器基地址
    dmaInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 存储器地址长度
    dmaInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         // 存储器地址是否自增
    // 其余配置
    dmaInitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // 数据转运方向:自外设到存储器
    dmaInitStructure.DMA_BufferSize = Count;           // 转运数据大小:转运次数
    dmaInitStructure.DMA_Mode = DMA_Mode_Normal;       // 数据转运模式:选择正常模式
    dmaInitStructure.DMA_Priority = DMA_Priority_High; // 数据转运优先级:高等
    dmaInitStructure.DMA_M2M = DMA_M2M_Enable;         // 存储器到存储器:选择使能
    DMA_Init(DMA1_Channel1, &dmaInitStructure);        // 使用结构体初始化配置DMA1的通道1

    // DMA使能
    DMA_Cmd(DMA1_Channel1, DISABLE); // 首先不使能,初始化后不立刻工作,等后续转运后再使能
}

/**
 * @description: DMA转运数据
 * @return: {*}
 */
void MyDMA_Transfer(void)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);                    // DMA去使能,在写入传输计数器之前,需要DMA暂停工作
    DMA_SetCurrDataCounter(DMA1_Channel1, dmaCount);    // 进行数据转运,转运次数为dmaCount
    DMA_Cmd(DMA1_Channel1, ENABLE);                     // DMA使能,开始工作

    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);  // 等待DMA工作完成
    DMA_ClearFlag(DMA1_FLAG_TC1);                       // 清除工作完成标志位
}