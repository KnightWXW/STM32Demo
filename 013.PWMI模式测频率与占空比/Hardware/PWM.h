#ifndef _PWM_H_
#define _PWM_H_

void PWM_Init(uint16_t GPIO_Pin);
void PWM_SetCompare1(uint16_t compareValue);
void PWM_SetPrescaler(uint16_t prescaler);

#endif
