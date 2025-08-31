#ifndef _LED_H_
#define _LED_H_

void LED_Init(uint16_t GPIO_Pin);
void LED_ON(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin);
void LED_OFF(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin);
void LED_Turn(GPIO_TypeDef *GPIO, uint16_t GPIO_Pin);

#endif
