#include "stm32f10x.h" // Device header
#include "Delay.h"

/**
 * @description: I2C向SCL线写入引脚电平
 * @param {uint16_t} GPIO_Pin
 * @param {uint8_t} BitValue
 * @return: {*}
 */
void MyI2C_W_SCL(uint16_t GPIO_Pin, uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin, (BitAction)BitValue);
    Delay_us(10);
}

/**
 * @description: I2C向SDA线写入引脚电平
 * @param {uint16_t} GPIO_Pin
 * @param {uint8_t} BitValue
 * @return: {*}
 */
void MyI2C_W_SDA(uint16_t GPIO_Pin, uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin, (BitAction)BitValue);
    Delay_us(10);
}

/**
 * @description: I2C从SDA线读取数据
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
uint8_t MyI2C_R_SDA(uint16_t GPIO_Pin)
{
    uint8_t bitValue = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin);
    Delay_us(10);
    return bitValue;
}

/**
 * @description: I2C初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */
void MyI2C_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // GPIOB

    // 初始化 GPIO
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,GPIO_Pin 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // GPIO模式,开漏输出模式
    GPIO_Init(GPIOB, &gpioStructure);

    // 设置SCL和SDA引脚为默认高电平
    GPIO_SetBits(GPIOB, GPIO_Pin);
}

/**
 * @description: I2C设置起始位
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @return: {*}
 */
void MyI2C_Start(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA)
{
    MyI2C_W_SDA(GPIO_Pin_SDA, 1); // 释放SDA,确保SDA为高电平
    MyI2C_W_SCL(GPIO_Pin_SCL, 1); // 释放SCL,确保SCL为高电平
    MyI2C_W_SDA(GPIO_Pin_SDA, 0); // 在SCL高电平期间,拉低SDA,产生起始信号
    MyI2C_W_SCL(GPIO_Pin_SCL, 0); // 起始后把SCL也拉低,即占用总线,也方便总线时序的拼接
}

/**
 * @description: I2C设置停止位
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @return: {*}
 */
void MyI2C_Stop(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA)
{
    MyI2C_W_SDA(GPIO_Pin_SDA, 0); // 拉低SDA,确保SDA为低电平
    MyI2C_W_SCL(GPIO_Pin_SCL, 1); // 释放SCL,使SCL呈现高电平
    MyI2C_W_SDA(GPIO_Pin_SDA, 1); // 在SCL高电平期间,释放SDA,产生终止信号
}

/**
 * @description: I2C发送字节型数据
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @param {uint8_t} Byte
 * @return: {*}
 */
void MyI2C_SendByte(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA, uint8_t Byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(GPIO_Pin_SDA, Byte & (0x80 >> i)); // 使用掩码的方式取出Byte的指定一位数据并写入到SDA线
        MyI2C_W_SCL(GPIO_Pin_SCL, 1);                  // 释放SCL,从机在SCL高电平期间读取SDA
        MyI2C_W_SCL(GPIO_Pin_SCL, 0);                  // 拉低SCL,主机开始发送下一位数据
    }
    return;
}

/**
 * @description: I2C接收一个字节
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @return: {*}
 */
uint8_t MyI2C_ReceiveByte(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA)
{
    uint8_t ReceiveByte = 0x00;   // 定义接收的数据,此处必须赋初值0x00,后面会用到
    MyI2C_W_SDA(GPIO_Pin_SDA, 1); // 接收前,主机先确保释放SDA,避免干扰从机的数据发送
    for (uint8_t i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(GPIO_Pin_SCL, 1); // 释放SCL,主机在SCL高电平期间读取SDA
        // 读取SDA数据,并存储到Byte变量
        // 当SDA为1时,置变量指定位为1,当SDA为0时,不做处理,指定位为默认的初值0
        if (MyI2C_R_SDA(GPIO_Pin_SDA) == 1)
        {
            ReceiveByte |= (0x80 >> i);
        }
        MyI2C_W_SCL(GPIO_Pin_SCL, 1); // 拉低SCL,从机在SCL低电平期间写入SDA
    }
    return ReceiveByte;
}

/**
 * @description: I2C发送应答位ACK
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @param {uint8_t} AckBit
 * @return: {*}
 */
void MyI2C_SendAck(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA, uint8_t AckBit)
{
    MyI2C_W_SDA(GPIO_Pin_SDA, AckBit); // 主机把应答位数据放到SDA线
    MyI2C_W_SCL(GPIO_Pin_SCL, 1);      // 释放SCL,从机在SCL高电平期间,读取应答位
    MyI2C_W_SCL(GPIO_Pin_SCL, 0);      // 拉低SCL,开始下一个时序模块
    return;
}

/**
 * @description: I2C接收应答位ACK
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @return: {*}
 */
uint8_t MyI2C_ReceiveAck(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA)
{
    uint8_t ReceiveACK = 0x00;              // 定义应答位变量
    MyI2C_W_SDA(GPIO_Pin_SDA, 1);           // 接收前，主机先确保释放SDA，避免干扰从机的数据发送
    MyI2C_W_SCL(GPIO_Pin_SCL, 1);           // 释放SCL，主机机在SCL高电平期间读取SDA
    ReceiveACK = MyI2C_R_SDA(GPIO_Pin_SDA); // 将应答位存储到变量里
    MyI2C_W_SCL(GPIO_Pin_SCL, 1);           // 拉低SCL，开始下一个时序模块
    return ReceiveACK;                      // 返回定义应答位变量
}