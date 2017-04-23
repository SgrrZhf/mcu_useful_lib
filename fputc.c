/**
  ******************************************************************************
  * @file    fputc.c
  * @author  LAB201 Huifeng
  * @version V1.0.0
  * @date    2017-04-23
  * @brief   *
  *          annotation
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/**
  * @brief  keil 使用微库方式重定向printf输出
  * @param  *
  * @retval *
  */
int fputc(int ch, FILE *f)
{
    uint8_t temp[1] = { ch };
    HAL_UART_StateTypeDef state;

    HAL_UART_Transmit(&huart1, temp, 1, 2);
    state = HAL_UART_GetState(&huart1);
    while ((state == HAL_UART_STATE_BUSY_TX) || (state == HAL_UART_STATE_BUSY_TX_RX));

    return (1);
}


//勾选Use MicroLib
//包含stdio.h
