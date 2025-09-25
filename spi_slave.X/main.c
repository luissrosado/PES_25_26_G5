/*
 * File:   slave.c
 * Author: guilh
 *
 */

#include "config.h"
        
#define SPI_SS_TRIS      TRISBbits.TRISB0
#define SPI_SS_PORT      PORTBbits.RB0


unsigned short spiBufT;		// SPI buffer for transmission
unsigned short My_Slave_Array[4];	// SPI buffer for Receiving

unsigned char spiCount=0;		// variable for how many data has been received from SPI bus


void SPI1Init(void)
{
    //config SPI1
    SPI1CON1bits.SPIEN 		= 0;	// disable SPI port
    SPI1CON1bits.SPISIDL 	= 0; 	// Continue module operation in Idle mode

    SPI1BUFL 				= 0;   	// clear SPI buffer

    //IFS0bits.SPI1IF 		= 0;	// clear interrupt flag
    //IEC0bits.SPI1IE 		= 1;	// enable interrupt
    IFS3bits.SPI1RXIF       = 0;    // clear interrupt flag
    SPI1IMSKLbits.SPITBFEN  = 1;    // buffer rx full
    IEC3bits.SPI1RXIE       = 1;       // enable interrupt

    SPI1CON1bits.DISSDO		= 0;	// SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 	= 1;	// set in 16-bit mode, clear in 8-bit mode
    SPI1CON1bits.SMP		= 0;	// SMP must be cleared when SPIx is used in Slave mode
    SPI1CON1bits.CKP 		= 1;	// CKP and CKE is subject to change ...
    SPI1CON1bits.CKE 		= 0;	// ... based on your communication mode.
    SPI1CON1bits.SSEN		= 1;	// SSx pin is used for Slave mode
	SPI1CON1bits.MSTEN 		= 0; 	// 1 =  Master mode; 0 =  Slave mode

    SPI1CON2 				= 0;	// non-framed mode

    SPI_SS_PORT				= 1;	//
	SPI_SS_TRIS				= 1;	// set SS as input
    
    
   RPINR20bits.SDI1R = 26;        // MOSI -> RP26 (P2)
   RPINR20bits.SCK1R = 27;        // SCK  -> RP27 (P3)
   RPINR21bits.SS1R  = 0;         // SS   -> RP0  (P4)
   RPOR3bits.RP7R    = 7;         // MISO -> RP7  (P16)


    SPI1CON1bits.SPIEN 		= 1; 	// enable SPI port, clear status
}

    
void __attribute__((interrupt, no_auto_psv)) _SPI1RXInterrupt(void) {
    IFS3bits.SPI1RXIF = 0;  // clean interrup flag

        if (!SPI1STATLbits.SPIROV) { // check overflow

                My_Slave_Array[spiCount] = SPI1BUFL; // read master com
                spiCount++;


            while (SPI1STATLbits.SPITBF); // espera buffer livre
            SPI1BUFL = 0xFFFF;
                    ;             // envia de volta
        }
}


int main(void){
    
    while(1);
    
}