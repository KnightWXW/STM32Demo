#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO);
void W25Q64_WriteEnable(uint16_t GPIO_Pin_SS);
void W25Q64_WaitBusy(uint16_t GPIO_Pin_SS);
void W25Q64_ReadID(uint16_t GPIO_Pin_SS, uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram(uint16_t GPIO_Pin_SS, uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_SectorErase(uint16_t GPIO_Pin_SS, uint32_t Address);
void W25Q64_ReadData(uint16_t GPIO_Pin_SS, uint32_t Address, uint8_t *DataArray, uint32_t Count);

#endif
