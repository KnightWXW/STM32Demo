#include "stm32f10x.h"

/**
 * @description: 串口初始化
 * @return: {*}
 */
void Serial_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // USART1

    // GPIO初始化
    // 当前只需要数据发送功能,只初始化TX引脚
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // GPIO模式,复用推挽输出模式
    GPIO_Init(GPIOA, &gpioStructure);

    // USART初始化
    USART_InitTypeDef usartInitStructure;
    usartInitStructure.USART_Mode = USART_Mode_Tx;                                 // 模式: 发送模式
    usartInitStructure.USART_BaudRate = 9600;                                      // 波特率
    usartInitStructure.USART_WordLength = USART_WordLength_8b;                     // 字长: 选择8位
    usartInitStructure.USART_StopBits = USART_StopBits_1;                          // 停止位: 选择1位
    usartInitStructure.USART_Parity = USART_Parity_No;                             // 校验位: 无校验位
    usartInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制: 不需要
    USART_Init(USART1, &usartInitStructure);

    // USART使能
    USART_Cmd(USART1, ENABLE);
}

/**
 * @description: USART发送字节类型数据
 * @param {uint8_t} Byte
 * @return: {*}
 */
void Serial_SendByte(uint8_t Byte)
{
    // 将字节数据写入数据寄存器，写入后USART自动生成时序波形
    USART_SendData(USART1, Byte);
    // 等待发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    // 下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位
}

/**
 * @description: USART发送数组类型数据
 * @param {uint8_t} *Array
 * @param {uint16_t} Length
 * @return: {*}
 */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    for (uint8_t i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }
    return;
}

/**
 * @description: USART发送字符串类型数据
 * @param {char} *String
 * @return: {*}
 */
void Serial_SendString(char *String)
{
    for (uint8_t i = 0; String[i] != '\0'; i++)
    {
        Serial_SendByte(String[i]);
    }
    return;
}

/**
 * @description: 计算乘积函数, 返回 X的Y次方
 * @param {uint32_t} X
 * @param {uint32_t} Y
 * @return: {*}
 */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1; // 设置结果初值为1
    while (Y--)          // 执行Y次
    {
        Result *= X; // 将X累乘到结果
    }
    return Result;
}

/**
 * @description: USART发送数字类型数据
 * @param {uint32_t} Number 发送的数字,范围: 0~4294967295
 * @param {uint8_t} Length 发送数字的长度范围: 0~10
 * @return: {*}
 */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    for (uint8_t i = 0; i < Length; i++) // 根据数字长度遍历数字的每一位
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0'); // 依次调用Serial_SendByte发送每位数字
    }
}

/**
 * @description: 使用USART发送数据
 * @param {int} ch
 * @param {FILE} *f
 * @return: {*}
 */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch); // 将printf的底层重定向到自己的发送字节函数
    return ch;
}

/**
 * @description: 使用vsprintf发送数据
 * @param {char} *format
 * @return: {*}
 */
void Serial_Printf(char *format, ...)
{
    char String[100];              // 定义字符数组
    va_list arg;                   // 定义可变参数列表数据类型的变量arg
    va_start(arg, format);         // 从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg); // 使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                   // 结束变量arg
    Serial_SendString(String);     // 串口发送字符数组（字符串）
}