/**
  ******************************************************************************
  * @file    i2c_simulation.h
  * @author  LAB201 Huifeng
  * @version V1.0.0
  * @date    2017-04-23
  * @brief   i2c_soft module
  *          annotation
  ******************************************************************************
  * 引脚定义需要更具情况调整，
  * DELAY_TIME 延时时间可根据具体情况调整
  * 引脚高低电平操作需要根据情况调整，默认为STM32_HAL操作方式
  *
  ******************************************************************************
  */

#ifndef __I2C_SIMULATION_H
#define __I2C_SIMULATION_H

#include "stm32f1xx_hal.h"

#define SCK_Pin GPIO_PIN_8
#define SCK_GPIO_Port GPIOE
#define SDA_Pin GPIO_PIN_9
#define SDA_GPIO_Port GPIOE

#define DELAY_TIME         5

#define SDA_H()     (SDA_GPIO_Port->BSRR = SDA_Pin)
#define SDA_L()     (SDA_GPIO_Port->BSRR = (uint32_t)SDA_Pin << 16)
#define SCK_H()     (SCK_GPIO_Port->BSRR = SCK_Pin)
#define SCK_L()     (SCK_GPIO_Port->BSRR = (uint32_t)SCK_Pin << 16)
#define SDA_READ()  ((SDA_GPIO_Port->IDR & SDA_Pin) >> 7)


void I2C_Soft_Init();
void I2C_Soft_DeInit();
uint8_t I2C_Soft_Master_Transmit(uint8_t devAddress, uint8_t *pData, uint16_t size);
uint8_t I2C_Soft_Master_Receive(uint8_t devAddress, uint8_t *pData, uint16_t size);

#endif /* ifndef __I2C_SIMULATION_H */
