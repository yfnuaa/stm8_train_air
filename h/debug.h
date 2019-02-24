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

#define DEBUG   1

void uart1_init(void);
void print_enter(void);
void print(unsigned char *ptr_string);
void print_u8(unsigned char *ptr_string, unsigned char dat);
void print_uint16_t(unsigned char *ptr_string, uint16_t dat);


#endif