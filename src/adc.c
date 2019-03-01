#include "adc.h"
#include "debug.h"
#include "pm25.h"
#include "co2.h"
u16 g_ADC1_4C_Value;
u16 g_ADC1_3C_Value;

#include "adc.h"


#define ADC_CONVERSION_SPEED            ADC1_PRESSEL_FCPU_D2

#define SYSTEM_ADC_RESOLUTION_RATIO     1023.0
#define VOLTAGE_REF                     5000.0  //mv
#define VOLTAGE_MG812_REF               500.0   //mv


volatile uint16_t g_ad_value = 0;

volatile BitStatus  g_adc_finished = 0;
void adc1_reset(void)
{
	   ADC1_DeInit();
     g_adc_finished = RESET;
		 g_ad_value = 0;
}
#if 1
void adc1_isr(void)
{
    /* Get converted value */
    g_ad_value = ADC1_GetConversionValue( ); 
    print_u16("adc [",g_ad_value);
 
    g_adc_finished = SET;
    ADC1_ClearITPendingBit(ADC1_IT_EOC);
}
 #endif
//PD_DDR     CR1      CR2
//  0         0        0    float             input     pullup resistor is off
//  0         1        0    pullup            input     pullup resistor is on 
//  0         0        1    interrup float    input     pullup resistor is off 
//  0         1        1    interrup pullup   input     pullup resistor is on 

void ADC1_C4_Init(void)  //co2
{
    //ADC1_DeInit();
   
    //GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);//AIN4 IO设置为上拉输入
         //sensor vcc enable
    GPIOD->DDR &=(u8)( ~GPIO_PIN_3);
    GPIOD->CR1 |= GPIO_PIN_3;
    GPIOD->CR2 &=(u8)( ~GPIO_PIN_3);
#ifdef CONVERSIONMODE_SINGLE
   /**< Single conversion mode */
   /**< Analog channel 10 */
   /**< Prescaler selection fADC2 = fcpu/18 */
   /**< Conversion from Internal TIM TRGO event */
   /** DISABLE ADC2_ExtTriggerState**/
   /**< Data alignment right */
   /**< Schmitt trigger disable on AIN10 */
   /**DISABLE ADC2_SchmittTriggerState*/
   ADC1_Init(ADC1_CONVERSIONMODE_SINGLE , ADC1_CHANNEL_4, ADC1_PRESSEL_FCPU_D18,
   ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL4,DISABLE);
   ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
   
   
#else // CONVERSIONMODE ==CONVERSIONMODE_CONTINUOUS
   /**< Continuous conversion mode */
   /**< Analog channel 10 */
   /**< Prescaler selection fADC2 = fcpu/18 */
   /**< Conversion from Internal TIM TRGO event */
   /** DISABLE ADC2_ExtTriggerState**/
   /**< Data alignment right */
   /**< Schmitt trigger disable on AIN10 */
   /**DISABLE ADC2_SchmittTriggerState*/
   ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS , ADC1_CHANNEL_4, ADC1_PRESSEL_FCPU_D18,
   ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL4,DISABLE);
   ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
#endif
   
    ADC1_Cmd(ENABLE);
    ADC1_StartConversion();
}

 
void ADC1_C3_Init(void)  //PM25
{
    //GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_IN_FL_NO_IT); 
    //ADC1_DeInit();
    //GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);//AIN3 IO设置为上拉输入
    GPIOD->DDR &=(u8)( ~GPIO_PIN_2);
    GPIOD->CR1 |= GPIO_PIN_2;
    GPIOD->CR2 &=(u8)( ~GPIO_PIN_2);
#ifdef CONVERSIONMODE_SINGLE
   /**< Single conversion mode */
   /**< Analog channel 10 */
   /**< Prescaler selection fADC2 = fcpu/18 */
   /**< Conversion from Internal TIM TRGO event */
   /** DISABLE ADC2_ExtTriggerState**/
   /**< Data alignment right */
   /**< Schmitt trigger disable on AIN10 */
   /**DISABLE ADC2_SchmittTriggerState*/
   ADC1_Init(ADC1_CONVERSIONMODE_SINGLE , ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D18,
   ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3,DISABLE);
   ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
   
   
#else //CONVERSIONMODE_CONTINUOUS
   /**< Continuous conversion mode */
   /**< Analog channel 10 */
   /**< Prescaler selection fADC2 = fcpu/18 */
   /**< Conversion from Internal TIM TRGO event */
   /** DISABLE ADC2_ExtTriggerState**/
   /**< Data alignment right */
   /**< Schmitt trigger disable on AIN10 */
   /**DISABLE ADC2_SchmittTriggerState*/
   ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS , ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D18,
   ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3,DISABLE);
   ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
#endif
   
    ADC1_Cmd(ENABLE);
   ADC1_StartConversion();
}
#if 0
void  adc_init(void)
{
    GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);//AIN4 IO设置为上拉输入
    ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);//预分频2
	ADC1_ExternalTriggerConfig(ADC1_EXTTRIG_TIM,DISABLE);//不使用外部触发  
	ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL4,DISABLE); //禁止AIN2 AIN4的施密特触发器，降低IO静态功耗

    //PD5,PD6上的通道如果施密特方式禁用会导致串口无法收发数据！
    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, //连续转换
                  ADC1_CHANNEL_4,//只能选择一个通道！
                   ADC1_ALIGN_RIGHT);//右对齐
    ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
    ADC1_Cmd(ENABLE);//开启ADC
    ADC1_StartConversion();//开启连续转换
    ADC1_DataBufferCmd(ENABLE);//开启缓存
}

void main( void )
{ 
    u16value=0;   
	adc_init(); 
	while(1)
    {
        while(!ADC1_GetFlagStatus(ADC1_FLAG_EOC));//等待转换完成
        ADC1_ClearFlag(ADC1_FLAG_EOC);//软件清除
        value=0;
        for(u8 i=0;i<10;i++)//
        {
            value+=ADC1_GetBufferValue(i);//将10个缓存中的值求和
        }
        value=value/10;//求出ADC的平均值
    }
}
#endif


  