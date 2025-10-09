#include "stm32f10x.h" // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0 // MPU6050的I2C从机地址

/**
 * @description: MPU6050写寄存器
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @param {uint8_t} RegAddress
 * @param {uint8_t} Data
 * @return: {*}
 */
void MPU6050_WriteReg(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA, uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start(GPIO_Pin_SCL, GPIO_Pin_SDA);                     // I2C起始
    MyI2C_SendByte(GPIO_Pin_SCL, GPIO_Pin_SDA, MPU6050_ADDRESS); // 发送从机地址，读写位为0，表示即将写入
    MyI2C_ReceiveAck(GPIO_Pin_SCL, GPIO_Pin_SDA);                // 接收应答
    MyI2C_SendByte(GPIO_Pin_SCL, GPIO_Pin_SDA, RegAddress);      // 发送寄存器地址
    MyI2C_ReceiveAck(GPIO_Pin_SCL, GPIO_Pin_SDA);                // 接收应答
    MyI2C_SendByte(GPIO_Pin_SCL, GPIO_Pin_SDA, Data);            // 发送要写入寄存器的数据
    MyI2C_ReceiveAck(GPIO_Pin_SCL, GPIO_Pin_SDA);                // 接收应答
    MyI2C_Stop(GPIO_Pin_SCL, GPIO_Pin_SDA);                      // I2C终止
    return;
}

/**
 * @description: MPU6050读寄存器
 * @param {uint16_t} GPIO_Pin_SCL
 * @param {uint16_t} GPIO_Pin_SDA
 * @param {uint8_t} RegAddress
 * @return: {*}
 */
uint8_t MPU6050_ReadReg(uint16_t GPIO_Pin_SCL, uint16_t GPIO_Pin_SDA, uint8_t RegAddress)
{
    uint8_t Data;
    MyI2C_Start(GPIO_Pin_SCL, GPIO_Pin_SDA);                     // I2C起始
    MyI2C_SendByte(GPIO_Pin_SCL, GPIO_Pin_SDA, MPU6050_ADDRESS); // 发送从机地址，读写位为0，表示即将写入
    MyI2C_ReceiveAck(GPIO_Pin_SCL, GPIO_Pin_SDA);                // 接收应答
    MyI2C_SendByte(GPIO_Pin_SCL, GPIO_Pin_SDA, RegAddress);      // 发送寄存器地址
    MyI2C_ReceiveAck(GPIO_Pin_SCL, GPIO_Pin_SDA);                // 接收应答

    MyI2C_Start(GPIO_Pin_SCL, GPIO_Pin_SDA);                            // I2C重复起始
    MyI2C_SendByte(GPIO_Pin_SCL, GPIO_Pin_SDA, MPU6050_ADDRESS | 0x01); // 发送从机地址，读写位为1，表示即将读取
    MyI2C_ReceiveAck(GPIO_Pin_SCL, GPIO_Pin_SDA);                       // 接收应答
    Data = MyI2C_ReceiveByte(GPIO_Pin_SCL, GPIO_Pin_SDA);               // 接收指定寄存器的数据
    MyI2C_SendAck(GPIO_Pin_SCL, GPIO_Pin_SDA, 1);                       // 发送应答，给从机非应答，终止从机的数据输出
    MyI2C_Stop(GPIO_Pin_SCL, GPIO_Pin_SDA);                             // I2C终止
    return Data;
}

/**
 * @description: MPU6050初始化
 * @return: {*}
 */
void MPU6050_Init(void)
{
    MyI2C_Init(GPIO_Pin_10 | GPIO_Pin_11); // 初始化底层的I2C模块

    // MPU6050寄存器初始化,需要对照MPU6050手册的寄存器描述配置,此处仅配置了部分重要的寄存器
    MPU6050_WriteReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_PWR_MGMT_1, 0x01);   // 电源管理寄存器1,取消睡眠模式,选择时钟源为X轴陀螺仪
    MPU6050_WriteReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_PWR_MGMT_2, 0x00);   // 电源管理寄存器2,保持默认值0,所有轴均不待机
    MPU6050_WriteReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_SMPLRT_DIV, 0x09);   // 采样率分频寄存器,配置采样率
    MPU6050_WriteReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_CONFIG, 0x06);       // 配置寄存器,配置DLPF
    MPU6050_WriteReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_GYRO_CONFIG, 0x18);  // 陀螺仪配置寄存器,选择满量程为±2000°/s
    MPU6050_WriteReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_ACCEL_CONFIG, 0x18); // 加速度计配置寄存器,选择满量程为±16g
    return;
}

/**
 * @description: 
 * @return: {*}
 */
uint8_t MPU6050_GetID(void)
{
    // 返回WHO_AM_I寄存器的值
    return MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_WHO_AM_I);
}

/**
 * @description: MPU6050获取当前数据
 * @return: {*}
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL; // 定义数据高8位和低8位的变量

    DataH = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_ACCEL_XOUT_H); // 读取加速度计X轴的高8位数据
    DataL = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_ACCEL_XOUT_L); // 读取加速度计X轴的低8位数据
    *AccX = (DataH << 8) | DataL;                                            // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_ACCEL_YOUT_H); // 读取加速度计Y轴的高8位数据
    DataL = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_ACCEL_YOUT_L); // 读取加速度计Y轴的低8位数据
    *AccY = (DataH << 8) | DataL;                                            // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_ACCEL_ZOUT_H); // 读取加速度计Z轴的高8位数据
    DataL = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_ACCEL_ZOUT_L); // 读取加速度计Z轴的低8位数据
    *AccZ = (DataH << 8) | DataL;                                            // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_GYRO_XOUT_H); // 读取陀螺仪X轴的高8位数据
    DataL = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_GYRO_XOUT_L); // 读取陀螺仪X轴的低8位数据
    *GyroX = (DataH << 8) | DataL;                                          // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_GYRO_YOUT_H); // 读取陀螺仪Y轴的高8位数据
    DataL = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_GYRO_YOUT_L); // 读取陀螺仪Y轴的低8位数据
    *GyroY = (DataH << 8) | DataL;                                          // 数据拼接，通过输出参数返回

    DataH = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_GYRO_ZOUT_H); // 读取陀螺仪Z轴的高8位数据
    DataL = MPU6050_ReadReg(GPIO_Pin_10, GPIO_Pin_11, MPU6050_GYRO_ZOUT_L); // 读取陀螺仪Z轴的低8位数据
    *GyroZ = (DataH << 8) | DataL;                                          // 数据拼接，通过输出参数返回
}