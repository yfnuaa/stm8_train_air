 //////////////util//////////////////////////////////////////
 #include "system.h"
 void delay_1ms(void)//1ms��ʱ����
{
    #ifdef USE_DEFAULT_CLK_2M
    int i=625;
    #else
    int i = 625*8;
    #endif
    while(--i);
 }
 void delay_ms(int ms)//ms��ʱ����
{
  int i;
  while(--ms)
  {
    delay_1ms();
  }
 }
 //////////////////////////////////