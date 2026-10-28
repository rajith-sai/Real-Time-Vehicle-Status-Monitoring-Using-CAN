#include <LPC21XX.H>      
#include "types.h"        
#include "delay.h"        
#include "can_defines.h"  
#include "can.h"          

void Init_CAN1(void)
{
  PINSEL1 |= RD1_PIN_0_25;     // Configure P0.25 pin as CAN1 Receive

  C1MOD |= 1<<RM_BIT;          // Set Reset Mode bit to put CAN controller in reset mode

  AFMR &= ~(1<<AccOFF_BIT);    // Enable Acceptance Filter

  AFMR |= 1<<AccBP_BIT;        // Set Acceptance Filter to bypass mode

  C1BTR = BTR_LVAL;            // Set CAN bit timing register 

  C1MOD &= ~(1<<RM_BIT);       // Clear Reset Mode bit to start CAN controller
}

void CAN1_Tx(CANF txF)
{
   u32 wait_time = 500000;     // Timeout counter to avoid infinite waiting

   // Wait until Transmit Buffer 1 becomes free
   while(((C1GSR>>TBS1_BIT)&1)==0)
   {
     if(wait_time-- == 0)      // If timeout occurs
	   {
	      break;                // Exit loop to avoid blocking
	   }
   }

   C1TID1 = txF.ID;            // Load CAN message ID into transmit identifier register

   // Load Frame Information(RTR and DLC values)
   C1TFI1 = ((txF.bfv.RTR<<RTR_BIT)|(txF.bfv.DLC<<DLC_BIT));

   // If frame is not a Remote Frame, load data into transmit registers
   if(txF.bfv.RTR != 1)
   {
      C1TDA1 = txF.DATA1;      // Load first 4 bytes of data
	    C1TDB1 = txF.DATA2;      // Load next 4 bytes of data
   }

   // Select transmit buffer 1 and request transmission
   C1CMR |= ((1<<STB1_BIT)|(1<<TR_BIT));

   wait_time = 500000;         // Reset timeout counter

   // Wait until transmission is completed
   while(((C1GSR>>TCS1_BIT)&1)==0)
   {
     if(wait_time-- == 0)      // If timeout occurs
	   {
	      break;                // Exit loop
	   }
   }	  
}

u8 CAN1_Rx(CANF *rxF)
{
    if(((C1GSR>>RBS_BIT)&1)==0)   // Check if a message is available in receive buffer
	  return 0;                    // If no message received, return 0

	rxF->ID = C1RID;              // Read received CAN message ID

	rxF->bfv.RTR = ((C1RFS>>RTR_BIT)&1);   // Read RTR bit (Remote Transmission Request)

	rxF->bfv.DLC = ((C1RFS>>DLC_BIT)&15);  // Read DLC (Data Length Code)

	if(rxF->bfv.RTR == 0)       // If received frame is a data frame, read data bytes
	{
	    rxF->DATA1 = C1RDA;     // Read first 4 bytes of received data
	    rxF->DATA2 = C1RDB;     // Read next 4 bytes of received data
  }

	C1CMR |= (1<<RRB_BIT);      // Release receive buffer so next message can be received

	return 1;                   // Return 1 to indicate message was received successfully
}
