#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_TxPacket[4]; // 定义发送数据包数组 数据包格式(FF 数据包 FE)
uint8_t Serial_RxPacket[4]; // 定义接收数据包数组
uint8_t Serial_RxFlag;      // 定义接收数据包标志位

/**
 * @description: 串口初始化
 * @param {uint16_t} GPIO_Pin_IN
 * @param {uint16_t} GPIO_Pin_OUT
 * @return: {*}
 */
void Serial_Init(uint16_t GPIO_Pin_IN, uint16_t GPIO_Pin_OUT)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // USART1

    // GPIO初始化
    // 数据发送&串口接收,只初始化TX&RX引脚
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_OUT;       // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // GPIO模式,复用推挽输出模式
    GPIO_Init(GPIOA, &gpioStructure);

    gpioStructure.GPIO_Pin = GPIO_Pin_IN;        // GPIO引脚,PIN 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_IPU;     // GPIO模式,上拉输入模式
    GPIO_Init(GPIOA, &gpioStructure);

    // USART初始化
    USART_InitTypeDef usartInitStructure;
    usartInitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // 模式: 发送模式&接收模式
    usartInitStructure.USART_BaudRate = 9600;                                      // 波特率
    usartInitStructure.USART_WordLength = USART_WordLength_8b;                     // 字长: 选择8位
    usartInitStructure.USART_StopBits = USART_StopBits_1;                          // 停止位: 选择1位
    usartInitStructure.USART_Parity = USART_Parity_No;                             // 校验位: 无校验位
    usartInitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制: 不需要
    USART_Init(USART1, &usartInitStructure);

    // 串口接收可使用查询与接收两种方式
    // 若使用中断需要,需配置NVIC
    // 开启更新中断,开启USART接收数据的中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // NVIC配置中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // NVIC初始化
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = USART1_IRQn;         // 选择配置NVIC的TIM3线
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 1; // 指定NVIC线路的抢占优先级为1
    nvicStructure.NVIC_IRQChannelSubPriority = 1;        // 指定NVIC线路的响应优先级为1
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;           // 指定NVIC线路使能
    NVIC_Init(&nvicStructure);

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

/**
 * @description: USART发送数据包
 * @return: {*}
 * 数据包发送内容: 包头(FF) + Serial_TxPacket数组 + 包尾(FE)
 */
void Serial_SendPacket(void)
{
    Serial_SendByte(0xFF);
    Serial_SendArray(Serial_TxPacket, 4);
    Serial_SendByte(0xFE);
}

/**
 * @description: USART获取接受数据包标志位
 * @return: {*}
 * 接收到数据包后:标志位置1,
 * 读取后:标志位自动清0
 */
uint8_t Serial_GetRxFlag(void)
{
    // 如果标志位为1,则返回1,并自动清零标志位
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    // 如果标志位为0,则返回0
    return 0;
}

void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;   // 当前状态机状态(静态变量)
    static uint8_t pRxPacket = 0; // 当前接收数据位置(静态变量)
    // 判断是否是USART1的接收事件触发的中断
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        // 读取数据寄存器,存放在接收的数据变量
        uint8_t RxData = USART_ReceiveData(USART1);
        // 状态0:接收数据包包头
        if (RxState == 0)
        {
            // 如果数据确实是包头:
            // 置下一个状态(1),数据包的位置归零
            if (RxData == 0xFF)
            {
                RxState = 1;
                pRxPacket = 0;
            }
        }
        // 状态1:接收数据包数据
        else if (RxState == 1)
        {
            // 将数据存入数据包数组的指定位置,数据包的位置自增
            Serial_RxPacket[pRxPacket] = RxData;
            pRxPacket++;
            // 如果收够4个数据,置下一个状态(2)
            if (pRxPacket >= 4)
            {
                RxState = 2;
            }
        }
        // 状态2:接收数据包包尾
        else if (RxState == 2)
        {
            // 如果数据确实是包尾:
            // 置下一个状态(0),接收数据包标志位置1,成功接收一个数据包
            if (RxData == 0xFE)
            {
                RxState = 0;
                Serial_RxFlag = 1;
            }
        }
        // 清除标志位
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
