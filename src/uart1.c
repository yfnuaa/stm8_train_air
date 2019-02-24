/**
  ******************************************************************************
  * @file    : uart.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */   
#include "stm8s.h"
#include "debug.h"

#define UART_BRR2   0x00    //例如对于波特率位9600时，分频系数=2000000/9600=208
#define UART_BRR1   0x0d    //例如对于波特率位9600时，分频系数=2000000/9600=208

unsigned char HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


void uart1_init(void)
{
    #if DEBUG
    UART1->CR1 = 0x00;
    UART1->CR2 = 0x00;
    UART1->CR3 = 0x00;

    UART1->BRR2 = UART_BRR2;
    UART1->BRR1 = UART_BRR1;

    UART1->CR2=0x2c;    //允许接收，发送，开接收中断
    #endif
}


static void send_char(unsigned char c)
{
      while((UART1->SR & 0x80)==0x00);
      UART1->DR=c;
}

void print_enter()
{
    #if DEBUG   
    send_char('\r');
    send_char('\n');
    #endif
}

void print(unsigned char *ptr_string)
{
    #if DEBUG
    while(*ptr_string != '\0')	
    {                   
      send_char(*ptr_string);              
      ptr_string++;  
    }
    
    send_char('\r');
    send_char('\n');
    #endif
}


void print_u8(unsigned char *ptr_string, unsigned char dat)
{
    #if DEBUG
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
    #endif
}


void print_uint16_t(unsigned char *ptr_string, uint16_t dat)
{
    #if DEBUG
    while(*ptr_string != '\0')	
    {                   
      send_char(*ptr_string);              
      ptr_string++;  
    }
    
    send_char('0');
    send_char('x');

    send_char(HexTable[dat>>12]);
    send_char(HexTable[(dat&0x0f00)>>8]);
    send_char(HexTable[(dat&0x00f0)>>4]);
    send_char(HexTable[dat&0x000f]);

    send_char('\r');
    send_char('\n');
    #endif
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
