#include "types.h"          

#define MMA7660_SA    0x98  // I2C slave address of MMA7660 sensor for WRITE operation
#define MMA7660_SA_R  0x99  // I2C slave address of MMA7660 sensor for READ operation

#define MMA_XOUT   0x00     // Register address to read X-axis acceleration data
#define MMA_YOUT   0x01     // Register address to read Y-axis acceleration data
#define MMA_ZOUT   0x02     // Register address to read Z-axis acceleration data
#define MMA_TILT   0x03     // Register that provides tilt detection information
#define MMA_SRST   0x04     // Register used for sample count or reset configuration
#define MMA_SPCNT  0x05     // Register used to set sleep count timing
#define MMA_INTSU  0x06     // Interrupt setup register
#define MMA_MODE   0x07     // Mode register used to control standby or active mode of sensor
#define MMA_SR     0x08     // Sample rate register used to configure sensor sampling speed
