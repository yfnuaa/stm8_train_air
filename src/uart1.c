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
      // ���ò����ʣ�����ע�����¼��㣺
      // (1) ������дBRR2
      // (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
      // (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ
      // ����0λ
      // ������ڲ�����λ9600ʱ����Ƶϵ��=2000000/9600=208
      // ��Ӧ��ʮ��������Ϊ00D0��BBR1=0D,BBR2=00

      UART1->BRR2=0x00;
      UART1->BRR1=0x0d;

      UART1->CR2=0x2c;//������գ����ͣ��������ж�
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
