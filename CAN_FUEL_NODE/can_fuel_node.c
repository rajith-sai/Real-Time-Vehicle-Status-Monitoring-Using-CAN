#include <LPC21XX.H>       
#include "can.h"             
#include "types.h"         
#include "delay.h"         
#include "adc.h"             

#define FUEL_MIN 120       // fuel min value i.e it indicates no fuel is there shows as 0%
#define FUEL_MAX 720       // fuel max value i.e car full capacity shows as 100%
#define Tx_LED 16          // LED is connected to P0.16 to know whether transmitting the data or not 

u32 dVal;                  // Variable to store ADC digital value
f32 eAR;                   // Variable to store equivalent analog voltage
u32 fuel_per;              // Variable to store fuel percentage

int main()
{
   CANF txF;               // Create a CAN frame variable for transmission

   IODIR0 |= 1<<Tx_LED;    // Set P0.16 as output pin for LED

   Init_CAN1();            // Initialize CAN1 module
   Init_ADC();             // Initialize ADC module

   txF.ID = 1;             // Set CAN message ID as 1
   txF.bfv.RTR = 0;        // Set RTR = 0, means this is a data frame, not remote frame
   txF.bfv.DLC = 1;        // Set DLC = 1, means 1 bytes of data can be sent

   while(1)                // Infinite loop
   {
       Read_ADC(1,&dVal,&eAR);   // Read ADC channel 1 value and store digital and analog results

			 if(dVal<=FUEL_MIN)       // If ADC value is less than or equal to minimum limit
			 {
	        fuel_per = 0;          // Set fuel percentage to 0%
	     }

	     else if(dVal>=FUEL_MAX)   // If ADC value is greater than or equal to maximum limit
	     {
	        fuel_per = 100;        // Set fuel percentage to 100%
	     }

	     else{                     // If ADC value is between minimum and maximum limits
	        fuel_per = (((dVal-FUEL_MIN)*100)/(FUEL_MAX-FUEL_MIN)); // Calculate fuel percentage
	     }

	     txF.DATA1 = fuel_per;     // Store calculated fuel percentage in first CAN data byte

	     CAN1_Tx(txF);             // Transmit CAN frame through CAN1

	     IOPIN0 ^= 1<<Tx_LED;      // Toggle LED state to indicate transmission activity

	     delay_ms(500);            // Wait for 500 milliseconds before next reading
    }
}
