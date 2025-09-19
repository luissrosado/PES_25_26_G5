/*
 * File:   main.c
 * Author: guilh
 *
 * Created on September 19, 2025, 6:23 PM
 */


#include "config.h"


#define numWords 2
volatile uint16_t master_com[numWords];
volatile uint16_t spiCount = 0;

void spi_init()
{
   SPI1CON1Lbits.SPIEN = 0;      // disable the spi peripheral
   IFS0bits.SPI1IF = 0;         //  Clear spi interrupt flag
   IEC0bits.SPI1IE = 1;         //  enable spi interrupt flag
   SPI1CON1bits.DISSCK = 0;      // Enable SCK signal generation
   SPI1CON1bits.DISSDO = 0;      // Enable sdo signal generation
   SPI1CON1bits.MODE16 = 1;   // Set 16-bit mode
   SPI1CON1bits.SMP = 0;         // Input is sample at mid-bit
   SPI1CON1bits.CKP = 0;         // clock is low when idle
   SPI1CON1bits.CKE = 1;          // Data changes on the falling edge (because idle is on low if ckp = 1 changes this bit)
   SPI1CON1bits.MSTEN = 0;       // Define device as master
   SPI1CON2 = 0;                 // disable frame mode
   RPINR20bits.SDI1R = 26;        // mosi config pin to rp26 P1
   RPINR20bits.SCK1R = 27;        // clock config pin to rp27 P2
   RPINR21bits.SS1R = 6;          // SS config pint to rp6 p15
   RPOR3bits.RP7R = 7;            // miso config pint to rp7 p16
   
   SPI1CON1Lbits.SPIEN = 1;      // Enable the spi peripheral
}

void __attribute__((interrupt, no_auto_psv)) _SPI1Interrupt(void){
    IFS0bits.SPI1IF = 0; // clean interrupt flag
    
    if (!SPI1STATbits.SPIROV) // check for overflow
    {
        if (spiCount < numWords)
        {
            master_com[spiCount] = SPI1BUF; // read master message   
            while (SPI1STATbits.SPITBF);  // Wait until buffer is empty
            SPI1BUF = 0xFFFF;            // respond to master
            spiCount++;
        }
    }
    
    if (spiCount >= numWords)   // restart counter for next TX
        spiCount = 0; 
}

int main(void) {
    return 0;
}
