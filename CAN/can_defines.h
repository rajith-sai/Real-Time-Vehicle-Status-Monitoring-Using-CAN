#include "types.h"            

#define RD1_PIN_0_25 0x15440000   // Configure P0.25 pin as RD1 
#define PCLK 15000000             // Peripheral clock frequency 
#define BIT_RATE 125000           // Desired CAN communication speed 
#define QUANTA 15                 // Number of time quanta used for one CAN bit

#define BRP (PCLK/(BIT_RATE*QUANTA)) // Used to divide PCLK to generate CAN bit timing clock
#define SAMPLE_POINT (0.7*QUANTA) // Sample point position
#define TSEG1 ((u32)SAMPLE_POINT-1) // Time Segment 1 length
                                   // From sync segment to sample point
#define TSEG2 (QUANTA-(1+TSEG1))   // Time Segment 2 length
                                   // From sample point to end of bit
#define SJW ((TSEG2 >= 5)?4:(TSEG2-1)) // Used for resynchronization in CAN communication

#define SAM 0                    // Sampling mode
#define BTR_LVAL ((SAM<<23)|((TSEG2-1)<<20)|((TSEG1-1)<<16)|((SJW-1)<<14)|(BRP-1)) // Complete value for CAN Bit Timing Register (C1BTR)

#define TR_BIT   0                 // Transmission Request bit in CAN command register
#define RRB_BIT  2                // Release Receive Buffer bit
#define STB1_BIT 5               // Select Transmit Buffer 1 bit

#define RBS_BIT  0                // Receive Buffer Status bit
#define TBS1_BIT 2               // Transmit Buffer 1 Status bit
#define TCS1_BIT 3               // Transmit Complete Status bit
#define FF_BIT   31                // Frame Format bit (standard or extended frame)
#define RTR_BIT  30               // Remote Transmission Request bit

#define DLC_BIT 16               // Data Length Code starting bit position

#define RM_BIT 0                 // Reset Mode bit in CAN Mode Register

#define AccOFF_BIT 0             // Acceptance Filter OFF bit
#define AccBP_BIT  1             // Acceptance Filter Bypass bit
