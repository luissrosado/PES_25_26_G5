#define FOSC 8000000UL
#define FCY FOSC/2
#define PWM_FREQ 25000UL            // PWM frequency defined as 25 kHz

#include "xc.h"
#include "cfigbits.h"
#include <libpic30.h>
#include <p24FJ256GA702.h>

void PWM_init(uint16_t dcyc){
    OC1CON1 = 0x00;                 // good practice to clear off the control bits initially 
    OC1CON2 = 0x00;                 // ^^^
    
    OC1CON1bits.OCSIDL = 0b0;       // OC1 continues to operate in Idle mode
    OC1CON1bits.OCTSEL = 0b111;     // Peripheral Clocl (Fcy) selected
    OC1CON1bits.OCM = 0b110;        // Edge-Aligned PWM on OC1
    
    OC1CON2bits.OC32 = 0b0;         // Disable cascading of Two 16b Modules
    OC1CON2bits.OCTRIG = 0b0;       // Syncs OCx with source designated by SYNCSELx bits
    OC1CON2bits.SYNCSEL = 0x1F;     // OC1 Sync Out defined; This selects the synchronization source as itself
    OC1CON2bits.OCTRIS = 0b0;       // OC Peripheral 1 connected to OC1 
       
    PR1 = (FCY / PWM_FREQ) - 1;     // according to datasheet
    
    if(dcyc > 100)  dcyc=100;
    
    OC1R = ((PR1+1)*dcyc)/100;      // duty cycle is defined, pin is set
}


void PWM_updDcyc(uint16_t dcyc){
    if(dcyc > 100)  dcyc=100;
      
    OC1R = ((PR1+1)*dcyc)/100;      // duty cycle is defined, pin is set
}