 //////////////util//////////////////////////////////////////
 void delay_1ms(void)//1ms��ʱ����
{
  int i=108;
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