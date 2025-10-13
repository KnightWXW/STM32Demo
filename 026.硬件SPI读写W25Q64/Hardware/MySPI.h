#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_W_SS(uint16_t GPIO_Pin, uint8_t BitValue);
void MySPI_Init(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO);
void MySPI_Start(uint16_t GPIO_Pin_SS);
void MySPI_Stop(uint16_t GPIO_Pin_SS);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
