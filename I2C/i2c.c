#include <LPC21XX.H>        
#include "i2c_defines.h"    
#include "types.h"          

void Init_i2c(void)
{
   PINSEL0 |= SCL_0_2|SDA_0_3;   // Configure P0.2 as SCL and P0.3 as SDA for I2C communication

   I2SCLH = I2C_DIVIDER;         // Set high time of SCL clock 

   I2SCLL = I2C_DIVIDER;         // Set low time of SCL clock 

   I2CONSET = 1<<I2EN_BIT;       // Enable I2C interface by setting I2EN bit
}

void i2c_start(void)
{
   I2CONSET = 1<<STA_BIT;        // Generate START condition on I2C bus

   while(!(I2CONSET&(1<<SI_BIT))); // Wait until START condition is transmitted

   I2CONCLR = 1<<STAC_BIT;       // Clear START bit after START condition is sent
}

void i2c_restart(void)
{
   I2CONSET = 1<<STA_BIT;        // Generate RESTART condition on I2C bus

   I2CONCLR = 1<<SIC_BIT;        // Clear SI flag to continue operation

   while(!(I2CONSET&(1<<SI_BIT))); // Wait until restart condition is completed

   I2CONCLR = 1<<STAC_BIT;       // Clear START bit
}

void i2c_stop(void)
{
   I2CONSET = 1<<STO_BIT;        // Generate STOP condition on I2C bus

   I2CONCLR = 1<<SIC_BIT;        // Clear SI flag to complete operation
}

void i2c_write(u8 dat)
{
   I2DAT = dat;                  // Load data into I2C data register for transmission

   I2CONCLR = 1<<SIC_BIT;        // Clear SI flag to start data transmission

   while(!(I2CONSET&(1<<SI_BIT))); // Wait until data transmission is completed
}

u8 i2c_nack(void)
{
   I2CONCLR = 1<<SIC_BIT;        // Clear SI flag to receive data

   while(!(I2CONSET&(1<<SI_BIT))); // Wait until data is received

   return I2DAT;                 // Return received data without sending ACK (NACK)
}

u8 i2c_mack(void)
{
   I2CONSET = 1<<AA_BIT;         // Set AA bit to send ACK after receiving data

   I2CONCLR = 1<<SIC_BIT;        // Clear SI flag to continue I2C operation

   while(!(I2CONSET&(1<<SI_BIT))); // Wait until data is received

   I2CONCLR = 1<<AAC_BIT;        // Clear AA bit after sending ACK

   return I2DAT;                 // Return received data
}
