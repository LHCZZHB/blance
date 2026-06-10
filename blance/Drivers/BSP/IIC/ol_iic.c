#include "ol_iic.h"
#include "../../SYSTEM/delay/delay.h"


void ol_iic_init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   
   OSCL_IIC_CLK_ENABLE();
   
   GPIO_InitStruct.Pin = OSCL_IIC_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(OSCL_IIC_PORT, &GPIO_InitStruct);
   
   GPIO_InitStruct.Pin = OSDA_IIC_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(OSCL_IIC_PORT, &GPIO_InitStruct); 

   HAL_GPIO_WritePin(OSCL_IIC_PORT, OSCL_IIC_PIN, GPIO_PIN_SET);
   HAL_GPIO_WritePin(OSDA_IIC_PORT, OSDA_IIC_PIN, GPIO_PIN_SET);
}

void ol_iic_delay(void)
{
    delay_us(5);
}

void ol_iic_start(void)
{
    OSCL_H();
    OSDA_H();
    ol_iic_delay();
    
    OSDA_L();
    ol_iic_delay();
    
    OSCL_L();
    ol_iic_delay();
}

void ol_iic_stop(void)
{
    OSCL_L();
    ol_iic_delay();
    
    OSDA_L();
    ol_iic_delay();
    
    OSCL_H();
    ol_iic_delay();
    
    OSDA_H();
    ol_iic_delay();
}

void ol_iic_send_ack(void)
{
    OSCL_L();
    ol_iic_delay();
    
    OSDA_L();
    ol_iic_delay();
    
    OSCL_H();
    ol_iic_delay();

    OSCL_L();
    ol_iic_delay();
    
    OSDA_H();
    ol_iic_delay();
}

void ol_iic_send_nack(void)
{
    OSCL_L();
    ol_iic_delay();
    
    OSDA_H();
    ol_iic_delay();
    
    OSCL_H();
    ol_iic_delay();

    OSCL_L();
    ol_iic_delay();
}

uint8_t ol_iic_wait_ack(void)
{
    OSDA_H();
    ol_iic_delay();
    
    OSCL_L();
    ol_iic_delay();
    
    OSCL_H();
    ol_iic_delay();

    if(OSDA_READ())
    {
        ol_iic_stop();
        
        return 1;
    }
    
    OSCL_L();
    ol_iic_delay();
    
    return 0;
}

void ol_iic_send_byte(uint8_t data)
{
    OSCL_L();
    ol_iic_delay();
    
    for(uint8_t i = 0; i < 8; i++)
    {
        if((data & 0x80) >> 7)
        {
            OSDA_H();
        }
        else
        {
            OSDA_L();
        }
        ol_iic_delay();
        
        OSCL_H();
        ol_iic_delay();
        
        OSCL_L();
        ol_iic_delay();
        
        data <<= 1;
    }
    
    OSDA_H();
}

uint8_t ol_iic_rec_byte(uint8_t ack)
{
    uint8_t receive = 0;
    
    for(uint8_t i = 0; i < 8; i++)
    {
        OSCL_L();
        ol_iic_delay();
        
        OSCL_H();
        ol_iic_delay();
        
        receive <<= 1;
        
        if(OSDA_READ())
        {
            receive |= 0x01;
        }
        
        OSCL_L();
    }
    
    if(ack)
    {
        ol_iic_send_ack();
    }
    else
    {
        ol_iic_send_nack();
    }
    
    return receive;
}
