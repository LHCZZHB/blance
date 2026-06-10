#ifndef __OL_IIC_H
#define __OL_IIC_H

#include "../../SYSTEM/sys/sys.h"


/*Òý½Å¶¨Òå*/
#define     OSCL_IIC_PORT           GPIOB
#define     OSCL_IIC_PIN            GPIO_PIN_8
#define     OSCL_IIC_CLK_ENABLE()   do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)

#define     OSDA_IIC_PORT           GPIOB
#define     OSDA_IIC_PIN            GPIO_PIN_9
#define     OSDA_IIC_CLK_ENABLE()   do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)

#define     OSCL_H()                HAL_GPIO_WritePin(OSCL_IIC_PORT, OSCL_IIC_PIN, GPIO_PIN_SET)
#define     OSCL_L()                HAL_GPIO_WritePin(OSCL_IIC_PORT, OSCL_IIC_PIN, GPIO_PIN_RESET)

#define     OSDA_H()                HAL_GPIO_WritePin(OSDA_IIC_PORT, OSDA_IIC_PIN, GPIO_PIN_SET)
#define     OSDA_L()                HAL_GPIO_WritePin(OSDA_IIC_PORT, OSDA_IIC_PIN, GPIO_PIN_RESET)

#define     OSDA_READ()             HAL_GPIO_ReadPin(OSDA_IIC_PORT, OSDA_IIC_PIN) 

void ol_iic_init(void);
void ol_iic_start(void);
void ol_iic_stop(void);
void ol_iic_send_ack(void);
void ol_iic_send_nack(void);
uint8_t ol_iic_wait_ack(void);
void ol_iic_send_byte(uint8_t data);
uint8_t ol_iic_rec_byte(uint8_t ack);
#endif
