#include<LPC21xx.h>        
#include "can.h"           
#include "can_defines.h"   
#include "types.h"         
#include "delay.h"         

void left_indication()
{
	    CANF txF;            // Create a CAN frame variable for transmission
	    txF.ID = 2;          // Set CAN message ID to 2 (used for indicator messages)
			txF.bfv.RTR = 0;    // RTR = 0 means it is a data frame (not a remote request)
			txF.bfv.DLC = 1;    // DLC = 1 means 1 byte of data will be transmitted
	
			txF.DATA2 = 0x01;   // Send value 0x01 to indicate LEFT indicator ON
			CAN1_Tx(txF);       // Transmit the CAN frame using CAN1
}

void right_indication()
{
	    CANF txF;            // Create a CAN frame variable for transmission
	    txF.ID = 2;          // Set CAN message ID to 2 (same ID used for indicator messages
			txF.bfv.RTR = 0;    // RTR = 0 means it is a data frame
			txF.bfv.DLC = 1;    // DLC = 1 means 1 byte of data will be transmitted

			txF.DATA2 = 0x02;   // Send value 0x02 to indicate RIGHT indicator ON
			CAN1_Tx(txF);       // Transmit the CAN frame through CAN1
}
