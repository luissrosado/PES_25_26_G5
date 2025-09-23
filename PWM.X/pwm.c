#include "pwm.h"

void main(void) {
    TRISBbits.TRISB6 = 0b0;         // RB6 (pin 15 - remappable) defined as output
    OSCCONbits.IOLOCK = 0b0;        // IO Lock for PPS is inactive
    RPOR3bits.RP6R = 13;            // defined RB10 as Output Compare 1 (OC1)
    OSCCONbits.IOLOCK = 0b1;        // IO Lock for PPS is active
    
    uint16_t aux = 99;
    PWM_init(aux);
    
    while(1){
        __delay_ms(2000);
        if(aux == 99){
            aux = aux - 9;
            PWM_updDcyc(aux);
        }else{
            aux = aux - 10;
            PWM_updDcyc(aux);
        }
        
        if(aux <= 0)    aux = 99;       
    }
    
    return;
 }

// PIC24FJ256GA702 uses PPS (Peripheral Pin Selection):
// (datasheet) pages 138 for output function number and 154 for output register