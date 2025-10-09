#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_W_SCL(uint16_t GPIO_Pin, uint8_t BitValue);
void MyI2C_W_SDA(uint16_t GPIO_Pin, uint8_t BitValue);
uint8_t MyI2C_R_SDA(uint16_t GPIO_Pin);
void MyI2C_Init(uint16_t GPIO_Pin);
void MyI2C_Start(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA);
void MyI2C_Stop(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA);
void MyI2C_SendByte(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA, uint8_t Byte);
uint8_t MyI2C_ReceiveByte(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA);
void MyI2C_SendAck(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA, uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA);

#endif
