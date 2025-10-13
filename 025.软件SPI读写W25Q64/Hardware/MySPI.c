#include "stm32f10x.h" // Device header

/**
 * @description: SPI写入SS引脚电平
 * @param {uint16_t} GPIO_Pin
 * @param {uint8_t} BitValue
 * @return: {*}
 */
void MySPI_W_SS(uint16_t GPIO_Pin, uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin, (BitAction)BitValue);
}

/**
 * @description: SPI写入SCK引脚电平
 * @param {uint16_t} GPIO_Pin
 * @param {uint8_t} BitValue
 * @return: {*}
 */
void MySPI_W_SCK(uint16_t GPIO_Pin, uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin, (BitAction)BitValue);
}

/**
 * @description: SPI写入MOSI引脚电平
 * @param {uint16_t} GPIO_Pin
 * @param {uint8_t} BitValue
 * @return: {*}
 */
void MySPI_W_MOSI(uint16_t GPIO_Pin, uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin, (BitAction)BitValue);
}

/**
 * @description: SPI读取MISO引脚电平
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
uint8_t MySPI_R_MISO(uint16_t GPIO_Pin)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin);
}

/**
 * @description: SPI初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void MySPI_Init(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA

    // GPIO初始化
    // 输出引脚
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_SS | GPIO_Pin_SCK | GPIO_Pin_MOSI; // GPIO引脚, GPIO_Pin_SS | GPIO_Pin_SCK | GPIO_Pin_MOSI 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;                         // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;                          // GPIO模式,推挽输出模式
    GPIO_Init(GPIOA, &gpioStructure);
    // 输入引脚
    gpioStructure.GPIO_Pin = GPIO_Pin_MISO;      // GPIO引脚,GPIO_Pin_MISO 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;     // GPIO模式,上拉输入模式
    GPIO_Init(GPIOA, &gpioStructure);

    // 设置默认电平
    MySPI_W_SS(GPIO_Pin_SS, 1);   // SS默认高电平
    MySPI_W_SCK(GPIO_Pin_SCK, 0); // SCK默认低电平
}

/**
 * @description: SPI起始
 * @param {uint16_t} GPIO_Pin_SS
 * @return: {*}
 */
void MySPI_Start(uint16_t GPIO_Pin_SS)
{
    MySPI_W_SS(GPIO_Pin_SS, 0); // 拉低SS，开始时序
}

/**
 * @description: SPI终止
 * @param {uint16_t} GPIO_Pin_SS
 * @return: {*}
 */
void MySPI_Stop(uint16_t GPIO_Pin_SS)
{
    MySPI_W_SS(GPIO_Pin_SS, 1); // 拉高SS，终止时序
}

/**
 * @description: SPI交换一个字节数据(模式0)
 * @param {uint16_t} GPIO_Pin_SS
 * @param {uint16_t} GPIO_Pin_SCK
 * @param {uint16_t} GPIO_Pin_MOSI
 * @param {uint16_t} GPIO_Pin_MISO
 * @param {uint8_t} ByteSend
 * @return: {*}
 */
uint8_t MySPI_SwapByte(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO, uint8_t ByteSend)
{
    uint8_t ByteReceive = 0x00;
    for (uint8_t i = 0; i < 8; i++)
    {
        MySPI_W_MOSI(GPIO_Pin_MOSI, ByteSend & (0x80 >> i));
        MySPI_W_SCK(GPIO_Pin_SCK, 1);
        if (MySPI_R_MISO(GPIO_Pin_MISO) == 1)
        {
            ByteReceive |= (0x80 >> i);
        }
        MySPI_W_SCK(GPIO_Pin_SCK, 0);
    }
    return ByteReceive;
}