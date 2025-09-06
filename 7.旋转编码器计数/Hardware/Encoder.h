#ifndef _ENCODER_H_
#define _ENCODER_H_

void Encoder_Init(uint16_t GPIO_Pin);
void Encoder_EXTI_Init(void);
uint8_t Encoder_GetNum();

#endif
