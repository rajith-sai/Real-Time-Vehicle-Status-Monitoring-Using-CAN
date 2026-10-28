#include<LPC21xx.h>          
#include "i2c.h"             
#include "delay.h"           
#include "MMA7660.h"         
#include "MMA7660_defines.h" 

// Function to write data to MMA7660 register
void mma7660_write(u8 reg, u8 data)
{
    i2c_start();             // Generate I2C START condition
    i2c_write(MMA7660_SA);   // Send MMA7660 slave address with WRITE operation
    i2c_write(reg);          // Send register address where data should be written
    i2c_write(data);         // Send actual data to that register
    i2c_stop();              // Generate I2C STOP condition
    delay_ms(2);             // Small delay for sensor to process the command
}

// Function to read data from MMA7660 register
u8 mma7660_read(u8 reg)
{
    u8 data;                 // Variable to store received data
    i2c_start();             // Generate START condition
    i2c_write(MMA7660_SA);   // Send slave address with WRITE operation
    i2c_write(reg);          // Send register address from which data should be read
    i2c_restart();           // Generate RESTART condition for read operation
    i2c_write(MMA7660_SA_R); // Send slave address with READ operation
    data = i2c_nack();       // Read data from sensor and send NACK 
    i2c_stop();              // Generate STOP condition
    delay_ms(2);             // Small delay for stability
    return data;             // Return received register value
}

// Function to initialize MMA7660 accelerometer
void Init_mma7660(void)
{
    delay_ms(10);            // Wait for sensor power stabilization
	
    mma7660_write(MMA_MODE, 0x00);  // Put sensor in standby mode before configuration
    delay_ms(10);
	
    mma7660_write(MMA_SR, 0x00);    // Configure sample rate register
    delay_ms(10);
	
    mma7660_write(MMA_MODE, 0x01);  // Enable sensor and switch to active mode
    delay_ms(10);                   // Wait for sensor to start measuring
}


// Function to convert raw sensor data into signed value
s8 convert_data(u8 val)
{
    val &= 0x3F;              // Mask upper bits, keep only lower 6 bits (valid data)

    if(val & 0x20)            // Check sign bit (bit 5 indicates negative value)
        val |= 0xC0;          // Extend sign to make it proper signed 8-bit value

    return (s8)val;           // Return converted signed acceleration value
}


// Function to read X-axis acceleration
s8 mma7660_get_x(void)
{
    return convert_data(mma7660_read(MMA_XOUT)); // Read X register and convert data
}

// Function to read Y-axis acceleration
s8 mma7660_get_y(void)
{
    return convert_data(mma7660_read(MMA_YOUT)); // Read Y register and convert data
}

// Function to read Z-axis acceleration
s8 mma7660_get_z(void)
{
    return convert_data(mma7660_read(MMA_ZOUT)); // Read Z register and convert data
}
