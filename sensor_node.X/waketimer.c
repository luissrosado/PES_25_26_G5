/*
 * File:   waketimer.c
 * Author: Guilherme Garcia
 *         Rodrigo Caldeira
 *         Francisco Jesus
 *
 * Created on October 1, 2025, 4:35 PM
 */

#include "sensor_node.h"

void timer1_setup(uint16_t period)
{
    T1CON = 0x00;   //Stops the Timer1 and reset control reg.
    TMR1 = 0x00;    //Clear contents of the timer register
    PR1 = period;   //Load the timer with the target period
    
    IPC0bits.T1IP = 0b111;  //Define the Timer1 interrupt priority (in this case highest priority)
    IFS0bits.T1IF = 0;      //Clear Timer1 interrupt flag
    IEC0bits.T1IE = 1;      //Enable the timer interrupt
    INTCON2bits.GIE = 1;    //Globally enable interrupts (in case they were disabled)
    
    T1CONbits.TON = 1;      //Start the timer
    
    return;
}

void __attribute__((__interrupt__, __shadow__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;  //Reset Timer1 interrupt flag
    TMR1 = 0x00;        //Reset the timer, restarting count
    LATBbits.LATB3 = ~LATBbits.LATB3;   //Toggle LED
}
