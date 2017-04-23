/**
  ******************************************************************************
  * @file    SendTO_ANO.c
  * @author  LAB201 Huifeng
  * @version V1.0.0
  * @date    2017-04-23
  * @brief   向ANO_TC上位机发送数据
  *          使用方法，包含模块头文件，将需要发送的数据送入制定变量数组中
  *          (f32_cache,i32_cache,i16_cache),然后调用SendTO_ANO函数，根据情况填
  *          入f32_num,i32_num,i16_num的值
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"//需要按情况修改
#include "Send_ANO.h"
#include "stdbool.h"

float   f32_cache[10], *f32;
int32_t i32_cache[10], *i32;
int16_t i16_cache[10], *i16;

extern UART_HandleTypeDef huart1;//需要按情况修改


bool SendTO_ANO(uint8_t f32_num, uint8_t i32_num, uint8_t i16_num)
{
    union fi32
    {
        float u_f32;
        int32_t u_i32;
        int8_t fi32_i8[4];
    } fi32_un;

    union i16
    {
        int16_t u_i16;
        int8_t i16_i8[2];
    } i16_un;

    int8_t buff[32], check = 0;
    uint8_t first = 4;

    f32 = f32_cache;
    i32 = i32_cache;
    i16 = i16_cache;
    /*
    buff[0] = 0x88; buff[1] = 0xa1;
    buff[2] = f32_num * 4 + i32_num * 4 + i16_num * 2;          //帧头的处理
    check = 0x88 + 0xa1 + buff[2];
    */
    buff[0] = 0xaa;
    buff[1] = 0xaa;
    buff[2] = 0xf1;
    buff[3] = f32_num * 4 + i32_num * 4 + i16_num * 2;          //帧头的处理
    check = 0xaa + 0xaa + buff[3] + 0xf1;


    while (f32_num--)                                       //将浮点数据存入发送缓冲区
    {
        //first——此刻存入buff的位置
        fi32_un.u_f32 = *f32++;                             //将传进来的浮点数据存入f32_un联合体

        //将f32_un联合体转成字符进行读取
        buff[first] = fi32_un.fi32_i8[3];
        check += buff[first++];
        buff[first] = fi32_un.fi32_i8[2];
        check += buff[first++];
        buff[first] = fi32_un.fi32_i8[1];
        check += buff[first++];
        buff[first] = fi32_un.fi32_i8[0];
        check += buff[first++];
    }

    while (i32_num--)
    {
        fi32_un.u_i32 = *i32++;

        buff[first] = fi32_un.fi32_i8[3];
        check += buff[first++];
        buff[first] = fi32_un.fi32_i8[2];
        check += buff[first++];
        buff[first] = fi32_un.fi32_i8[1];
        check += buff[first++];
        buff[first] = fi32_un.fi32_i8[0];
        check += buff[first++];
    }

    while (i16_num--)
    {
        i16_un.u_i16 = *i16++;

        buff[first] = i16_un.i16_i8[1];
        check += buff[first++];
        buff[first] = i16_un.i16_i8[0];
        check += buff[first++];
    }

    buff[first++] = check;
    if (first > 32)
    {
        return false;
    }
    HAL_UART_Transmit(&huart1, (uint8_t *)buff, first, 100);//需要按情况修改
    while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);

    return true;
}
