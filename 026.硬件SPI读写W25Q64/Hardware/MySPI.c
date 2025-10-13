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
 * @description: SPI初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void MySPI_Init(uint16_t GPIO_Pin_SS, uint16_t GPIO_Pin_SCK, uint16_t GPIO_Pin_MOSI, uint16_t GPIO_Pin_MISO)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  // SPI1

    // GPIO初始化
    // SS引脚:推挽输出(软件模拟)
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_SS;        // GPIO引脚, GPIO_Pin_SS 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // GPIO模式,推挽输出模式
    GPIO_Init(GPIOA, &gpioStructure);
    // SCK与MOSI引脚:复用推挽输出(片上外设控制)
    gpioStructure.GPIO_Pin = GPIO_Pin_SCK | GPIO_Pin_MOSI; // GPIO引脚, GPIO_Pin_SCK | GPIO_Pin_MOSI 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;           // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;             // GPIO模式,复用推挽输出模式
    GPIO_Init(GPIOA, &gpioStructure);
    // MISO引脚:上拉输入
    gpioStructure.GPIO_Pin = GPIO_Pin_MISO;      // GPIO引脚,GPIO_Pin_MISO 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;     // GPIO模式,上拉输入模式
    GPIO_Init(GPIOA, &gpioStructure);

    // SPI初始化
    SPI_InitTypeDef spiInitStructure;
    spiInitStructure.SPI_Mode = SPI_Mode_Master;                        // 模式:选择为SPI主模式
    spiInitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // 方向:选择2线全双工
    spiInitStructure.SPI_DataSize = SPI_DataSize_8b;                    // 数据宽度:选择为8位
    spiInitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                   // 先行位:选择高位先行
    spiInitStructure.SPI_CPOL = SPI_CPOL_Low;                           // SPI极性:选择低极性
    spiInitStructure.SPI_CPHA = SPI_CPHA_1Edge;                         // SPI相位:选择第一个时钟边沿采样,极性和相位决定选择SPI模式0
    spiInitStructure.SPI_NSS = SPI_NSS_Soft;                            // NSS:选择由软件控制
    spiInitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 波特率分频:选择128分频
    spiInitStructure.SPI_CRCPolynomial = 7;                             // CRC多项式:暂时用不到,给默认值7
    SPI_Init(SPI1, &spiInitStructure);

    // SPI使能
    SPI_Cmd(SPI1, ENABLE);

    // 设置默认电平
    MySPI_W_SS(GPIO_Pin_SS, 1); // SS默认高电平
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
 * @param {uint8_t} ByteSend
 * @return: {*}
 */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
    // 发送数据
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET)
        ;                             // 等待发送数据寄存器空
    SPI_I2S_SendData(SPI1, ByteSend); // 写入数据到发送数据寄存器,开始产生时序
    // 接收数据
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET)
        ;                             // 等待接收数据寄存器非空
    return SPI_I2S_ReceiveData(SPI1); // 读取接收到的数据并返回
}
