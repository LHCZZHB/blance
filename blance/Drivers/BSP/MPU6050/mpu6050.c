#include "mpu6050.h"
#include "../IIC/mpu_iic.h"
#include "../../SYSTEM/delay/delay.h"
#include "../../SYSTEM/usart/usart.h"


//MPU6050指定地址读取一个字节数据
uint8_t mpu6050_receivebyte(uint8_t regaddress)
{
    uint8_t data;

    mpu_iic_start();
    mpu_iic_send_byte(Device_Write_Addr_mpu); // 发送 MPU6050 写地址
    if(mpu_iic_wait_ack() == 1)
    {
        mpu_iic_stop();
        return 0x01;
    }
    
    mpu_iic_send_byte(regaddress); // 发送寄存器地址
    if(mpu_iic_wait_ack() == 1)
    {
        mpu_iic_stop();
        return 0x01;
    }

    mpu_iic_start(); // 重新开始
    mpu_iic_send_byte(Device_Read_Addr_mpu); // 发送 MPU6050 读地址
    if(mpu_iic_wait_ack() == 1)
    {
        mpu_iic_stop();
        return 0x01;
    }
    
    data = mpu_iic_rec_byte(0); // 读取数据后发送 NACK
    mpu_iic_stop(); // 停止 I2C 通信

    return data;
}

//MPU6050写一个字节数据
uint8_t mpu6050_sendbyte(uint8_t regaddress, uint8_t data)
{
    mpu_iic_start();
    mpu_iic_send_byte(Device_Write_Addr_mpu); // 发送 MPU6050 写地址
    if(mpu_iic_wait_ack() == 1)
    {
        mpu_iic_stop();
        return 0x02;
    }
    
    mpu_iic_send_byte(regaddress); // 发送寄存器地址
    if(mpu_iic_wait_ack() == 1)
    {
        mpu_iic_stop();
        return 0x02;
    }

    mpu_iic_send_byte(data); // 发送数据
    if(mpu_iic_wait_ack() == 1)
    {
        mpu_iic_stop();
        return 0x02;
    }
    
    mpu_iic_stop(); // 停止 I2C 通信
    
    return 0;
}

//MPU6050连续读取多个字节数据
uint8_t mpu6050_receivemultibytes(uint8_t regaddress, uint8_t *data, uint8_t length)
{
    mpu_iic_start();
    mpu_iic_send_byte(Device_Write_Addr_mpu); // 发送 MPU6050 写地址
    if(mpu_iic_wait_ack())
    {
        mpu_iic_stop();
        return 1;
    }
    mpu_iic_send_byte(regaddress); // 发送寄存器地址
    if(mpu_iic_wait_ack())
    {
        mpu_iic_stop();
        return 1;
    }

    mpu_iic_start(); // 重新开始
    mpu_iic_send_byte(Device_Read_Addr_mpu); // 发送 MPU6050 读地址
    if(mpu_iic_wait_ack())
    {
        mpu_iic_stop();
        return 1;
    }

    for(uint8_t i = 0; i < length; i++)
    {
        data[i] = mpu_iic_rec_byte(i < length - 1); // 读取数据，最后一个字节发送 NACK
    }
    
    mpu_iic_stop(); // 停止 I2C 通信
    return 0; // 成功读取
}

//MPU6050连续写入多个字节数据
uint8_t mpu6050_sendmultibytes(uint8_t dev_adder, uint8_t regaddress, uint8_t *data, uint8_t length)
{
    //dev_adder参数为了适配DMP库的函数接口，不调用
    mpu_iic_start();
    mpu_iic_send_byte(Device_Write_Addr_mpu); // 发送 MPU6050 写地址
    if(mpu_iic_wait_ack())
    {
        mpu_iic_stop();
        return 1;
    }
    mpu_iic_send_byte(regaddress); // 发送寄存器地址
    if(mpu_iic_wait_ack())
    {
        mpu_iic_stop();
        return 1;
    }

    for(uint8_t i = 0; i < length; i++)
    {
        mpu_iic_send_byte(data[i]); // 发送数据
        if(mpu_iic_wait_ack())
        {
            mpu_iic_stop();
            return 1;
        }
    }
    
    mpu_iic_stop(); // 停止 I2C 通信
    return 0; // 成功写入
}

