#include<LPC21XX.H>      
#include "types.h"       
#include "delay.h"                
#include "lcd.h"         
#include "i2c.h"     
#include "MMA7660.h"
#include "lcd_defines.h"

s8 x,y,z;                // Variables to store acceleration values from X, Y, Z axis

void airbag_trigger()
{

   x = mma7660_get_x(); // Read X-axis acceleration value from MMA7660 sensor
	 y = mma7660_get_y(); // Read Y-axis acceleration value from MMA7660 sensor
	 z = mma7660_get_z(); // Read Z-axis acceleration value from MMA7660 sensor

   // Check if acceleration exceeds threshold value on any axis
	 if((x <= -25 || x >= 25) || (y <= -25 || y >= 25) || (z <= -25 || z >= 25))
	 {

	     CmdLCD(GOTO_LINE3_POS0+15); // Move LCD cursor to specific position on line 3
		   StrLCD("ON ");            // Display "ON" indicating airbag is activated

		   CmdLCD(GOTO_LINE3_POS0+18); // Move LCD cursor to next position
		   CharLCD(255);              // Display a filled block symbol (airbag icon)

		   delay_ms(1000);            // Wait for 1 second
	 }
}
