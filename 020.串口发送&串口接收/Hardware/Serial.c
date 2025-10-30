#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData; // 串口接收的数据变量
uint8_t Serial_RxFlag; // 串口接收的标志位变量

/**
 * @description: 串口初始化
 * @return: {*}
 */
void Serial_Init(uint16_t GPIO_Pin_IN, uint16_t GPIO_Pin_OUT)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA 

    // GPIO初始化
    // 数据发送&串口接收,只初始化TX&RX引脚
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_OUT;       // GPIO引脚,PIN_OUT 引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // GPIO模式,复用推挽输出模式
    GPIO_Init(GPIOA, &gpioStructure);

    gpioStructure.GPIO_Pin = GPIO_Pin_IN;        // GPIO引脚,PIN_IN 引脚
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
    // 开启更新中断// 开启USART接收数据的中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 

    // NVIC配置中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // NVIC初始化
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = USART1_IRQn;         // 选择配置NVIC的TIM3线
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 2; // 指定NVIC线路的抢占优先级为2
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
 * @description: USART接收数据 获取标志位
 * @return: {*}
 */
uint8_t Serial_GetRxFlag(void)
{
    // 若标志位为1,则返回1,并自动清零标志位
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    // 若标志位为0，则返回0
    return 0;
}

/**
 * @description: USART接收数据 获取数据
 * @return: {*}
 */
uint8_t Serial_GetRxData(void)
{
    // 返回接收的数据
    return Serial_RxData;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        Serial_RxData = USART_ReceiveData(USART1); // 读取数据寄存器,存放在接收的数据变量
        Serial_RxFlag = 1;                         // 置接收标志位变量为1
        // 清除USART1的RXNE标志位,读取数据寄存器会自动清除此标志位
        // 如果已读取数据寄存器,也可以不执行此代码
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}


// uint8_t Serial_RxData;		//定义串口接收的数据变量
// uint8_t Serial_RxFlag;		//定义串口接收的标志位变量

// /**
//   * 函    数：串口初始化
//   * 参    数：无
//   * 返 回 值：无
//   */
// void Serial_Init(void)
// {
// 	/*开启时钟*/
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启USART1的时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	
// 	/*GPIO初始化*/
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA9引脚初始化为复用推挽输出
	
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA10引脚初始化为上拉输入
	
// 	/*USART初始化*/
// 	USART_InitTypeDef USART_InitStructure;					//定义结构体变量
// 	USART_InitStructure.USART_BaudRate = 9600;				//波特率
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
// 	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
// 	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
// 	USART_Init(USART1, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
	
// 	/*中断输出配置*/
// 	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
	
// 	/*NVIC中断分组*/
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
	
// 	/*NVIC配置*/
// 	NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
// 	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//选择配置NVIC的USART1线
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
// 	NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
	
// 	/*USART使能*/
// 	USART_Cmd(USART1, ENABLE);								//使能USART1，串口开始运行
// }

// /**
//   * 函    数：串口发送一个字节
//   * 参    数：Byte 要发送的一个字节
//   * 返 回 值：无
//   */
// void Serial_SendByte(uint8_t Byte)
// {
// 	USART_SendData(USART1, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
// 	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
// 	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
// }

// /**
//   * 函    数：串口发送一个数组
//   * 参    数：Array 要发送数组的首地址
//   * 参    数：Length 要发送数组的长度
//   * 返 回 值：无
//   */
// void Serial_SendArray(uint8_t *Array, uint16_t Length)
// {
// 	uint16_t i;
// 	for (i = 0; i < Length; i ++)		//遍历数组
// 	{
// 		Serial_SendByte(Array[i]);		//依次调用Serial_SendByte发送每个字节数据
// 	}
// }

// /**
//   * 函    数：串口发送一个字符串
//   * 参    数：String 要发送字符串的首地址
//   * 返 回 值：无
//   */
// void Serial_SendString(char *String)
// {
// 	uint8_t i;
// 	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
// 	{
// 		Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
// 	}
// }

// /**
//   * 函    数：次方函数（内部使用）
//   * 返 回 值：返回值等于X的Y次方
//   */
// uint32_t Serial_Pow(uint32_t X, uint32_t Y)
// {
// 	uint32_t Result = 1;	//设置结果初值为1
// 	while (Y --)			//执行Y次
// 	{
// 		Result *= X;		//将X累乘到结果
// 	}
// 	return Result;
// }

// /**
//   * 函    数：串口发送数字
//   * 参    数：Number 要发送的数字，范围：0~4294967295
//   * 参    数：Length 要发送数字的长度，范围：0~10
//   * 返 回 值：无
//   */
// void Serial_SendNumber(uint32_t Number, uint8_t Length)
// {
// 	uint8_t i;
// 	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
// 	{
// 		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
// 	}
// }

// /**
//   * 函    数：使用printf需要重定向的底层函数
//   * 参    数：保持原始格式即可，无需变动
//   * 返 回 值：保持原始格式即可，无需变动
//   */
// int fputc(int ch, FILE *f)
// {
// 	Serial_SendByte(ch);			//将printf的底层重定向到自己的发送字节函数
// 	return ch;
// }

// /**
//   * 函    数：自己封装的prinf函数
//   * 参    数：format 格式化字符串
//   * 参    数：... 可变的参数列表
//   * 返 回 值：无
//   */
// void Serial_Printf(char *format, ...)
// {
// 	char String[100];				//定义字符数组
// 	va_list arg;					//定义可变参数列表数据类型的变量arg
// 	va_start(arg, format);			//从format开始，接收参数列表到arg变量
// 	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
// 	va_end(arg);					//结束变量arg
// 	Serial_SendString(String);		//串口发送字符数组（字符串）
// }

// /**
//   * 函    数：获取串口接收标志位
//   * 参    数：无
//   * 返 回 值：串口接收标志位，范围：0~1，接收到数据后，标志位置1，读取后标志位自动清零
//   */
// uint8_t Serial_GetRxFlag(void)
// {
// 	if (Serial_RxFlag == 1)			//如果标志位为1
// 	{
// 		Serial_RxFlag = 0;
// 		return 1;					//则返回1，并自动清零标志位
// 	}
// 	return 0;						//如果标志位为0，则返回0
// }

// /**
//   * 函    数：获取串口接收的数据
//   * 参    数：无
//   * 返 回 值：接收的数据，范围：0~255
//   */
// uint8_t Serial_GetRxData(void)
// {
// 	return Serial_RxData;			//返回接收的数据变量
// }

// /**
//   * 函    数：USART1中断函数
//   * 参    数：无
//   * 返 回 值：无
//   * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
//   *           函数名为预留的指定名称，可以从启动文件复制
//   *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
//   */
// void USART1_IRQHandler(void)
// {
// 	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
// 	{
// 		Serial_RxData = USART_ReceiveData(USART1);				//读取数据寄存器，存放在接收的数据变量
// 		Serial_RxFlag = 1;										//置接收标志位变量为1
// 		USART_ClearITPendingBit(USART1, USART_IT_RXNE);			//清除USART1的RXNE标志位
// 																//读取数据寄存器会自动清除此标志位
// 																//如果已经读取了数据寄存器，也可以不执行此代码
// 	}
// }
