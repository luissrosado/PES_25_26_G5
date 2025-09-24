/*
 * File:   main.c
 * Author: rodis
 *
 * Created on 22 de Setembro de 2025, 18:46
 */


#define FOSC 8000000UL
#define FCY FOSC/2

#include <stdint.h>
#include <libpic30.h>
#include "p24FJ256GA702.h"
#include "config_bit.h"
#include "xc.h"

void timer1_setup(uint16_t period)
{
    T1CON = 0x00;   //Stops the Timer1 and reset control register
    TMR1 = 0x00;    //Clear contents of the timer register
    PR1 = period;   //Load the timer with the target period
    
    T1CONbits.TCKPS = 0b11; //Set pre-scaler 1:256
    IPC0bits.T1IP = 0b111;  //Define the Timer1 interrupt priority (in this case highest priority)
    IFS0bits.T1IF = 0;      //Clear Timer1 interrupt flag
    IEC0bits.T1IE = 1;      //Enable the timer interrupt
    INTCON2bits.GIE = 1;    //Globally enable interrupts (in case they were disabled)
    
    T1CONbits.TON = 1;      //Start the timer
    
    return;
}

void __attribute__((__interrupt__, __shadow__)) _T1Interrupt(void)
{
    TMR1 = 0x00;        //Reset the timer, restarting count
    IFS0bits.T1IF = 0;  //Reset Timer1 interrupt flag
    LATBbits.LATB6 = ~LATBbits.LATB6;   //Toggle LED
}

int main(void) {
    //ANSELBbits.ANSB6 = 0;
    TRISBbits.TRISB6 = 0;
    LATBbits.LATB6 = 0;
    
    uint32_t timer_period_ms = 1000;
            
    timer1_setup((timer_period_ms/1000)*FCY/256);  //Check if conversion is correct
    
    while(1)
    {
        Idle();
    }
    return 0;
}
