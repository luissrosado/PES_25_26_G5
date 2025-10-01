#include "pwm.h"

void main(void) {
    INTCON2bits.GIE = 0b1;          // Interrupts enabled
    CLKDIVbits.DOZEN = 0b0;         // CPU peripheral clock ratio is set to 1:1
    CLKDIVbits.PLLEN = 0b0;         // PLL disabled
    
    OSCCONbits.IOLOCK = 0b0;        // IO Lock for PPS is inactive
    
    TRISBbits.TRISB6 = 0b0;         // RB6 (pin 15 - remappable) defined as output
    RPOR3bits.RP6R = 13;            // defined RB6 as Output Compare 1 (OC1)

    TRISBbits.TRISB7 = 0b0;         // RB7 (pin 16 - remappable) defined as output
    RPOR3bits.RP7R = 14;            // defined RB7 as Output Compare 2 (OC2)

    TRISBbits.TRISB5 = 0b0;         // RB5 (pin 14 - remappable) defined as output
    RPOR2bits.RP5R = 15;            // defined RB5 as Output Compare 3 (OC3)
    
    OSCCONbits.IOLOCK = 0b1;        // IO Lock for PPS is active
    
    uint16_t aux = 99;
    PWM_init(3, aux);
    
    while(1){
        //__delay_ms(50);
        //aux = aux - 1;
        //PWM_updDcyc(1, aux);
        
        //if(aux <= 0)    aux = 99;       
    }
 }

// PIC24FJ256GA702 uses PPS (Peripheral Pin Selection):
// (datasheet) pages 138 for output function number and 154 for output register