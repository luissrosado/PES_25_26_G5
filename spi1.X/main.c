/*
 * File:   newmainXC16.c
 * Author: guilh
 *
 * Created on September 18, 2025, 9:17 AM
 */


#include "config.h"


void spi_init()
{
   SPI1CON1Lbits.SPIEN = 0;      // disable the spi peripheral
   IFS0bits.SPI1IF = 0;         //  Clear spi interrupt flag
   IEC0bits.SPI1IE = 0;         //  Disable spi interrupt flag
   SPI1CON1bits.DISSCK = 0;      // Enable SCK signal generation
   SPI1CON1bits.DISSDO = 0;      // Enable sdo signal generation
   SPI1CON1bits.MODE16 = 0x01;   // Set 16-bit mode
   SPI1CON1bits.SMP = 0;         // Input is sample at mid-bit
   SPI1CON1bits.CKP = 0;         // clock is low when idle
   SPI1CON1bits.CKE = 1;          // Data changes on the falling edge (because idle is on low if ckp = 1 changes this bit)
   SPI1CON1bits.MSTEN = 1;       // Define device as master
   SPI1BRGLbits.BRG = 4;         // fcy/BRG = freq of spi
   SPI1CON2 = 0;                 // disable frame mode
   RPINR20bits.SDI1R = 8;        // miso config pin to rp8
   RPOR6bits.RP12R = 7;           // mosi config pin to rp12
   RPOR5bits.RP10R = 8;           // clock config pin to rp10
   
   // chip select pin RB14
   ANSELBbits.ANSB14 = 0;
   TRISBbits.TRISB14 = 0;
   
   SPI1CON1Lbits.SPIEN = 0;      // Enable the spi peripheral
}

void spi_com(uint16_t *data, uint16_t *rx_data, uint16_t lenght)
{
    LATBbits.LATB14 = 0;                   // select slave
    for(uint16_t i = 0; i < lenght; i++)
    {
        SPI1BUFL  = data[i];                 // write to buffer for Transmit
        while(!SPI1STATLbits.SPIRBF);       // Wait until tx is completed
        rx_data[i] = SPI1BUFL;               // read the received value 
    }
    LATBbits.LATB14 = 1;
}

int main(void) {
    
    uint16_t tx_data[4] = {0x1234, 0x4567, 0x1234,  0x4567};
    uint16_t rx_data[4];
    
    spi_init();
    spi_com(tx_data, rx_data, 4);
    
    
    return 0;
}
