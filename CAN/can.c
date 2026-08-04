#include <LPC21XX.H>
#include "types.h"
#include "delay.h"
#include "can_defines.h"
#include "can.h"
//pinconnect block

void Init_CAN1(void)
{
  PINSEL1 |= RD1_PIN_0_25;
  C1MOD |= 1<<RM_BIT;
  AFMR &= ~(1<<AccOFF_BIT);
  AFMR |= 1<<AccBP_BIT;
  //AFMR = 0x02;
  C1BTR = BTR_LVAL;
  C1MOD &= ~(1<<RM_BIT);
}

void CAN1_Tx(CANF txF)
{
   u32 wait_time = 500000;

   while(((C1GSR>>TBS1_BIT)&1)==0)
   {
       if(wait_time-- == 0)
	   {
	      break;
	   }
   }

   C1TID1 = txF.ID;
   C1TFI1 = ((txF.bfv.RTR<<RTR_BIT)|(txF.bfv.DLC<<DLC_BIT));
   if(txF.bfv.RTR != 1)
   {
      C1TDA1 = txF.DATA1;
	  C1TDB1 = txF.DATA2;
   }
   C1CMR |= ((1<<STB1_BIT)|(1<<TR_BIT));

   wait_time = 500000;

   while(((C1GSR>>TCS1_BIT)&1)==0)
   {
       if(wait_time-- == 0)
	   {
	      break;
	   }
   }	  
}

u8 CAN1_Rx(CANF *rxF)
{
    //while(((C1GSR>>RBS_BIT)&1)==0);
    if(((C1GSR>>RBS_BIT)&1)==0)
	  return 0;

	rxF->ID = C1RID;
	rxF->bfv.RTR = ((C1RFS>>RTR_BIT)&1);
	rxF->bfv.DLC = ((C1RFS>>DLC_BIT)&15);
	if(rxF->bfv.RTR == 0)  //rxF.bfv->RTR
	{
	    rxF->DATA1 = C1RDA;
	    rxF->DATA2 = C1RDB;
    }
	C1CMR |= (1<<RRB_BIT);	//RRB
	return 1;
}
