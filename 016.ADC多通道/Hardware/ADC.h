#ifndef _ADC_H_
#define _ADC_H_

void AD_Init(uint16_t GPIO_Pin);
uint16_t ADC_GetValue(uint8_t ADC_Channel);

#endif
