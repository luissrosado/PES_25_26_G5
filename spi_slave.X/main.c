/*
 * File:   slave.c
 * Author: guilh
 *
 */

#include "config.h"

#define numWords 4
volatile uint16_t master_com[numWords];

void spi_init()
{
   SPI1CON1Lbits.SPIEN = 0;       // disable SPI
   IFS0bits.SPI1IF = 0;           // Clear interrupt flag
   IEC0bits.SPI1IE = 0;           // Disable interrupt
   SPI1CON1bits.DISSCK = 0;       // Enable clock
   SPI1CON1bits.DISSDO = 0;       // Enable SDO
   SPI1CON1bits.MODE16 = 1;       // 16-bit mode
   SPI1CON1bits.SMP = 0;          // Input sample mid
   SPI1CON1bits.CKP = 0;          // Clock idle low
   SPI1CON1bits.CKE = 1;          // Data changes falling edge
   SPI1CON1bits.MSTEN = 0;        // Slave mode
   SPI1CON2 = 0;                  // Disable frame mode

   RPINR20bits.SDI1R = 26;        // MOSI -> RP26 (P1)
   RPINR20bits.SCK1R = 27;        // SCK  -> RP27 (P2)
   RPINR21bits.SS1R  = 6;         // SS   -> RP6  (P15)
   RPOR3bits.RP7R    = 7;         // MISO -> RP7  (P16)

   SPI1CON1Lbits.SPIEN = 1;       // Enable SPI
}

int main(void) {
    
    uint16_t spiCount = 0;
    spi_init();
    
    while(1){
        while(PORTBbits.RB6 == 0) // SS ativo (baixo)
        {
            if (!SPI1STATLbits.SPIROV) // sem overflow
            {
                if (spiCount < numWords)
                {
                    master_com[spiCount] = SPI1BUFL; // lê do master
                    while (SPI1STATLbits.SPITBF);    // espera buffer livre
                    SPI1BUFL = 0xFFFF;              // responde
                    spiCount++;
                }
            }
        }
        
        if (spiCount >= numWords) // reset contador
            spiCount = 0;
    }
    
    return 0;
}
