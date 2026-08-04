#include "types.h"

typedef struct CAN_Frame
{
   u32 ID;
   //u32 DATA1,DATA2;
   struct BitField
   {
      u32 RTR : 1;
	    u32 DLC : 4;
	    u32 FF : 1;
   }bfv;
   u32 DATA1,DATA2;
}CANF;

void Init_CAN1(void);
void CAN1_Tx(CANF);
u8 CAN1_Rx(CANF *);

void left_indication(void);
void right_indication(void);

void led_off(void);
void Init_LED(void);
void left_indicator(void);
void right_indicator(void); 

void airbag_trigger(void);

/*struct CAN_Frame
{
   u32 ID,RTR,DLC,DATA1,DATA2;
};

void Init_CAN1(void);
void CAN1_Tx(struct CAN_Frame);
void CAN1_Rx(struct CAN_Frame *);*/
