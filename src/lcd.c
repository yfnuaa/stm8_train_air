/**
  ******************************************************************************
  * @file    : lcd.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#include "stm8s.h"
#include "pwm.h"
#include "debug.h"

#define LCD_BACK_LIGHT_PORT       GPIOB
#define LCD_BACK_LIGHT_PIN        GPIO_PIN_4

#define LCD_DA_PORT         GPIOA
#define LCD_DA_PIN          GPIO_PIN_2    
    
#define LCD_CS_PORT         GPIOB
#define LCD_CS_PIN          GPIO_PIN_5

#define LCD_WR_PORT         GPIOA
#define LCD_WR_PIN          GPIO_PIN_1


/********************************************************
功能描述: HT1621芯片测试程序
说 明：本测试程序能够测试HT1621的每一个字段，
            依次点亮每一个字段
参数：
********************************************************/
 
#define _Nop() nop();nop();nop();nop();nop();nop();nop()
#define BIAS_1_3BIAS 0x52 //0b1000 0101 0010 1/3bias 4com
#define BIAS_1_2BIAS 0x50 //0b1000 0101 0000 1/2bias 4com
 
#define SYSDIS    0x00  //0b100 0 0000 000 0 关振系统荡器和LCD偏压发生器
#define SYSEN     0x02  //0b100 0 0000 001 0 打开系统振荡器
 
#define LCDOFF    0x04  //0b100 0 0000 010 0 关LCD偏压
#define LCDON     0x06  //0b100 0 0000 011 0 打开LCD偏压
#define XTAL      0x28  //0b100 0 0010 100 0 外部接时钟
#define RC256     0x30  //0b100 0 0011 000 0 内部时钟
#define TONEON    0x12  //0b100 0000 1001 0 打开声音输出
#define TONEOFF   0x10  //0b100 0000 1000 0 关闭声音输出
#define TONEFRE2K 0xC0  //0b100 0110 0000 0  
#define TONEFRE4K 0x80  //0b100 0110 0000 0  
#define WDTDIS    0x0A  //0b1000 0000 1010 禁止看门狗
 
#define DA_HIGH LCD_DA_PORT->ODR |= (uint8_t)( LCD_DA_PIN)
#define DA_LOW  LCD_DA_PORT->ODR &= (uint8_t)(~LCD_DA_PIN)
 
#define CS_HIGH LCD_CS_PORT->ODR |= (uint8_t)( LCD_CS_PIN)
#define CS_LOW  LCD_CS_PORT->ODR &= (uint8_t)(~LCD_CS_PIN)

#define WR_HIGH LCD_WR_PORT->ODR |= (uint8_t)( LCD_WR_PIN)
#define WR_LOW  LCD_WR_PORT->ODR &= (uint8_t)(~LCD_WR_PIN)


 
/******************************************************
写数据函数,cnt为传送数据位数,数据传送为低位在前
*******************************************************/
void ht1621_write_data(uint8_t Data,uint8_t cnt) 
{
    uint8_t i;
    for (i=0;i<cnt;i++)
    {
       WR_LOW;
       _Nop();
       //HT1621_DAT=Data&0x80;
       if(Data&0x80)DA_HIGH;
       else DA_LOW;
       _Nop();
       WR_HIGH;
       _Nop();
       Data<<=1;
   }
}
 
/********************************************************
函数名称：void ht1621_write_command(uint8_t Cmd)
功能描述: HT1621命令写入函数
全局变量：无
参数说明：Cmd为写入命令数据
返回说明：无
 
说 明：写入命令标识位100
********************************************************/
void ht1621_write_command(uint8_t Cmd)
{
    CS_LOW;
    _Nop();
    ht1621_write_data(0x80,4); //写入命令标志100
    ht1621_write_data(Cmd,8); //写入命令数据
    CS_HIGH;
    _Nop();
}
/********************************************************
函数名称：void ht1621_write_one_data_4bits(uint8_t Addr,uint8_t Data)
功能描述: HT1621在指定地址写入数据函数
全局变量：无
参数说明：Addr为写入初始地址，Data为写入数据
返回说明：无
说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
********************************************************/
void ht1621_write_one_data_4bits(uint8_t Addr,uint8_t Data)
{
    CS_LOW;
    ht1621_write_data(0xa0,3); //写入数据标志101
    ht1621_write_data((u8)(Addr<<2),6); //写入地址数据
    ht1621_write_data((u8)(Data<<4),4); //写入数据
    CS_HIGH;
    _Nop();
}
/********************************************************
HT1621测试程序，2008-2-13, 22:41:43
函数名称：void ht1621_write_all_data(uint8_t Addr,uint8_t *p,uint8_t cnt)
功能描述: HT1621连续写入方式函数
全局变量：无
参数说明：Addr为写入初始地址，*p为连续写入数据指针，
                  cnt为写入数据总数
返回说明：无
说 明：HT1621的数据位4位，此处每次数据为8位，写入数据
           总数按8位计算
********************************************************/
void ht1621_write_all_data(uint8_t Addr,uint8_t *p,uint8_t cnt)
{
    uint8_t i;
    CS_LOW;
    ht1621_write_data(0xa0,3); //写入数据标志101
    ht1621_write_data((u8)(Addr<<2),6); //写入地址数据
    for (i=0; i<cnt; i++)
    {
        ht1621_write_data(*p,8); //写入数据
        p++;
    }
    CS_HIGH;
    _Nop();
}
//u8 BCD_table[]= {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xa, 0xb ， 0xc,0xd,0xe , 0xF};
u8 DIG_CODE[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f ,0x77,0x7C,0x39,0x5e,0x79,0x71}; //数码管数字表

