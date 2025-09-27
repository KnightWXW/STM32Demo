#ifndef __AD_DMA_H
#define __AD_DMA_H

extern uint16_t ADC_Value[4]; // 存放AD转换结果的全局数组

void DMA_ADC_Init(uint16_t GPIO_Pin);

#endif
