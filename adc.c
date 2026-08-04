#include<LPC21XX.h>
#include "delay.h"
#include "adc_defines.h"
#include "types.h"
#include "adc.h"

void Init_ADC(void)
{
    //PINSEL1 = 0;
	PINSEL1 &= ~(0xff<<22);
	PINSEL1 |=  ((AIN1_0_28)); //|(AIN1_0_28)|(AIN2_0_29)|(AIN3_0_30));
	ADCR |= (CLKDIV << CLKDIV_BITS_START)|(1<<PDN_BIT);
}

void Read_ADC(u32 chNo,u32 *dVal,f32 *eAR)
{
  ADCR &= 0xffffff00;
	ADCR |= (1<<chNo)|(1<<CONV_START_BIT);
	delay_us(3);
	while(((ADDR>>DONE_BIT)&1)==0);
	ADCR &= ~(1<<CONV_START_BIT);
	*dVal = ((ADDR>>RESULT_BITS_START)&1023);
	*eAR  = ((*dVal)*(3.3/1023));
}