#if 0
u8 g_ht1621_tab[]={0x00,0x00,0x00,0x00,
                  0x00,0x00,0x00,0x00};
#endif

u8 g_Ht1621Tab[]={0x00,0x00,0x00,0x00,
                  0x00,0x00,0x00,0x00};
                  //     com0 com1 com2 com3 com0  com1 com2  com3 
                  //ADD0[ D0   D1   D2   D3   D0    D1   D2    D3 ]
                  //ADD2[ D0   D1   D2   D3   D4    D5   D6    D7 ]
                  //ADD4[ D0   D1   D2   D3   D4    D5   D6    D7 ]                  
/*            
     com0 com1 com2   com3  |ADDr
seg0  D4   D5    D6     D7  | 0
seg1  D0   D1    D2     D3  | 1
                  
                  

     com0 com1 com2   com3  |ADDr
seg0  D0   D1    D2     D3  | 0
seg1  D0   D1    D2     S3  | 1

seg2                        | 2
seg3                    S4  | 3

seg4                        | 4
seg5                    S1  | 5

seg6                        | 6
seg7                    S17 | 7

seg8                        | 8
seg9                    S9  | 9

seg10                       | 10
seg11                   S2  | 11
...                         |... 
seg31                       | 31
      ----------------------
      D3   D2   D1   D0  DATa
*/                        
void ht1621_light_up_always_on_seg(void)
{
    g_Ht1621Tab[0]|= 0x01;ht1621_write_one_data_4bits((u8)(1),  g_Ht1621Tab[0]);//S3  key power
    g_Ht1621Tab[1]|= 0x01;ht1621_write_one_data_4bits((u8)(3 ), g_Ht1621Tab[1]);//S4  key sub
    g_Ht1621Tab[4]|= 0x01;ht1621_write_one_data_4bits((u8)(9 ), g_Ht1621Tab[4]);//S9  key mode
    g_Ht1621Tab[5]|= 0x01;ht1621_write_one_data_4bits((u8)(11), g_Ht1621Tab[5]);//S2  key plus
    g_Ht1621Tab[2]|= 0x01;ht1621_write_one_data_4bits((u8)(5 ), g_Ht1621Tab[2]);//S1  PM2.5 && CO2 icon
    
    g_Ht1621Tab[6]|= 0x01;ht1621_write_one_data_4bits((u8)(13), g_Ht1621Tab[6]);//S5   air quality index
    g_Ht1621Tab[7]|= 0x10;ht1621_write_one_data_4bits((u8)(14), (u8)(g_Ht1621Tab[7]>>4));//S10  Fan step index
    
    g_Ht1621Tab[3]|= 0x1;ht1621_write_one_data_4bits((u8)(7), (u8)(g_Ht1621Tab[7]>>4));//S17   dan wei step index
}           
 void ht1621_fill_digital_code(u8 di, u8 temp)
{
    g_Ht1621Tab[di] &=(u8)(~0xFE);
    if(temp&0x1)
        g_Ht1621Tab[di]|= 0x8;

    if(temp&0x2)
        g_Ht1621Tab[di]|= 0x4;

    if(temp&0x4)
        g_Ht1621Tab[di]|= 0x2;

    if(temp&0x8)
        g_Ht1621Tab[di]|= 0x10;
  
    if(temp&0x10)
        g_Ht1621Tab[di]|= 0x20;

    if(temp&0x20)
        g_Ht1621Tab[di]|= 0x80;

    if(temp&0x40)
        g_Ht1621Tab[di]|= 0x40;

    ht1621_write_all_data((u8)(di*2), g_Ht1621Tab+di,1);
}

  


