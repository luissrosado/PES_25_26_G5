/*
 * File:   main.c
 * Author: guilh
 *
 * Created on September 12, 2025, 4:05 PM
 */

#define FCY 4000000UL
#include <libpic30.h>
#include "p24FJ256GA702.h"
#include "config_bit.h"


int main(void) {
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 0;
    I2C1CON1bits.I2CEN = 1;     // I2C module enable
    I2C1CON1bits.SEN = 1;       // enable ports for serial communication
    
    
    while(1){
        
        I2C1TRNbits.I2CTXDATA = 0xAA;       // SFR to transmit data
        __delay_ms(1000);
        
    }
    
    return 0;
}
