#include <LPC21XX.h>         
#include "interrupt_defines.h" 
#include "interrupt.h"         
#include "types.h"             

volatile u32 flag1 = 0;    // Flag used to indicate left indicator interrupt occurred
volatile u32 flag2 = 0;    // Flag used to indicate right indicator interrupt occurred
volatile u32 blink1 = 0;   // Variable used to control blinking of left indicator
volatile u32 blink2 = 0;   // Variable used to control blinking of right indicator


// Interrupt Service Routine for External Interrupt 0 (EINT0)
void eint0_isr(void) __irq
{
     flag1 = 1;           // Set flag1 to inform main program that EINT0 interrupt occurred

	   blink1 = !blink1;   // Used for blinking left indicator

	   blink2 = 0;	       // Turn OFF right indicator blinking

	   EXTINT = 1<<0;      // Clear EINT0 interrupt flag in EXTINT register

	   VICVectAddr = 0;    // Inform VIC that interrupt service routine execution is finished
}


// Interrupt Service Routine for External Interrupt 2 (EINT2)
void eint2_isr(void) __irq
{
  	 flag2 = 1;           // Set flag2 to inform main program that EINT2 interrupt occurred

	   blink2 = !blink2;   // Used for blinking right indicator

	   blink1 = 0;         // Turn OFF left indicator blinking

	   EXTINT = 1<<2;      // Clear EINT2 interrupt flag in EXTINT register

	   VICVectAddr = 0;    // Inform VIC that interrupt service routine execution is finished
}
  

void Init_EXT_INT(void)
{   
   PINSEL0 |= EINT0_0_1 | EINT2_0_7;   // Configure P0.1 as EINT0 and P0.7 as EINT2

   VICIntEnable = 1<<EINT0_VIC_CHNO | 1<<EINT2_VIC_CHNO;  // Enable interrupts for EINT0 and EINT2 in VIC

   VICVectCntl0 = (1<<5)|EINT0_VIC_CHNO;  // Enable vector slot 0 and assign it to EINT0 interrupt

   VICVectAddr0 = (u32)eint0_isr;         // Store address of EINT0 ISR in vector slot 0

   VICVectCntl2 = (1<<5)|EINT2_VIC_CHNO;  // Enable vector slot 2 and assign it to EINT2 interrupt

   VICVectAddr2 = (u32)eint2_isr;         // Store address of EINT2 ISR in vector slot 2

   EXTMODE = (1<<0)|(1<<2);               // Configure EINT0 and EINT2 as edge triggered interrupts
}