/********************************************************
函数名称：void ht1621_init(void)
功能描述: HT1621初始化
全局变量：无
参数说明：无
返回说明：无
版 本：1.0
说 明：初始化后，液晶屏所有字段均显示
********************************************************/
void ht1621_init(void)
{
    CS_HIGH;
    WR_HIGH;
    DA_HIGH;
    
   //  DelayMS(2000);      //延时使LCD工作电压稳定
    ht1621_write_command(BIAS_1_3BIAS);
    ht1621_write_command(RC256 ); //使用内部振荡器
    ht1621_write_command(SYSDIS);
    ht1621_write_command(WDTDIS);
    ht1621_write_command(SYSEN );
    ht1621_write_command(LCDON );

    ht1621_write_command(TONEFRE2K ); 
}


void lcd_tone_off(void)
{
    ht1621_write_command(TONEOFF );
}


void lcd_tone_on(void)
{
    ht1621_write_command(TONEON );
}


void lcd_back_light_on(void)
{
    LCD_BACK_LIGHT_PORT->ODR |= (uint8_t)( LCD_BACK_LIGHT_PIN);
}


void lcd_back_light_off(void)
{
    LCD_BACK_LIGHT_PORT->ODR &= (uint8_t)(~LCD_BACK_LIGHT_PIN);
}
void lcd_on(void)
{
    ht1621_write_command(LCDON);
    lcd_back_light_on();
}
void lcd_off(void)
{
    ht1621_write_command(LCDOFF);
    lcd_back_light_off();
}
#if 0
void lcd_update_memory_loop(void)
{
    u8 i,j;
    for(i=0;i<8;i++)
    {
        if(g_ht1621_tab[i] != g_Ht1621Tab[i])
        {
            if(g_ht1621_tab[i]&0xF != g_Ht1621Tab[i]&(u8)0xF)
            {
                ht1621_write_one_data_4bits((u8)(i),g_Ht1621Tab[i]);
            }
            if(g_ht1621_tab[i]&0xF0 != g_Ht1621Tab[i]&(u8)0xF0)
            {
                ht1621_write_one_data_4bits((u8)(i + 1),(u8)(g_Ht1621Tab[i]>>4));
            }
            g_ht1621_tab[i] = g_Ht1621Tab[i];        
        }
    }
}
#endif

void lcd_clear_screen(void)
{
    //ht1621_write_all_data(0,Ht1621Tab,8); //清除1621寄存器数据，暨清屏
}


void lcd_display_pm25( uint16_t pm25 )
{
  //PM25 value
    u8 temp=0; 
    temp = (u8)(pm25/100);
 
    pm25%=100;
    temp = DIG_CODE[temp]; //led digital 1  hundred dig
    ht1621_fill_digital_code(2, temp);
    
    temp = (u8)(pm25/10);
    pm25%=10;
    temp = DIG_CODE[temp]; //led digital 2   ten dig
    ht1621_fill_digital_code(3, temp);
     
    temp = DIG_CODE[pm25]; //led digital 3   ten dig
    ht1621_fill_digital_code(4, temp);
}


void lcd_display_co2( uint16_t c02 )
{//CO2 value
    u8 temp=0;
    temp = (u8)(c02/100);
  
    c02%=100;
    temp = DIG_CODE[temp]; //led digital 4  C02 hundred dig
    ht1621_fill_digital_code(5, temp);
    
    temp = (u8)(c02/10);
    c02%=10;
    temp = DIG_CODE[temp]; //led digital 5   ten dig
    ht1621_fill_digital_code(0, temp);
     
    temp = DIG_CODE[c02];  //led digital 6   ten dig
    ht1621_fill_digital_code(1, temp);
}
#if 0
 //fan step
    if(0 == fan_sept)
    {
          g_Ht1621Tab[7]|= 0x10;       
    }
    else if(1 == fan_sept)                //S14
    {
          g_Ht1621Tab[7]|= 0x90; 
    }
    else if(2 == fan_sept)                //S15
    {
          g_Ht1621Tab[7]|= 0x50; 
    }
    else if(3 == fan_sept)                 //S16
    {
          g_Ht1621Tab[7]|= 0x30; 
    }
#endif
void lcd_light_mode_manual(void)
{    
    g_Ht1621Tab[6]|= 0x02; ht1621_write_one_data_4bits((u8)(13),  g_Ht1621Tab[0]); //S6     //manual icon
}
void lcd_light_mode_auto(void)
{
    g_Ht1621Tab[6]|= 0x04;ht1621_write_one_data_4bits((u8)(13),  g_Ht1621Tab[0]); //S7     //automocit icon
}
void lcd_light_mode_sleep(void)
{
    g_Ht1621Tab[6]|= 0x08;ht1621_write_one_data_4bits((u8)(13),  g_Ht1621Tab[0]); //S8     //sleep mode icon
}


