#ifndef __MPU_IIC_H
#define __MPU_IIC_H

#include "../../SYSTEM/sys/sys.h"


/*Òý½Å¶¨Òå*/
#define     MSCL_IIC_PORT           GPIOB
#define     MSCL_IIC_PIN            GPIO_PIN_10
#define     MSCL_IIC_CLK_ENABLE()   do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)

#define     MSDA_IIC_PORT           GPIOB
#define     MSDA_IIC_PIN            GPIO_PIN_11
#define     MSDA_IIC_CLK_ENABLE()   do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)

#define     MSCL_H()                HAL_GPIO_WritePin(MSCL_IIC_PORT, MSCL_IIC_PIN, GPIO_PIN_SET)
#define     MSCL_L()                HAL_GPIO_WritePin(MSCL_IIC_PORT, MSCL_IIC_PIN, GPIO_PIN_RESET)

#define     MSDA_H()                HAL_GPIO_WritePin(MSDA_IIC_PORT, MSDA_IIC_PIN, GPIO_PIN_SET)
#define     MSDA_L()                HAL_GPIO_WritePin(MSDA_IIC_PORT, MSDA_IIC_PIN, GPIO_PIN_RESET)

#define     MSDA_READ()             HAL_GPIO_ReadPin(MSDA_IIC_PORT, MSDA_IIC_PIN) 


void mpu_iic_init(void);
void mpu_iic_start(void);
void mpu_iic_stop(void);
void mpu_iic_send_ack(void);
void mpu_iic_send_nack(void);
uint8_t mpu_iic_wait_ack(void);
void mpu_iic_send_byte(uint8_t data);
uint8_t mpu_iic_rec_byte(uint8_t ack);
#endif
