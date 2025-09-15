#ifndef _LED_H_
#define _LED_H_

void Sensor_Init(uint16_t GPIO_Pin);
uint8_t Sensor_GetNum(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin);

#endif
