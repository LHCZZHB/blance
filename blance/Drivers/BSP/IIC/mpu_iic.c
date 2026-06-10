#include "mpu_iic.h"
#include "../../SYSTEM/delay/delay.h"


void mpu_iic_init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   
   MSCL_IIC_CLK_ENABLE();
   
   GPIO_InitStruct.Pin = MSCL_IIC_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(MSCL_IIC_PORT, &GPIO_InitStruct);
   
   GPIO_InitStruct.Pin = MSDA_IIC_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(MSCL_IIC_PORT, &GPIO_InitStruct); 

   HAL_GPIO_WritePin(MSCL_IIC_PORT, MSCL_IIC_PIN, GPIO_PIN_SET);
   HAL_GPIO_WritePin(MSDA_IIC_PORT, MSDA_IIC_PIN, GPIO_PIN_SET);
}

void mpu_iic_delay(void)
{
    delay_us(5);
}

void mpu_iic_start(void)
{
    MSCL_H();
    MSDA_H();
    mpu_iic_delay();
    
    MSDA_L();
    mpu_iic_delay();
    
    MSCL_L();
    mpu_iic_delay();
}

void mpu_iic_stop(void)
{
    MSCL_L();
    mpu_iic_delay();
    
    MSDA_L();
    mpu_iic_delay();
    
    MSCL_H();
    mpu_iic_delay();
    
    MSDA_H();
    mpu_iic_delay();
}

void mpu_iic_send_ack(void)
{
    MSCL_L();
    mpu_iic_delay();
    
    MSDA_L();
    mpu_iic_delay();
    
    MSCL_H();
    mpu_iic_delay();

    MSCL_L();
    mpu_iic_delay();
    
    MSDA_H();
    mpu_iic_delay();
}

void mpu_iic_send_nack(void)
{
    MSCL_L();
    mpu_iic_delay();
    
    MSDA_H();
    mpu_iic_delay();
    
    MSCL_H();
    mpu_iic_delay();

    MSCL_L();
    mpu_iic_delay();
}

uint8_t mpu_iic_wait_ack(void)
{
    MSDA_H();
    mpu_iic_delay();
    
    MSCL_L();
    mpu_iic_delay();
    
    MSCL_H();
    mpu_iic_delay();

    if(MSDA_READ())
    {
        mpu_iic_stop();
        
        return 1;
    }
    
    MSCL_L();
    mpu_iic_delay();
    
    return 0;
}

void mpu_iic_send_byte(uint8_t data)
{
    MSCL_L();
    mpu_iic_delay();
    
    for(uint8_t i = 0; i < 8; i++)
    {
        if((data & 0x80) >> 7)
        {
            MSDA_H();
        }
        else
        {
            MSDA_L();
        }
        mpu_iic_delay();
        
        MSCL_H();
        mpu_iic_delay();
        
        MSCL_L();
        mpu_iic_delay();
        
        data <<= 1;
    }
    
    MSDA_H();
}

uint8_t mpu_iic_rec_byte(uint8_t ack)
{
    uint8_t receive = 0;
    
    for(uint8_t i = 0; i < 8; i++)
    {
        MSCL_L();
        mpu_iic_delay();
        
        MSCL_H();
        mpu_iic_delay();
        
        receive <<= 1;
        
        if(MSDA_READ())
        {
            receive |= 0x01;
        }
        
        MSCL_L();
    }
    
    if(ack)
    {
        mpu_iic_send_ack();
    }
    else
    {
        mpu_iic_send_nack();
    }
    
    return receive;
}
