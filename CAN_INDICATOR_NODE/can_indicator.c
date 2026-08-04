#include<LPC21XX.H>      
#include "can.h"         
#include "delay.h"       
#include "types.h"       

#define LED 0             // Starting LED pin position (P0.0)
#define LED_SET 0xFF<<LED // Mask for 8 LEDs starting from P0.0 to P0.7

static s32 position = 0; // Variable to store current LED position

// Function to turn OFF all LEDs
void led_off(void)
{
  IOSET0 = LED_SET;      // Set all LED pins HIGH (assuming LEDs are active LOW ? LEDs OFF)
}
  
// Function to create LEFT indicator running LED pattern
void left_indicator(void)
{
   IODIR0 |= 0xFF << LED; // Configure P0.0–P0.7 as output pins
   IOCLR0 = LED_SET;      // Clear all LED bits (turn ON LEDs i.e Active low leds)
   IOSET0 = 1<<(LED+position); // Turn ON LED at current position
   position++;            // Move LED position to next LED (left direction)

   if(position >= 8)      // If last LED reached
   {
       led_off();         // Turn OFF all LEDs
	     delay_ms(300);     // Wait for a short pause
       position = 0;      // Reset position to start again
   }
   delay_ms(100);         // Small delay for LED movement effect
}


// Function to create RIGHT indicator running LED pattern
void right_indicator(void)
{
   IODIR0 |= 0xFF << LED; // Configure P0.0–P0.7 as output pins
   IOCLR0 = LED_SET;      // Clear all LED bits (turn ON LEDs i.e Active low leds)
   IOSET0 = 1<<(LED+position); // Turn ON LED at current position
   position--;            // Move LED position to previous LED (right direction)

   if(position < 0)       // If first LED passed
   {
       led_off();         // Turn OFF all LEDs
	     delay_ms(300);     // Short pause
       position = 7;      // Reset position to last LED
   }
   delay_ms(100);         // Delay to control LED movement speed
}
