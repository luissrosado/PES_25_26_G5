/*
 * File:   spi.c
 * Author: Guilherme Garcia
 *         Rodrigo Caldeira
 *         Francisco Jesus
 *
 * Created on October 1, 2025, 4:08 PM
 */

#include "sensor_node.h"

extern uint16_t rx_data[4];	// SPI buffer for Receiving
extern uint16_t tx_data[4];

void SPI1Init(void)
{
    //config SPI1
    INTCON2bits.GIE = 0;
    SPI1CON1bits.SPIEN 		= 0;	// disable SPI port
    IEC3bits.SPI1RXIE       = 0;
    SPI1CON1bits.SPISIDL 	= 0; 	// Continue module operation in Idle mode

     SPI1BUFL               = 0;   	// clear SPI buffer
     //ifs3   --- iec3 ---- IPC14
    // FLAG   --- ENABLE ---- Priority
    SPI1IMSKLbits.SPIRBFEN = 1;

    IFS3bits.SPI1RXIF      = 0;
    IFS3bits.SPI1RXIF      = 0;
    IPC14bits.SPI1RXIP     = 7;
    SPI1STATLbits.SPIROV   = 0;


    SPI1CON1bits.DISSDO		= 0;	// SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 	= 1;	// set in 16-bit mode, clear in 8-bit mode
    SPI1CON1bits.SMP		= 0;	// SMP must be cleared when SPIx is used in Slave mode
    SPI1CON1bits.CKP 		= 0;	// CKP and CKE is subject to change ...
    SPI1CON1bits.CKE 		= 1;	// ... based on your communication mode.
    SPI1CON1bits.SSEN		= 1;	// SSx pin is used for Slave mode
	SPI1CON1bits.MSTEN 		= 0; 	// 1 =  Master mode; 0 =  Slave mode

    SPI1CON2 				= 0;	// non-framed mode

    SPI_SS_ANS              = 0;    // define as digital
    SPI_SS_PORT				= 1;	//
	SPI_SS_TRIS				= 1;	// set SS as input
    LED_TRIS                = 0;    // LED pin 15
    LED_LAT                 = 0;
    
    
   ANSAbits.ANSA0           = 0;
   ANSAbits.ANSA1           = 0;
   ANSBbits.ANSB2           = 0;
   ANSBbits.ANSB3           = 0; 
   RPINR20bits.SDI1R        = 26;        // MOSI -> RP26 (P2)
   RPINR20bits.SCK1R        = 27;        // SCK  -> RP27 (P3)
   RPINR21bits.SS1R         = 3;         // SS   -> RP3  (P7)
   RPOR1bits.RP2R           = 7;         // MISO -> RP2  (P6)


    SPI1CON1bits.SPIEN 		= 1; 	// enable SPI port, clear status
    IEC3bits.SPI1RXIE       = 1;
    INTCON2bits.GIE = 1;
}

/*
// implementar byte dummy 
void __attribute__((interrupt, no_auto_psv)) _SPI1RXInterrupt(void){
        IFS3bits.SPI1RXIF = 0;  // clean interrup flag
        LED_LAT = 1;
        if (SPI1STATLbits.SPIROV == 0 ) { // check overflow
            for (uint8_t i = 0; i < 4; i++) {
                while (SPI1STATLbits.SPITBF);
                SPI1BUFL = tx_data[i];
                rx_data[i] = SPI1BUFL;          
            }            
        }else{
            SPI1STATLbits.SPIROV = 0; // clean overflow
        }
        LED_LAT = 0;
}*/

uint8_t i = 0;

void __attribute__((interrupt, no_auto_psv)) _SPI1RXInterrupt(void){
        
        IFS3bits.SPI1RXIF = 0;  // clean interrup flag
        LED_LAT = 1;
        if (SPI1STATLbits.SPIROV == 0 ) { // check overflow
        
                while (SPI1STATLbits.SPITBF);
                SPI1BUFL = tx_data[i];
                rx_data[i] = SPI1BUFL;                      
        }else{
            SPI1STATLbits.SPIROV = 0; // clean overflow
        }
        if(i == 3) i=0;
        i++;
        LED_LAT = 0;
}

