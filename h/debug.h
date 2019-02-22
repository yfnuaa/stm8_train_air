/**
  ******************************************************************************
  * @file    : debug.h
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "stm8s.h"

void uart1_init(void);
void print(unsigned char *ptr_string);
void print_u8(unsigned char *ptr_string, unsigned char dat);
void print_u16(unsigned char *ptr_string, uint16_t dat);


#endif