#include<LPC21XX.H>     
#include "types.h"       
#include "delay.h"     
#include "lcd.h"         
#include "can.h"         
#include "can_defines.h" 
#include "lcd_defines.h"
#include "interrupt.h"   
#include "i2c.h"         
#include "MMA7660.h"     

#define Rx_LED	23       // LED used to indicate CAN reception connected to P0.23

extern volatile u32 flag1;   // Flag set when left indicator interrupt occurs
extern volatile u32 flag2;   // Flag set when right indicator interrupt occurs
extern volatile u32 blink1;  // Flag used for blinking left indicator symbol
extern volatile u32 blink2;  // Flag used for blinking right indicator symbol

CANF rxF;                    // CAN frame structure variable to store received CAN message

int main()
{
   s8 fuel,fuel_block,i;    // fuel = fuel percentage, fuel_block = LCD fuel bar blocks, i = loop variable

   IODIR0 |= 1<<Rx_LED;    // Configure P0.21 and P0.23 as output pins

   InitLCD();                // Initialize LCD display
   WriteToCGRAM();           // Load custom characters (icons) into LCD CGRAM
   Init_CAN1();              // Initialize CAN1 communication
   Init_i2c();               // Initialize I2C communication
   Init_EXT_INT();           // Initialize external interrupts
   Init_mma7660();           // Initialize MMA7660 accelerometer sensor

   while(1)                  // Infinite loop
   {
	   if(flag1)             // Check if left indicator interrupt flag is set
		 {
			  flag1 = 0;         // Clear the flag
		    left_indication(); // Execute left indicator function
		 }

		 if(flag2)            // Check if right indicator interrupt flag is set
		 {
			  flag2 = 0;        // Clear the flag
			  right_indication(); // Execute right indicator function
		 }

       airbag_trigger();     // Check accelerometer values and trigger airbag if accident detected

	   if(CAN1_Rx(&rxF))      // Check if a CAN message is received
	   {
	     fuel = rxF.DATA1;    // Read fuel percentage from received CAN data
		   
		   IOPIN0 ^= 1<<Rx_LED;  // Toggle Rx LED to indicate CAN data received
	     delay_ms(500);        // Delay for visibility

		   fuel_block = fuel/25; // Divide fuel percentage into blocks (0–4 blocks)

		   for(i=0;i<4;i++)     // Loop to display 4 fuel bar blocks
		   {
		       if(i<fuel_block) // If block position is within fuel level
			     CharLCD(i+4);   // Display filled fuel block symbol

			   else
			     CharLCD(' ');   // Otherwise display empty space
		   }

	     CmdLCD(GOTO_LINE1_POS0+10); // Move LCD cursor to specific position
		   IntLCD(fuel);       // Display fuel percentage value
		   StrLCD("%");        // Display percentage symbol
		   CharLCD(' ');       // Clear the percentage symbol
	   }

	   CmdLCD(GOTO_LINE1_POS0);           // Move cursor to first line
	   StrLCD("------DASHBOARD-----");   // Display dashboard title

		 CmdLCD(GOTO_LINE2_POS0);        // Move LCD cursor to second line
	   StrLCD("Fuel:");                // Display text "Fuel:"
		 
	   CmdLCD(GOTO_LINE3_POS0);        // Move cursor to third line
	   StrLCD("AIRBAG STATUS:");      // Display airbag status text

	   CmdLCD(GOTO_LINE3_POS0+15);    // Move cursor position
	   StrLCD("OFF");          // Display airbag status as OFF

		 CmdLCD(GOTO_LINE3_POS0+18);  // Move cursor position
		 CharLCD(3);            // Display airbag icon

	   CmdLCD(GOTO_LINE4_POS0);   // Move cursor to fourth line
	   StrLCD("Indicator:");   // Display text "Indicator:"

	   CmdLCD(GOTO_LINE4_POS0+10);  // Move cursor to left indicator icon position
	   CharLCD(0);             // Display left indicator symbol

	   CmdLCD(GOTO_LINE4_POS0+12); // Move cursor position
	   CharLCD(1);             // Display center indicator symbol

	   CmdLCD(GOTO_LINE4_POS0+14);  // Move cursor to right indicator icon position
	   CharLCD(2);             // Display right indicator symbol

	   if(blink1)              // If left indicator blink flag is set
	   {
	       CmdLCD(GOTO_LINE4_POS0+10); // Move cursor to left indicator
		     CharLCD(' ');      // Clear the symbol (blink OFF)
		     delay_ms(500);     // Wait 500 ms
		     CmdLCD(GOTO_LINE4_POS0+10); // Move cursor again
		     CharLCD(0);        // Display left indicator symbol (blink ON)
	   }
	   else
	   {
	     CmdLCD(GOTO_LINE4_POS0+10);  // Move cursor
		   CharLCD(0);         // Keep left indicator symbol ON
	   }

	   if(blink2)              // If right indicator blink flag is set
	   {
	       CmdLCD(GOTO_LINE4_POS0+14);  // Move cursor to right indicator
		     CharLCD(' ');       // Clear symbol (blink OFF)
		     delay_ms(500);      // Wait 500 ms
		     CmdLCD(GOTO_LINE4_POS0+14);  // Move cursor again
		     CharLCD(2);         // Display right indicator symbol (blink ON)
	   }
	   else
	   {
	       CmdLCD(GOTO_LINE4_POS0+14);  // Move cursor
		     CharLCD(2);       // Keep right indicator symbol ON
	   }
	}
}
