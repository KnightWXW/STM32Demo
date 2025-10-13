#include "stm32f10x.h" // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0 // MPU6050的I2C从机地址

/**
 * @description: MPU6050等待事件
 * @param {I2C_TypeDef*} I2Cx
 * @param {uint32_t} I2C_EVENT
 * @return: {*}
 */
void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
    uint32_t Timeout = 10000; // 给定超时计数时间
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
    {
        Timeout--;
        // 等待超时,跳出等待
        if (Timeout == 0)
        {
            break;
        }
    }
    return;
}

/**
 * @description: MPU6050写寄存器
 * @param {uint8_t} RegAddress
 * @param {uint8_t} Data
 * @return: {*}
 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); // 硬件I2C发送从机地址，方向为发送
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   // 等待EV6

    I2C_SendData(I2C2, RegAddress);                              // 硬件I2C发送寄存器地址
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 等待EV8

    I2C_SendData(I2C2, Data);                                   // 硬件I2C发送数据
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2

    I2C_GenerateSTOP(I2C2, ENABLE); // 硬件I2C生成终止条件
}

/**
 * @description: MPU6050读寄存器
 * @param {uint8_t} RegAddress
 * @return: {*}
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;

    I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); // 硬件I2C发送从机地址，方向为发送
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   // 等待EV6

    I2C_SendData(I2C2, RegAddress);                             // 硬件I2C发送寄存器地址
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待EV8_2

    I2C_GenerateSTART(I2C2, ENABLE);                       // 硬件I2C生成重复起始条件
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待EV5

    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver); // 硬件I2C发送从机地址，方向为接收
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);   // 等待EV6

    I2C_AcknowledgeConfig(I2C2, DISABLE); // 在接收最后一个字节之前提前将应答失能
    I2C_GenerateSTOP(I2C2, ENABLE);       // 在接收最后一个字节之前提前申请停止条件

    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // 等待EV7
    Data = I2C_ReceiveData(I2C2);                            // 接收数据寄存器

    I2C_AcknowledgeConfig(I2C2, ENABLE); // 将应答恢复为使能，为了不影响后续可能产生的读取多字节操作

    return Data;
}

/**
 * @description: MPU6050初始化
 * @param {uint16_t} GPIO_Pin
 * @return: {*}
 */ 
void MPU6050_Init(uint16_t GPIO_Pin)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // GPIOB
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  // I2C2

    // 初始化 GPIO
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin;           // GPIO引脚,GPIO_Pin引脚
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO速度,50MHz
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // GPIO模式,复用开漏输出模式
    GPIO_Init(GPIOB, &gpioStructure);

    // 初始化I2C
    I2C_InitTypeDef I2CInitStructure;
    I2CInitStructure.I2C_Mode = I2C_Mode_I2C;                                // 模式:选择为I2C模式
    I2CInitStructure.I2C_ClockSpeed = 40000;                                 // 时钟速度:选择为40KHz
    I2CInitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        // 时钟占空比:选择Tlow/Thigh = 2
    I2CInitStructure.I2C_Ack = I2C_Ack_Enable;                               // 应答:选择使能
    I2CInitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 应答地址:选择7位,从机模式下才有效
    I2CInitStructure.I2C_OwnAddress1 = 0x00;                                 // 自身地址:从机模式下才有效
    I2C_Init(I2C2, &I2CInitStructure);

    // I2C使能
    I2C_Cmd(I2C2, ENABLE);

    // MPU6050寄存器初始化,需要对照MPU6050手册的寄存器描述配置,此处仅配置了部分重要的寄存器
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);   // 电源管理寄存器1:取消睡眠模式,选择时钟源为X轴陀螺仪
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);   // 电源管理寄存器2:保持默认值0,所有轴均不待机
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);   // 采样率分频寄存器:配置采样率
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);       // 配置寄存器:配置DLPF
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);  // 陀螺仪配置寄存器:选择满量程为±2000°/s
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计配置寄存器:选择满量程为±16g
}

/**
 * @description:
 * @return: {*}
 */
uint8_t MPU6050_GetID(void)
{
    // 返回WHO_AM_I寄存器的值
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
 * @description: MPU6050获取当前数据
 * @return: {*}
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL; // 定义数据高8位和低8位的变量

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H); // 读取加速度计X轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L); // 读取加速度计X轴的低8位数据
    *AccX = (DataH << 8) | DataL;                  // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H); // 读取加速度计Y轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L); // 读取加速度计Y轴的低8位数据
    *AccY = (DataH << 8) | DataL;                  // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H); // 读取加速度计Z轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L); // 读取加速度计Z轴的低8位数据
    *AccZ = (DataH << 8) | DataL;                  // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H); // 读取陀螺仪X轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L); // 读取陀螺仪X轴的低8位数据
    *GyroX = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H); // 读取陀螺仪Y轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L); // 读取陀螺仪Y轴的低8位数据
    *GyroY = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H); // 读取陀螺仪Z轴的高8位数据
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L); // 读取陀螺仪Z轴的低8位数据
    *GyroZ = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回
}
