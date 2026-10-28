#include<LPC21XX.H>       
#include "types.h"        
#include "delay.h"        
#include "can.h"          
#include "can_defines.h"  

// Enumeration to define different LED indicator modes
typedef enum
{
    MODE_OFF = 0,   // Indicator OFF state
	  MODE_LEFT,      // Left indicator mode
  	MODE_RIGHT      // Right indicator mode
}LED_MODE;

// Variable to store current indicator mode
LED_MODE current_mode = MODE_OFF;

// CAN frame structure used to store received CAN message
CANF rxF;

int main()
{
   Init_CAN1();    // Initialize CAN1 module for communication

   while(1)        // Infinite loop
   {
	   if(CAN1_Rx(&rxF))   // Check if CAN message is received
	   {
	     if(rxF.ID == 2)   // Check if received message ID is 2 (indicator message)
		   {
		      if(rxF.DATA2 == 0x01)   // If received data indicates LEFT indicator
			    {
				    if(current_mode == MODE_LEFT)   // If left indicator already ON
				    {
				       current_mode = MODE_OFF;     // Turn OFF indicator
				    }
				    else
						{
				      current_mode = MODE_LEFT;     // Otherwise turn ON left indicator
			      }
			    }
			    else if(rxF.DATA2 == 0x02)   // If received data indicates RIGHT indicator
			    {
				     if(current_mode == MODE_RIGHT)   // If right indicator already ON
				     {
				        current_mode = MODE_OFF;      // Turn OFF indicator
				     }
				     else
					   {
				       current_mode = MODE_RIGHT;    // Otherwise turn ON right indicator
				     }
			    }
		   }
		}

		   // Execute function based on current indicator mode
		   switch(current_mode)
		   {
		      case MODE_LEFT  : left_indicator();  // Blink left indicator
			                      break;

			    case MODE_RIGHT : right_indicator(); // Blink right indicator
			                      break;

			    case MODE_OFF   : led_off();         // Turn OFF all indicators
			                      break;
		   }
    }
}
