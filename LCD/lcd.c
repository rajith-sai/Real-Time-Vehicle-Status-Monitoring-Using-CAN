#include<LPC21xx.h>          
#include "delay.h"           
#include "types.h"           
#include "defines.h"         
#include "lcd.h"             
#include "lcd_defines.h"     

void InitLCD(void)
{
    IODIR0 |= (0xff<<LCD_DATA)|(1<<RS)|(1<<RW)|(1<<EN); // Configure LCD data pins and control pins as output
	  delay_ms(15);            // Wait for LCD power stabilization
	  CmdLCD(0x30);            // Initialize LCD (first command in initialization sequence)
	  delay_ms(5);             // Small delay
	  CmdLCD(0x30);            // Repeat initialization command
	  delay_ms(1);             // Small delay
	  CmdLCD(0x30);            // Repeat again for proper initialization
	  delay_ms(1);             // Small delay
	  CmdLCD(0x38);            // Set LCD to 8-bit mode, 2-line display
	  CmdLCD(0x01);            // Clear LCD display
	  CmdLCD(0x06);            // Set cursor move direction (increment mode)
	  CmdLCD(0x0C);            // Turn ON display and hide cursor
}

void DispLCD(u8 val)
{
    IOCLR0=1<<RW;             // Set RW = 0 (write operation)
	  WRITEBYTE(IOPIN0,LCD_DATA,val); // Write data/command to LCD data pins
	  IOSET0=1<<EN;            // Enable LCD
	  delay_us(2);             // Small enable pulse delay
	  IOCLR0=1<<EN;            // Disable LCD 
	  delay_ms(2);             // Wait for LCD to process data
}

void CmdLCD(u8 cmd)
{
     IOCLR0 = 1<<RS;          // Command register selected
	   DispLCD(cmd);           // Send command to LCD
}

void CharLCD(s8 data)
{
     IOSET0 = 1<<RS;          // Data register selected
     DispLCD(data);           // Send character data to LCD
}

void StrLCD(s8 *ptr)
{
    while(*ptr!='\0')         // Loop until end of string
	  {
	      CharLCD(*ptr);       // Display each character on LCD
	      ptr++;               // Move to next character
	  }
}

void IntLCD(s32 num)
{
    s8 a[10];                 // Array to store digits
	  s32 i=0;                  // Index variable

	  if(num==0)                // If number is zero
	  {
	    CharLCD(0+48);         // Display character '0'
	  }
	  else
	  {
	    if(num<0)               // If number is negative
			{
	        CharLCD('-');      // Display minus sign
		      num = -num;       // Convert number to positive
			}

	    while(num)              // Convert integer to digits
	    {
	         a[i++]=num%10+48; // Store digit as ASCII character
	         num /= 10;        // Remove last digit
	    }

	     --i;                  // Adjust index

	   for(;i>=0;i--)          // Print digits in correct order
	   {
	       CharLCD(a[i]);      // Display digit
	   }
	}
}

void WriteToCGRAM(void)
{
    s8 i;                    // Loop variable
	 
	  s8 a[] = {0x00,0x06,0x0E,0x1E,0x1E,0x0E,0x06,0x00}; // Custom character for Left indicator
	  s8 b[] = {0x00,0x04,0x0E,0x1F,0x1F,0x0E,0x04,0x00}; // Custom character for Diamond symbol
	  s8 c[] = {0x00,0x0C,0x0E,0x0F,0x0F,0x0E,0x0C,0x00}; // Custom character for Right indicator
	  s8 d[] = {0x1F,0x11,0x11,0x11,0x11,0x11,0x11,0x1F}; // Custom character for Airbag 
	  s8 e[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00}; // Custom character for Fuel bar level 1
	  s8 f[] = {0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x00}; // Custom character for Fuel bar level 2
	  s8 g[] = {0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x00}; // Custom character for Fuel bar level 3
	  s8 h[] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x00}; // Custom character for Fuel bar level 4

	  CmdLCD(0x40);            // Set CGRAM address to store custom characters

	  for(i=0;i<8;i++)          // Write first custom character
	    CharLCD(a[i]);

    for(i=0;i<8;i++)       // Write second custom character
	    CharLCD(b[i]);

	  for(i=0;i<8;i++)         // Write third custom character
	    CharLCD(c[i]);

	  for(i=0;i<8;i++)         // Write fourth custom character
	    CharLCD(d[i]);

	  for(i=0;i<8;i++)         // Write fifth custom character
	    CharLCD(e[i]);

    for(i=0;i<8;i++)       // Write sixth custom character
	    CharLCD(f[i]);

	  for(i=0;i<8;i++)         // Write seventh custom character
	    CharLCD(g[i]);

	  for(i=0;i<8;i++)         // Write eighth custom character
	    CharLCD(h[i]);
}

void FloatLCD(f32 fN,u32 nDP)
{
   u32 n;                     // Variable to store integer part
   s32 i;                     // Loop variable

   if(fN<0.0)                 // If number is negative
   {
      CharLCD('-');           // Display minus sign
	    fN = -fN;               // Convert number to positive
   }

   n=fN;                      // Extract integer part
   IntLCD(n);                 // Display integer part
   CharLCD('.');              // Display decimal point

   for(i=0;i<nDP;i++)         // Loop for decimal places
   {
       fN=(fN-n)*10;          // Extract next decimal digit
	     n=fN;
	     CharLCD(n+48);        // Display decimal digit
   }
}

void HexLCD(u8 data)
{
   u8 up,low;                 // Variables to store upper and lower nibbles

   up = (data>>4) & 0x0F;     // Extract upper 4 bits
   low = data & 0x0F;         // Extract lower 4 bits

   if(up<10)
     DispLCD(up+'0');         // Convert to ASCII number
   else
     DispLCD(up-10+'A');      // Convert to ASCII letter (A–F)

   if(low<10)
     DispLCD(low+'0');        // Convert to ASCII number
   else
     DispLCD(low-10+'A');     // Convert to ASCII letter (A–F)
}
   
void Hex_data(u32 value)
{
   StrLCD("0x");              // Display "0x" prefix for hexadecimal

   HexLCD((value>>24)&0xFF);  // Display highest byte
   HexLCD((value>>16)&0xFF);  // Display second byte
   HexLCD((value>>8)&0xFF);   // Display third byte
   HexLCD(value&0xFF);        // Display lowest byte
}
