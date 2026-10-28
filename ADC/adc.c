#include<LPC21XX.h>      
#include "delay.h"       
#include "adc_defines.h" 
#include "types.h"       
#include "adc.h"         

void Init_ADC(void)
{
	PINSEL1 &= ~(0xff<<22);      // Clear bits 22�29 in PINSEL1 to reset pin functions
	PINSEL1 |=  ((AIN1_0_28));   // Configure P0.28 pin as ADC channel input

	// Set ADC clock divider and power ON the ADC
	ADCR |= (CLKDIV << CLKDIV_BITS_START)|(1<<PDN_BIT);
}

void Read_ADC(u32 chNo,u32 *dVal,f32 *eAR)
{
	ADCR &= 0xffffff00;          // Clear previously selected ADC channel bits

	// Select ADC channel and start conversion
	ADCR |= (1<<chNo)|(1<<CONV_START_BIT);

	delay_us(3);                 // Small delay to allow ADC conversion to begin

	// Wait until ADC conversion is completed (DONE bit becomes 1)
	while(((ADDR>>DONE_BIT)&1)==0);

	ADCR &= ~(1<<CONV_START_BIT); // Stop ADC conversion by clearing start bit

	// Read 10-bit digital result from ADDR register
	*dVal = ((ADDR>>RESULT_BITS_START)&1023);

	// Convert digital value to equivalent analog voltage
	// Formula: Voltage = (Digital Value � Reference Voltage) / ADC Resolution
	*eAR  = ((*dVal)*(3.3/1023));
}


#define FUEL_MIN 120   // fuel min value i.e it indicates no fuel is there
#define FUEL_MAX 720  // fuel max value i.e car full capacity
