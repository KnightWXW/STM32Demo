#include "stm32f10x.h"

/**
 * @description: ADC初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void AD_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC1

    // 设置ADC时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz

    // GPIO初始化
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,PIN引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_AIN;     // GPIO模式,模拟输入模式
    GPIO_Init(GPIOA, &gpioStructure);

    // ADC初始化
    ADC_InitTypeDef adcInitStructure;
    ADC_StructInit(&adcInitStructure);
    adcInitStructure.ADC_Mode = ADC_Mode_Independent;                  // 模式:选择独立模式,即单独使用ADC1
    adcInitStructure.ADC_ScanConvMode = DISABLE;                       // 扫描模式:失能,只转换规则组的序列1这一个位置
    adcInitStructure.ADC_ContinuousConvMode = DISABLE;                 // 连续转换:失能,每转换一次规则组序列后停止
    adcInitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发:使用软件触发,不需要外部触发;
    adcInitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 数据对齐:选择右对齐;
    adcInitStructure.ADC_NbrOfChannel = 1;                             // 通道数:为1，仅在扫描模式下,才需要指定大于1的数,在非扫描模式下,只能是1
    ADC_Init(ADC1, &adcInitStructure);

    // ADC使能
    ADC_Cmd(ADC1, ENABLE);

    // ADC校准
    ADC_ResetCalibration(ADC1); // 固定流程，内部有电路会自动执行校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
 * @description: 获取ADC转换后的数值
 * @param {uint8_t} ADC_Channel
 * @return: {*}
 */
uint16_t ADC_GetValue(uint8_t ADC_Channel)
{
    // 规则组通道配置:规则组序列1的位置,配置为通道0
    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                 // 软件触发AD转换一次
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // 等待EOC标志位，即等待AD转换结束
    return ADC_GetConversionValue(ADC1);                    // 读数据寄存器，得到AD转换的结果
}
