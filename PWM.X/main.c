#define FOSC 8000000UL
#define FCY FOSC/2

#include "xc.h"
#include "cfigbits.h"
#include <libpic30.h>
#include <p24FJ256GA702.h>


void main(void) {    
    TRISBbits.TRISB10 = 0b0;        // RB10 (pin 21 - remappable) defined as output
    RPOR5bits.RP10R = 13;           // defined RB10 as Output Compare 1 (OC1)
    OSCCONbits.IOLOCK = 0b1;        // IO Lock for PPS is active

    PR1 = 61;                       // PWM Frequency in Hz (R=16bits, Prescaler 1:1))
 
    OC1R = 0xEFFF;                  // 93% Duty Cycle defined
    OC1RS = 0xFFFF;                 // PWM period as calculated (16 ms)
    
    OC1CON1bits.OCTSEL = 0b111;     // Peripheral Clocl (Fcy) selected
    OC1CON1bits.OCM = 0b011;        // Single Compare Continuous Pulse mode
    
    OC1CON2bits.OCTRIG = 0b0;       // Syncs OCx with source designated by SYNCSELx bits
    OC1CON2bits.SYNCSEL = 0x1F;     // OC1 Sync Out defined
            
     
    while(1){
        //if(OC1TMR == 0xFFFF)
          //  LAT
    }
 }

// PIC24FJ256GA702 uses PPS (Peripheral Pin Selection):
// pages 138 for output function number and 154 for output register