#include "stm32f10x.h"

uint16_t ADC_Value[4]; // 存放AD转换结果的全局数组

/**
 * @description: DMA&ADC 初始化
 * @return: {*}
 */
void DMA_ADC_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    // DMA1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA

    // 设置ADC时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz

    // GPIO初始化
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,PIN引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_AIN;     // GPIO模式,模拟输入模式
    GPIO_Init(GPIOA, &gpioStructure);

    // 规则组通道配置
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); // 规则组序列1的位置,配置为通道0
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); // 规则组序列2的位置,配置为通道1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5); // 规则组序列3的位置,配置为通道2
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5); // 规则组序列4的位置,配置为通道3

    // ADC初始化
    ADC_InitTypeDef adcInitStructure;
    adcInitStructure.ADC_Mode = ADC_Mode_Independent;                  // 模式:选择独立模式,即单独使用ADC1;
    adcInitStructure.ADC_ScanConvMode = ENABLE;                        // 连续转换:使能,每转换一次规则组序列后立刻开始下一次转换;
    adcInitStructure.ADC_ContinuousConvMode = ENABLE;                  // 扫描模式:使能,扫描规则组的序列,扫描数量由ADC_NbrOfChannel确定;
    adcInitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发:使用软件触发,不需要外部触发;
    adcInitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 数据对齐:选择右对齐;
    adcInitStructure.ADC_NbrOfChannel = 4;                             // 通道数:4,仅在扫描模式下,才需要指定大于1的数,在非扫描模式下,只能是1;
    ADC_Init(ADC1, &adcInitStructure);

    // DMA初始化
    DMA_InitTypeDef dmaInitStructure;
    // 外设单元
    dmaInitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             // 外设基地址
    dmaInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设地址长度
    dmaInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址是否自增
    // 存储器单元
    dmaInitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;         // 存储器基地址
    dmaInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 存储器地址长度
    dmaInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // 存储器地址是否自增
    // 其余配置
    dmaInitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // 数据转运方向:自外设到存储器
    dmaInitStructure.DMA_BufferSize = 4;               // 转运数据大小:转运次数
    dmaInitStructure.DMA_Mode = DMA_Mode_Circular;     // 数据转运模式:选择正常模式
    dmaInitStructure.DMA_Priority = DMA_Priority_High; // 数据转运优先级:高等
    dmaInitStructure.DMA_M2M = DMA_M2M_Disable;        // 存储器到存储器:选择使能
    DMA_Init(DMA1_Channel1, &dmaInitStructure);        // 使用结构体初始化配置DMA1的通道1

    // ADC&DMA使能
    ADC_Cmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); // 首先不使能,初始化后不立刻工作,等后续转运后再使能

    // ADC校正
    ADC_ResetCalibration(ADC1); // 固定流程，内部有电路会自动执行校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

    // ADC触发
    // 软件触发ADC开始工作，由于ADC处于连续转换模式，故触发一次后ADC就可以一直连续不断地工作
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
