/**
  ******************************************************************************
  * @file    SendTO_ANO.h
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

#ifndef _Send_ANO_H_
#define _Send_ANO_H_

#include "stdint.h"
#include "stdbool.h"

extern float    f32_cache[10];
extern int32_t i32_cache[10];
extern int16_t i16_cache[10];

bool SendTO_ANO(uint8_t f32_num, uint8_t i32_num, uint8_t i16_num);

#endif
