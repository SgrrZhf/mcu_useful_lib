/**
  ******************************************************************************
  * @file    i2c_simulation.c
  * @author  LAB201 Huifeng
  * @version V1.0.0
  * @date    2017-04-23
  * @brief   软件模拟I2C总线主机
  *          对外提供初始化与反初始化函数以及发送，接受函数
  ******************************************************************************
  *
  * @I2C_Soft_Init() 需要根据需要修改
  * @I2C_Soft_DeInit() 同上
  * @I2C_Soft_Master_Transmit() 
  * @I2C_Soft_Master_Receive()
  * 
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"
#include "i2c_simulation.h"

/**
  * @brief  I2C总线的初始化，即配置I2C总线占用的两个GPIO口
  * @param  none
  * @retval none
  */
void I2C_Soft_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOE_CLK_ENABLE();

    HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCK_GPIO_Port, SDA_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SCK_Pin | SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/**
  * @brief  I2C总线反初始化，将两个GPIO口反配置
  * @param  none
  * @retval none
  */
void I2C_Soft_DeInit()
{
    HAL_GPIO_DeInit(GPIOE, SCK_Pin | SDA_Pin);
}

static void IIC_Delay(uint32_t n)
{
    while (n--)
    {
        __NOP();
    }
}

static uint8_t I2C_Start()
{
    SDA_H();
    SCK_H();

    IIC_Delay(DELAY_TIME);
    if (!SDA_READ())return (0);
    SDA_L();
    IIC_Delay(DELAY_TIME);
    if (SDA_READ()) return (0);

    SCK_L();
    IIC_Delay(DELAY_TIME);
    return (1);
}

static void I2C_Stop(void)
{
    SCK_L();
    IIC_Delay(DELAY_TIME);
    SDA_L();
    IIC_Delay(DELAY_TIME);
    SCK_H();
    IIC_Delay(DELAY_TIME);
    SDA_H();
    IIC_Delay(DELAY_TIME);
}

static uint8_t I2C_WaitAck(void)
{
    SCK_L();
    IIC_Delay(DELAY_TIME);
    SDA_H();
    IIC_Delay(DELAY_TIME);
    SCK_H();
    IIC_Delay(DELAY_TIME);

    if (SDA_READ())
    {
        SCK_L();
        IIC_Delay(DELAY_TIME);
        return 0;
    }
    SCK_L();
    IIC_Delay(DELAY_TIME);
    return 1;
}

static void I2C_Ack(void)
{
    SCK_L();
    IIC_Delay(DELAY_TIME);
    SDA_L();
    IIC_Delay(DELAY_TIME);
    SCK_H();
    IIC_Delay(DELAY_TIME);
    SCK_L();
    IIC_Delay(DELAY_TIME);
}

static void I2C_NoAck(void)
{
    SCK_L();
    IIC_Delay(DELAY_TIME);
    SDA_H();
    IIC_Delay(DELAY_TIME);
    SCK_H();
    IIC_Delay(DELAY_TIME);
    SCK_L();
    IIC_Delay(DELAY_TIME);
}

static void I2C_SendByte(uint8_t SendByte)
{
    uint8_t i = 8;

    while (i--)
    {
        SCK_L();
        IIC_Delay(DELAY_TIME);
        if (SendByte & 0x80)
            SDA_H();
        else
            SDA_L();
        SendByte <<= 1;
        IIC_Delay(DELAY_TIME);
        SCK_H();
        IIC_Delay(DELAY_TIME);
    }
    SCK_L();
}

static uint8_t I2C_ReadByte(void)
{

    uint8_t i = 8;
    uint8_t ReceiveByte = 0;

    SDA_H();
    while (i--)
    {
        ReceiveByte <<= 1;
        SCK_L();
        IIC_Delay(DELAY_TIME);
        SCK_H();
        IIC_Delay(DELAY_TIME);

        if (SDA_READ())
        {
            ReceiveByte |= 0x01;
        }
    }

    SCK_L();
    return ReceiveByte;
}

/**
  * @brief  通过I2C总线发送指定个数的数据
  * @param  devAddress 从器件写地址
  * @param  pData 指向待发送数据的指针
  * @param  size 待发送的数据的个数，单位字节
  * @retval 0 发送失败，1 发送成功
  */
uint8_t I2C_Soft_Master_Transmit(uint8_t devAddress, uint8_t *pData, uint16_t size)
{
    if (!I2C_Start())
        return 0;

    I2C_SendByte(devAddress);
    if (!I2C_WaitAck())
    {
        I2C_Stop();
        return 0;
    }

    while (size--)
    {
        I2C_SendByte(*pData++);
        if (!I2C_WaitAck())
        {
            I2C_Stop();
            return 0;
        }
    }

    I2C_Stop();
    return 1;
}

/**
  * @brief  通过I2C总线接收指定个数的数据
  * @param  devAddress 从器件写地址
  * @param  pData 指向存储待接收数据的内存地址
  * @param  size 待接收的数据的个数，单位字节
  * @retval 0 接收失败，1 接收成功
  */
uint8_t I2C_Soft_Master_Receive(uint8_t devAddress, uint8_t *pData, uint16_t size)
{

    if (!I2C_Start())
        return 0;

    I2C_SendByte(devAddress + 1);
    if (!I2C_WaitAck())
    {
        I2C_Stop();
        return 0;
    }

    while (--size)
    {
        *pData++ = I2C_ReadByte();
        I2C_Ack();
    }
    *pData = I2C_ReadByte();

    I2C_NoAck();
    I2C_Stop();

    return 1;
}
