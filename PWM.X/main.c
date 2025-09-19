#define FCY 8000000UL
#include "xc.h"
#include <libpic30.h>
#include "cfigbits.h"
#include <p24FJ256GA702.h>

void main(void) {
    CCP2RA = 0x7FFF;                // 50% Duty Cycle
    CCP2PRL = 0xFFFF;               // 
    
    TRISBbits.TRISB14 = 0b0;        // pin 25 (RB14) defined as output
    ANSELBbits.ANSB14 = 0b0;        // pin 25 (RB14) defined an digital O
    
    // PIC24FJ256GA702 uses PPS (Peripheral Pin Selection):
    RPOR7bits.RP14R = 0x14;         // RB14 is set with OC2 Output Compare/PWM function
    // pages 138 for output function number and 154 for output register
    
    CCP2CON1Lbits.CCPON = 0b0;      // clear associated registers
    // CCPON = 0 disables module and is enabled once MOD[3:0] is specified
    
    CCP2CON1Lbits.TMRSYNC = 0b1;    // time base clock synced to internal system clocks
    CCP2CON1Lbits.CLKSEL = 0b000;   // System Clock (Tcy)
    CCP2CON1Lbits.TMRPS = 0b00;     // 1:1 Prescaler
    CCP2CON1Lbits.T32 = 0b0;        // 16 bit Operation
    CCP2CON1Lbits.CCSEL = 0b0;      // Output Capture Mode
    CCP2CON1Lbits.CCPMOD = 0b0010;  // Output Low on Compare -> High to Low after compare

    CCP2CON2Hbits.OCAEN = 0b1;      // Enable desired output signals (OC1A) 
    CCP2STATLbits.CCPTRIG = 0b1;    // Timer has been triggered and is running
       
    return;
}
