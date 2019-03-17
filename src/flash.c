/**
  ******************************************************************************
  * @file    : flash.c
  * @brief   :
  * @author  : 
  * @version :
  * @date    :
  */ 
#include "stm8s.h"
#include "system.h"


void read_serial_no(void)
{
    #if 0
    u32 serialno = 0;
    serialno  = (u32)( FLASH_ReadByte(SERIALNO   )) <<24; 
    serialno |= (u32)( FLASH_ReadByte(SERIALNO + 1))<<16;  
    serialno |= (u32)( FLASH_ReadByte(SERIALNO + 2))<<8; 
    serialno |=        FLASH_ReadByte(SERIALNO + 3);
    
    g_pm25_dust_density_ug_m3 =(uint16_t) (serialno / 1000);
    g_pm25_dust_density_ug_m3 += 10000; //show A01 234 //serial no 1234
    g_co2_value = (uint16_t)(serialno % 1000);
    #endif
}


