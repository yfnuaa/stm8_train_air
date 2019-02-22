 //////////////util//////////////////////////////////////////
 void delay_1ms(void)//1ms延时函数
{
  int i=108;
  while(--i);
 }
 void delay_ms(int ms)//ms延时函数
{
  int i;
  while(--ms)
  {
    delay_1ms();
  }
 }
 //////////////////////////////////