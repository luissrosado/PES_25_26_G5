/*
 * File:   main.c
 * Author: G5
 *
 * Created on September 12, 2025, 4:05 PM
 */

#define FOSC 8000000UL
#define FCY FOSC/2

#define INA_ADDR 0b10000000 // 7 MSB correspond to address (check p.14 on INA datasheet), last bit will be set or cleared depending on operation (R/~W)
#define INA_CAL_ADDR 0x05   // INA calibration register address
#define INA_CALIBRATION 0x00 // no value for now. Need to make calculations (check p.12 on INA datasheet)

#include <stdint.h>
#include <libpic30.h>
#include "p24FJ256GA702.h"
#include "config_bit.h"

void I2C_init()
{
    I2C1BRG = 3;                // configure baud rate, for fcy=4M we'll use 400kHz fSCL option (BRG=3)
    I2C1CON1bits.I2CEN = 1;     // I2C module enable (não é melhor pôr o resto dos bits a zero?)   
}

uint16_t I2C_Read_INA(uint8_t slave_addr, uint8_t register_addr)
{
    uint16_t received_data = 0;

    // start communication
    I2C1CON1bits.SEN = 1;       // init start condition
    while(I2C1CON1bits.SEN);    // wait for hardware clear (end of start condition)
    
    // write
    I2C1TRN = slave_addr & 0b11111110;   // set LSB as Write(0) and store transmission data on register
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete (slave ACK reception)
    
    I2C1TRN = register_addr;    // send register_addr and wait for ACK
    while(I2C1STATbits.TRSTAT); // after this, register pointer is set and data can be read
    
    
    // read request
    I2C1CON1bits.RSEN = 1;      // init repeated start condition [posso mudar isto para uma start condition normal?]
    while(I2C1CON1bits.RSEN);   // wait for hardware clear (end of repeated start condition)
    
    I2C1TRN = slave_addr | 1;   // set LSB as Read(1) and store transmission data on register
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete (slave ACK reception)
    
    
    // read data
    // high byte
    while(!I2C1STATbits.RBF);   // wait until receive buffer is full
    received_data = I2C1RCV;    // different uint sizes, is this an issue?
    received_data = received_data << 8;

    I2C1CON1bits.ACKEN = 1;
    while(I2C1CON1bits.ACKEN);  // send ACK to confirm data reception

    // low byte
    while(!I2C1STATbits.RBF);   // wait until receive buffer is full
    received_data = received_data | I2C1RCV;
    
    I2C1CON1bits.ACKEN = 1;
    while(I2C1CON1bits.ACKEN);  // send ACK to confirm data reception
    
    // stop communication
    I2C1CON1bits.PEN = 1;       // init stop condition
    while(I2C1CON1bits.PEN);    // wait for hardware clear (end of stop condition) [provavelmente não é necessário esperar a não ser que decidamos escrever logo a seguir, not sure]

    return received_data;
}

void I2C_Calibrate_INA(uint8_t slave_addr, uint16_t cal)
{
    // start communication
    I2C1CON1bits.SEN = 1;       // init start condition
    while(I2C1CON1bits.SEN);    // wait for hardware clear (end of start condition)
    
    
    // write
    I2C1TRN = slave_addr & 0b11111110;   // set LSB as Write(0) and store transmission data on register
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete (slave ACK reception)
    
    I2C1TRN = INA_CAL_ADDR;     // send calibration register address and wait for ACK
    while(I2C1STATbits.TRSTAT); // after this, register pointer is set and data can be sent
    
    I2C1TRN = cal >> 8;         // send high byte
    while(I2C1STATbits.TRSTAT); // wait for ACK
    
    I2C1TRN = cal;              // send low byte
    while(I2C1STATbits.TRSTAT); // wait for ACK
    
    
    // stop communication
    I2C1CON1bits.PEN = 1;       // init stop condition
    while(I2C1CON1bits.PEN);    // wait for hardware clear (end of stop condition) [provavelmente não é necessário esperar a não ser que decidamos escrever logo a seguir, not sure]
    
    return;
}

int main(void) {
    // isto é necessário ou era para os leds?
    //ANSELBbits.ANSB3 = 0;
    //TRISBbits.TRISB3 = 0;
    uint16_t reading_raw = 0;
    uint8_t register_addr = 0x03;   // the register we'll read from INA (check p.18 on INA datasheet)
    
    I2C_init();
    
    while(1)
    {
        I2C_Calibrate_INA(INA_ADDR, INA_CALIBRATION)
        reading_raw = I2C_Read_INA(INA_ADDR, register_addr);
        __delay_ms(1000);
        
    }
    
    return 0;
}
