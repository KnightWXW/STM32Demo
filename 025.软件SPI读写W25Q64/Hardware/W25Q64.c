#include "stm32f10x.h" // Device header
#include "MySPI.h"
#include "W25Q64_ins.h"

/**
 * @description: W25Q64初始化
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @return: {*}
 */
void W25Q64_Init(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO)
{
    MySPI_Init(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO); // 初始化SPI模块
}

/**
 * @description: 读取ID号
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @param {uint8_t} *MID 工厂ID
 * @param {uint16_t} *DID 设备ID
 * @return: {*}
 */
void W25Q64_ReadID(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO, uint8_t *MID, uint16_t *DID)
{
    MySPI_Start(GPIO_Pin_SS);                                                                           // SPI起始
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_JEDEC_ID);           // 交换发送读取ID的指令
    *MID = MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_DUMMY_BYTE);  // 交换接收MID，通过输出参数返回
    *DID = MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_DUMMY_BYTE);  // 交换接收DID高8位
    *DID <<= 8;                                                                                         // 高8位移到高位
    *DID |= MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_DUMMY_BYTE); // 或上交换接收DID的低8位，通过输出参数返回
    MySPI_Stop(GPIO_Pin_SS);                                                                            // SPI终止
}

/**
 * @description: W25Q64写使能
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @return: {*}
 */
void W25Q64_WriteEnable(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO)
{
    MySPI_Start(GPIO_Pin_SS);                                                                     // SPI起始
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_WRITE_ENABLE); // 交换发送写使能的指令
    MySPI_Stop(GPIO_Pin_SS);                                                                      // SPI终止
}

/**
 * @description: W25Q64等待忙状态
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @return: {*}
 */
void W25Q64_WaitBusy(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO)
{
    uint32_t Timeout = 100000;                                                                                          // 给定超时计数时间
    MySPI_Start(GPIO_Pin_SS);                                                                                           // SPI起始
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_READ_STATUS_REGISTER_1);             // 交换发送读状态寄存器1的指令
    while ((MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_DUMMY_BYTE) & 0x01) == 0x01) // 循环等待忙标志位
    {
        Timeout--;
        if (Timeout == 0)
        {
            break;
        }
    }
    MySPI_Stop(GPIO_Pin_SS); // SPI终止
}

/**
 * @description: W25Q64页编程
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @param {uint32_t} Address 目的地址[0x000000,0x7FFFFF]
 * @param {uint8_t} *DataArray 数据数组
 * @param {uint16_t} Count 数组长度
 * @return: {*}
 */
void W25Q64_PageProgram(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO, uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    W25Q64_WriteEnable(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO); // 写使能

    MySPI_Start(GPIO_Pin_SS);                                                                     // SPI起始
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_PAGE_PROGRAM); // 交换发送页编程的指令
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address >> 16);       // 交换发送地址23~16位
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address >> 8);        // 交换发送地址15~8位
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address);             // 交换发送地址7~0位
    for (uint16_t i = 0; i < Count; i++)                                                          // 循环Count次
    {
        MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, DataArray[i]); // 依次在起始地址后写入数据
    }
    MySPI_Stop(GPIO_Pin_SS); // SPI终止

    W25Q64_WaitBusy(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO); // 等待忙状态
}

/**
 * @description: W25Q64扇区擦除(4KB)
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @param {uint32_t} Address 目的地址[0x000000,0x7FFFFF]
 * @return: {*}
 */
void W25Q64_SectorErase(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO, uint32_t Address)
{
    W25Q64_WriteEnable(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO); // 写使能

    MySPI_Start(GPIO_Pin_SS);                                                                         // SPI起始
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_SECTOR_ERASE_4KB); // 交换发送扇区擦除的指令
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address >> 16);           // 交换发送地址23~16位
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address >> 8);            // 交换发送地址15~8位
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address);                 // 交换发送地址7~0位
    MySPI_Stop(GPIO_Pin_SS);                                                                          // SPI终止

    W25Q64_WaitBusy(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO); // 等待忙状态
}

/**
 * @description: W25Q64读取数据
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @param {uint32_t} Address 目的地址[0x000000,0x7FFFFF]
 * @param {uint8_t} *DataArray 数据数组
 * @param {uint32_t} Count 数组长度
 * @return: {*}
 */
void W25Q64_ReadData(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO, uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
    MySPI_Start(GPIO_Pin_SS);                                                                  // SPI起始
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_READ_DATA); // 交换发送读取数据的指令
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address >> 16);    // 交换发送地址23~16位
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address >> 8);     // 交换发送地址15~8位
    MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, Address);          // 交换发送地址7~0位
    for (uint32_t i = 0; i < Count; i++)                                                       // 循环Count次
    {
        DataArray[i] = MySPI_SwapByte(GPIO_Pin_SS, GPIO_Pin_SCK, GPIO_Pin_MOSI, GPIO_Pin_MISO, W25Q64_DUMMY_BYTE); // 依次在起始地址后读取数据
    }
    MySPI_Stop(GPIO_Pin_SS); // SPI终止
}
