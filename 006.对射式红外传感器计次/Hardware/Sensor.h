#ifndef _SENSOR_H_
#define _SENSOR_H_

void Sensor_Init(uint16_t GPIO_Pin);
void Sensor_EXTI_Init();
uint8_t Sensor_GetNum();

#endif
