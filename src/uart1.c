/**
  ******************************************************************************
  * @file    : uart.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
  
#include "stm8s.h"

unsigned char HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


void uart1_init(void)
{
      UART1->CR1=0x00;
      UART1->CR2=0x00;
      UART1->CR3=0x00;
      // 设置波特率，必须注意以下几点：
      // (1) 必须先写BRR2
      // (2) BRR1存放的是分频系数的第11位到第4位，
      // (3) BRR2存放的是分频系数的第15位到第12位，和第3位
      // 到第0位
      // 例如对于波特率位9600时，分频系数=2000000/9600=208
      // 对应的十六进制数为00D0，BBR1=0D,BBR2=00

      UART1->BRR2=0x00;
      UART1->BRR1=0x0d;

      UART1->CR2=0x2c;//允许接收，发送，开接收中断
}


static void send_char(unsigned char c)
{
      while((UART1->SR & 0x80)==0x00);
      UART1->DR=c;
}


void print(unsigned char *ptr_string)
{
    while(*ptr_string != '\0')	
    {                   
      send_char(*ptr_string);              
      ptr_string++;  
    }
    
    send_char('\r');
    send_char('\n');
}


void print_u8(unsigned char *ptr_string, unsigned char dat)
{
    while(*ptr_string != '\0')	
    {                   
      send_char(*ptr_string);              
      ptr_string++;  
    }
    
    send_char('0');

    send_char('x');

    send_char(HexTable[dat>>4]);

    send_char(HexTable[dat&0x0f]);

    send_char('\r');
    send_char('\n');
}


void print_u16(unsigned char *ptr_string, uint16_t dat)
{
    while(*ptr_string != '\0')	
    {                   
      send_char(*ptr_string);              
      ptr_string++;  
    }
    
    send_char('0');
    send_char('x');

    send_char(HexTable[dat>>12]);
    send_char(HexTable[dat>>8]);
    send_char(HexTable[dat>>4]);
    send_char(HexTable[dat&0x0f]);

    send_char('\r');
    send_char('\n');
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
