/*
 * File:   i2c.c
 * Author: Guilherme Garcia
 *         Rodrigo Caldeira
 *         Francisco Jesus
 *
 * Created on October 1, 2025, 4:27 PM
 */

#include "i2c.h"
#include <xc.h>

void I2C_init()
{
    I2C1CON1bits.I2CEN = 0;     // I2C module disable for configuration
    I2C1CON1bits.A10M = 0;      // slave adresses are 7 bits long
    I2C1BRG = 0x03;             // configure baud rate, for fcy=4M we'll use 400kHz fSCL option (BRG=0x03)
    ANSELBbits.ANSB9 = 0;       // configure I2C pin as digital (RB8 is digital only and doesn't need to be configured)
    I2C1CON1bits.I2CEN = 1;     // I2C module enable
    TRISBbits.TRISB6 = 0; 
    
    
}


uint16_t I2C_Read_INA(uint8_t slave_addr, uint8_t register_addr)
{   
    uint16_t received_data = 0;

    // start communication
    I2C1CON1bits.SEN = 1;       // init start condition
    while(I2C1CON1bits.SEN);    // wait for hardware clear (end of start condition)
    
    // write
    I2C1TRN = slave_addr & 0b11111110;   // set LSB as Write(0) and store transmission data on register
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete
    while(I2C1STATbits.ACKSTAT); // wait for slave ACK
    
    I2C1TRN = register_addr;    // send register_addr and wait for end of transmission
    while(I2C1STATbits.TRSTAT); // after this, register pointer is set and data can be read
    while(I2C1STATbits.ACKSTAT); // wait for slave ACK
    
    
    // read request
    I2C1CON1bits.RSEN = 1;      // init repeated start condition [posso mudar isto para uma start condition normal? Qual é a diferença?]
    while(I2C1CON1bits.RSEN);   // wait for hardware clear (end of repeated start condition)
    
    I2C1TRN = slave_addr | 1;   // set LSB as Read(1) and store transmission data on register
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete
    while(I2C1STATbits.ACKSTAT); // wait for slave ACK
    
    
    // read data
    // high byte
    I2C1CON1bits.RCEN = 1;      // enable receive
    while(I2C1CON1bits.RCEN);   // wait until the end of slave transmission
    while(!I2C1STATbits.RBF);   // wait until receive buffer is full
    received_data = I2C1RCV;    // different uint sizes, is this an issue?
    received_data = received_data << 8; // shift high byte into position in preparation for low byte

    I2C1CON1bits.ACKEN = 1;
    while(I2C1CON1bits.ACKEN);  // send ACK to confirm data reception

    // low byte
    I2C1CON1bits.RCEN = 1;      // enable receive
    while(I2C1CON1bits.RCEN);   // wait until the end of slave transmission
    while(!I2C1STATbits.RBF);   // wait until receive buffer is full
    received_data = received_data | I2C1RCV; // add the low byte to the received data variable
    
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
    I2C1CONLbits.SEN = 1;       // init start condition
    while(I2C1CONLbits.SEN);    // wait for hardware clear (end of start condition)
    
    
    // write
    I2C1TRN = slave_addr & 0b11111110;   // set LSB as Write(0) and store transmission data on register
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete
    while(I2C1STATbits.ACKSTAT); // wait for slave ACK   
    
    
    I2C1TRN = INA_CAL_ADDR;     // send calibration register address
    while(I2C1STATbits.TRSTAT); // after this, register pointer is set and data can be sent
    while(I2C1STATbits.ACKSTAT); // wait for slave ACK

    
    I2C1TRN = cal >> 8;         // send high byte
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete
    while(I2C1STATbits.ACKSTAT); // wait for slave ACK

    
    I2C1TRN = cal;              // send low byte
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete
    while(I2C1STATbits.ACKSTAT); // wait for slave ACK

    
    // stop communication
    I2C1CON1bits.PEN = 1;       // init stop condition
    while(I2C1CON1bits.PEN);    // wait for hardware clear (end of stop condition) [provavelmente não é necessário esperar a não ser que decidamos escrever logo a seguir, not sure]
    
    return;
}


