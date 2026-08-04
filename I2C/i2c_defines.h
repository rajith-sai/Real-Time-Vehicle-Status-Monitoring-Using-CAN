#ifndef I2C_DEFINES_H          
#define I2C_DEFINES_H          

#define SCL_0_2 0x00000010     // Configure P0.2 pin as SCL (I2C clock line)
#define SDA_0_3 0x00000040     // Configure P0.3 pin as SDA (I2C data line)
#define FOSC 12000000          // External crystal oscillator frequency 
#define CCLK (5*FOSC)          // CPU clock generated using PLL 
#define PCLK (CCLK/4)          // Peripheral clock 
#define I2C_SPEED 100000       // Desired I2C communication speed = 100 kHz (standard mode)
#define I2C_DIVIDER ((PCLK/I2C_SPEED)/2)      // Divider value used to generate required I2C clock
                                             // Used to configure I2SCLH and I2SCLL registers

#define AA_BIT   2            // Used to send ACK after receiving data
#define SI_BIT   3            // Indicates I2C operation completed
#define STO_BIT  4            // Used to generate STOP condition on I2C bus
#define STA_BIT  5            // Used to generate START condition
#define I2EN_BIT 6            // Enables the I2C hardware module

#define AAC_BIT   2           // Clears ACK control bit
#define SIC_BIT   3           // Clears SI flag after I2C operation
#define STAC_BIT  5           // Used to clear START condition after it is generated
#define I2ENC_BIT 6           // Used to disable the I2C module

#endif                      