void lcd_display_fan_speed( u8 step)
{
    if(step == 0)
    { 
        //g_Ht1621Tab[7]|= 0x10;    
        g_Ht1621Tab[7]&= (u8)(~0xE0);
 
    }
    else if(step<=e_speed_low)
    {
        g_Ht1621Tab[7]&= (u8)(~0x60);
        g_Ht1621Tab[7]|= 0x80; 
    }
    else if(step<=e_speed_middle)
    {
        g_Ht1621Tab[7]&= (u8)(~0x20);
        g_Ht1621Tab[7]|= 0xC0;     
    }
    else //if(step<=e_speed_high)
    {
        g_Ht1621Tab[7]|= 0xE0;     
    }
    //ht1621_write_one_data_4bits((u8)(7<<1),g_Ht1621Tab[7]);
    ht1621_write_one_data_4bits((u8)(14 ),(u8)(g_Ht1621Tab[7]>>4));
#if 0
    switch( step )
     {
        case e_speed_low:
        break;
            
        case e_speed_middle:
        break;
            
        case e_speed_high:
        break;
            
        default:
        break;
    }
#endif
}


void lcd_display_air_quality( uint16_t air_quality )
{
 //air quality step
    if(0 == air_quality) //good  green      S11
    {
        ht1621_write_one_data_4bits(12,8);
    }
    else if(1 == air_quality) //mid yellow  S12
    {
        ht1621_write_one_data_4bits(12,0x4);    
    }
    else if(2 == air_quality) //high red    S13
    {
        ht1621_write_one_data_4bits(12,2);
    }
 
}


////  1         1        0    push-pull         output    pullup resistor is off    
void lcd_init(void)
{
    u8 i = 0;
    
    //GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
    //GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_OD_LOW_FAST);
    //GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_OD_LOW_FAST);
    LCD_CS_PORT->DDR |= (u8)LCD_CS_PIN;
    LCD_CS_PORT->CR1 &= (u8)(~LCD_CS_PIN);
    LCD_CS_PORT->CR2 &= (u8)(~LCD_CS_PIN);
    
    LCD_WR_PORT->DDR |= (u8)LCD_WR_PIN;
    LCD_WR_PORT->CR1 &= (u8)(~LCD_WR_PIN);
    LCD_WR_PORT->CR2 &= (u8)(~LCD_WR_PIN);
    
    LCD_DA_PORT->DDR |= (u8)LCD_DA_PIN;
    LCD_DA_PORT->CR1 &= (u8)(~LCD_DA_PIN);
    LCD_DA_PORT->CR2 &= (u8)(~LCD_DA_PIN);
    
    ht1621_init();
    
 
    for(i=0;i<8;i++)
    {
        g_Ht1621Tab[i]=0;
    }    

    lcd_back_light_on();
    lcd_tone_off();
}
void lcd_test(void)
{   u8 i;
    u16 j;
    //u8 Ht1621Tab[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    // u8 Ht1621Tab[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
	ht1621_write_all_data(0,g_Ht1621Tab,8); //清除1621寄存器数据，暨清屏
    // ht1621_write_one_data_4bits(1,1);
    //return;
    for(i=0;i<=7;i++)    
    {
        for(j=1;j<=0xFF;j=j)
        { 
						u8 temp;
						temp = (u8)(j);
            ht1621_write_one_data_4bits((u8)(i*2),temp);
						temp = (u8)(temp>>4);
            ht1621_write_one_data_4bits((u8)(i*2+1),temp);
            j=(j<<1)+1;
            delay_ms(30);
        }
    }
    ht1621_write_all_data(0,g_Ht1621Tab,8);
    delay_ms(300);
    
    lcd_light_mode_manual();
    lcd_light_mode_auto();
    lcd_light_mode_sleep();
    
    
    
    ht1621_light_up_always_on_seg();
    
    lcd_display_air_quality(0);
    lcd_display_fan_speed(0); 
    delay_ms(300);
    lcd_display_air_quality(1);
    lcd_display_fan_speed(e_speed_low);
    delay_ms(300);
    lcd_display_air_quality(2);
    lcd_display_fan_speed(e_speed_middle);
    delay_ms(300);
    lcd_display_fan_speed(e_speed_high);
    // for(i=0;i<999;i++)
    {   lcd_display_pm25(000);delay_ms(100);}
    //    for(i=0;i<999;i++)
    { lcd_display_co2(000);delay_ms(100);}
    
}

