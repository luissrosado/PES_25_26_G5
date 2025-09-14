/*
 * File:   main.c
 * Author: G5
 *
 * Created on September 12, 2025, 4:05 PM
 */

#define FOSC 8000000UL
#define FCY FOSC/2

#include <stdint.h>
#include <libpic30.h>
#include "p24FJ256GA702.h"
#include "config_bit.h"

void I2C_init()
{
    I2C1BRG = 3;                // configure baud rate, for fcy=4M we'll use 400kHz fSCL option (BRG=3)
    I2C1CONbits.I2CEN = 1;      // I2C module enable (não é melhor pôr o resto dos bits a zero?)   
}

void I2C_Tx(uint8_t slave_addr, uint8_t data)
{
    I2C1CONbits.SEN = 1;        // init start condition
    while(I2C1CONbits.SEN);     // wait for hardware clear (end of start condition)
    
    I2C1TRN = slave_addr;       // the slave address is 7 bits (some devices have 10 bits) and 8th bit is R/~W
    while(I2C1STATbits.TRSTAT); // wait until the transmission is complete (slave ACK reception)
    
    I2CTRN = data;
    while(I2C1STATbits.TRSTAT); // send data and wait for ACK
    
    I2C1CONbits.PEN = 1;        // init stop condition
    while(I2C1CONbits.SEN);     // wait for hardware clear (end of stop condition) [provavelmente não é necessário esperar a não ser que decidamos escrever logo a seguir, not sure]
}

int main(void) {
    // isto é necessário ou era para os leds?
    //ANSELBbits.ANSB3 = 0;
    //TRISBbits.TRISB3 = 0;
    Tx_data = 0xAF;
    slave = 0xFF;
    I2C_init();
    
    while(1){
        
        I2C_Tx(slave, Tx_data);
        __delay_ms(1000);
        
    }
    
    return 0;
}