//MPU6050初始化
void mpu6050_init(void)
{
     mpu_iic_init(); // 初始化 I2C 接口
    
     // -----------------------------------------------------------------
     // 步骤 1：读取 WHO_AM_I 寄存器 (0x75)，验证器件身份
     // -----------------------------------------------------------------
     uint8_t chip_id = mpu6050_receivebyte(MPU6050_WHO_AM_I);
     if(chip_id == 0x68)
     {
         printf("MPU6050 initialization successful!\r\n");
     }
     else
     {
         printf("MPU6050 initialization failed! Read Chip ID: 0x%02X\r\n", chip_id);
     }

     // -----------------------------------------------------------------
     // 步骤 2：复位芯片并解除睡眠状态，选择时钟源
     // 寄存器：PWR_MGMT_1 (0x6B)
     // 写入值：0x01
     // 含义：BIT6(SLEEP)=0 唤醒芯片；BIT2:0(CLKSEL)=001 选择X轴陀螺仪作为时钟源（比内部RC更稳定）
     // -----------------------------------------------------------------     
     mpu6050_sendbyte(MPU6050_PWR_MGMT_1, 0x01); 

     // -----------------------------------------------------------------
     // 步骤 3：配置数字低通滤波器 (DLPF)
     // 寄存器：CONFIG (0x1A)
     // 写入值：0x03
     // 含义：设置低通滤波器截止频率为 42Hz。能极其有效地滤除电机震动或手抖噪声，使输出数据更平滑
     // -----------------------------------------------------------------
     mpu6050_sendbyte(MPU6050_CONFIG, 0x03); 

     // -----------------------------------------------------------------
     // 步骤 4：配置采样率分频器 (Sample Rate Divider)
     // 寄存器：SMPLRT_DIV (0x19)
     // 写入值：0x04
     // 核心公式：采样率 = 陀螺仪输出率 / (1 + SMPLRT_DIV)
     // 说明：因为上一步开启了 DLPF，陀螺仪输出率为 1kHz。
     //       写入 0x04 得到：1000 / (1 + 4) = 200Hz。即 MPU6050 每秒更新 200 次数据(每 5ms 一次)
     // -----------------------------------------------------------------
     mpu6050_sendbyte(MPU6050_SMPLRT_DIV, 0x04);

     // -----------------------------------------------------------------
     // 步骤 5：配置陀螺仪满量程范围
     // 寄存器：GYRO_CONFIG (0x1B)
     // 写入值：0x18
     // 含义：BIT4:3(FS_SEL)=11，选择最大量程 ±2000 °/s (dps)
     // 注意：此时代码中读取到的原始 16 位数据，需要除以灵敏度系数 16.4 才能转换成真正的角速度。
     // -----------------------------------------------------------------
     mpu6050_sendbyte(MPU6050_GYRO_CONFIG, 0x18); 

     // -----------------------------------------------------------------
     // 步骤 6：配置加速度计满量程范围
     // 寄存器：ACCEL_CONFIG (0x1C)
     // 写入值：0x00
     // 含义：BIT4:3(AFS_SEL)=00，选择最小量程 ±2g (重力加速度)
     // 注意：量程越小精度越高。此时灵敏度系数为 16384 LSB/g。原始数据除以 16384 即可得到 g 速度。
     // -----------------------------------------------------------------
     mpu6050_sendbyte(MPU6050_ACCEL_CONFIG, 0x00); 

}

//MPU6050获取加速度和陀螺仪数据
void mpu6050_getdata(int16_t *accel_data, int16_t *gyro_data)
{
     uint8_t buffer[14];

     if(mpu6050_receivemultibytes(MPU6050_ACCEL_XOUT_H, buffer, 14) == 0)
     {
        accel_data[0] = (int16_t)((buffer[0] << 8) | buffer[1]);    // 加速度 X
        accel_data[1] = (int16_t)((buffer[2] << 8) | buffer[3]);    // 加速度 Y
        accel_data[2] = (int16_t)((buffer[4] << 8) | buffer[5]);    // 加速度 Z
        gyro_data[0] = (int16_t)((buffer[8] << 8) | buffer[9]);	    //陀螺仪 X
        gyro_data[1] = (int16_t)((buffer[10] << 8) | buffer[11]);   //陀螺仪 Y
        gyro_data[2] = (int16_t)((buffer[12] << 8) | buffer[13]);   //陀螺仪 Z
     }
}
