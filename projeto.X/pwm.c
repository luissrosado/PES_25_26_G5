/*
 * File:   pwm.c
 * Author: Guilherme Garcia
 *         Rodrigo Caldeira
 *         Francisco Jesus
 *
 * Created on October 1, 2025, 4:31 PM
 */


void PWM_init(uint16_t channel, uint16_t dcyc){
    if(dcyc > 100)  dcyc=100;       // avoid malfunctions
    
    OC1RS = 0x007F;                 // Period Register
    OC2RS = 0x007F;                 // Period Register
    OC3RS = 0x007F;                 // Period Register
    
    if(channel == 1){
        OC1CON1 = 0x00;                 // good practice to clear off the control bits initially 
        OC1CON2 = 0x00;                 // ^^^
        // mind that: OC1CON1bits.OCM = 0b000;  <-> channel is disabled

        OC1CON1bits.OCSIDL = 0b0;       // OC1 continues to operate in Idle mode
        OC1CON1bits.OCTSEL = 0b111;     // Peripheral Clock (Fcy) selected
        
        OC1CON2bits.OC32 = 0b0;         // Disable cascading of Two 16b Modules
        OC1CON2bits.OCTRIG = 0b0;       // Syncs OCx with source designated by SYNCSELx bits
        OC1CON2bits.SYNCSEL = 0x1F;     // OC1RS controls the period
        OC1CON2bits.OCTRIS = 0b0;       // OC Peripheral 1 connected to OC1

        PR1 = (FCY / PWM_FREQ) - 1;     // according to datasheet
        OC1R = ((PR1+1)*dcyc)/100;      // Duty cycle register
        OC1CON1bits.OCM = 0b110;        // Edge-Aligned PWM on OC1
        //channel  is enabled
        
    }else if(channel == 2){
        OC2CON1 = 0x00;                 // good practice to clear off the control bits initially 
        OC2CON2 = 0x00;                 // ^^^
        // mind that: OC2CON1bits.OCM = 0b000;  <-> channel is disabled
        
        OC2CON1bits.OCSIDL = 0b0;       // OC1 continues to operate in Idle mode
        OC2CON1bits.OCTSEL = 0b111;     // Peripheral Clock (Fcy) selected
        
        OC2CON2bits.OC32 = 0b0;         // Disable cascading of Two 16b Modules
        OC2CON2bits.OCTRIG = 0b0;       // Syncs OCx with source designated by SYNCSELx bits
        OC2CON2bits.SYNCSEL = 0x1F;     // OC1RS controls the period
        OC2CON2bits.OCTRIS = 0b0;       // OC Peripheral 2 connected to OC2

        PR2 = (FCY / PWM_FREQ) - 1;     // according to datasheet
        OC2R = ((PR2+1)*dcyc)/100;      // Duty cycle register
        OC2CON1bits.OCM = 0b110;        // Edge-Aligned PWM on OC2
        //channel  is enabled
        
    }else if(channel == 3){
        OC3CON1 = 0x00;                 // good practice to clear off the control bits initially 
        OC3CON2 = 0x00;                 // ^^^
        // mind that: OC3CON1bits.OCM = 0b000;  <-> channel is disabled

        OC3CON1bits.OCSIDL = 0b0;       // OC1 continues to operate in Idle mode
        OC3CON1bits.OCTSEL = 0b111;     // Peripheral Clock (Fcy) selected
        
        OC3CON2bits.OC32 = 0b0;         // Disable cascading of Two 16b Modules
        OC3CON2bits.OCTRIG = 0b0;       // Syncs OCx with source designated by SYNCSELx bits
        OC3CON2bits.SYNCSEL = 0x1F;     // OC3RS controls the period
        OC3CON2bits.OCTRIS = 0b0;       // OC Peripheral 3 connected to OC3

        PR3 = (FCY / PWM_FREQ) - 1;     // according to datasheet
        OC3R = ((PR3+1)*dcyc)/100;      // Duty cycle register
        OC3CON1bits.OCM = 0b110;        // Edge-Aligned PWM on OC3
        //channel  is enabled
        
    }else{
        /* criar aqui algum formato de erro tipo
         * um pino ligar-se e um LED acender
         *  */
    }
}

void PWM_updDcyc(uint16_t channel, uint16_t dcyc){
    if(dcyc > 100)  dcyc=100;                   // caps the duty cycle to avoid mistakes
    
    if(channel == 1){
        if(OC1CON1bits.OCM == 0)     OC1CON1bits.OCM = 0b110;   //if(!0)    enables the module
        if(dcyc == 0)   OC1CON1bits.OCM = 0;    //if DC = 0% turns module off
     
        OC1R = ((PR1+1)*dcyc)/100;              // duty cycle is defined, pin is set
    }else if(channel == 2){
        if(OC2CON1bits.OCM == 0)     OC2CON1bits.OCM = 0b110;   //if(!0)    enables the module
        if(dcyc == 0)   OC2CON1bits.OCM = 0;    //if DC = 0% turns module off
      
        OC2R = ((PR2+1)*dcyc)/100;              // duty cycle is defined, pin is set
    }else if(channel == 3){
        if(OC3CON1bits.OCM == 0)     OC3CON1bits.OCM = 0b110;   //if(!0)    enables the module
        if(dcyc == 0)   OC3CON1bits.OCM = 0;    //if DC = 0% turns module off
      
        OC3R = ((PR3+1)*dcyc)/100;              // duty cycle is defined, pin is set
    }else{
        /* criar aqui algum formato de erro tipo
        * um pino ligar-se e um LED acender
        *  */
    }
}
