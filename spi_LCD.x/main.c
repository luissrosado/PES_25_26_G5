/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "bsp/adc.h"
#include "bsp/lcd.h"
#include "bsp/timer_1ms.h"
#include "bsp/buttons.h"
#include "bsp/leds.h"
#include "bsp/rtcc.h"
#include "p24FJ1024GB610.h"
 
#define FOSC 8000000UL
#define FCY FOSC/2
#include "io_mapping.h"
#include <libpic30.h>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************
extern void SYS_Initialize ( void ) ;
static void BlinkAliveEventHandler( void );
static void ScreenUpdateEventHandler( void );

static RTCC_DATETIME time;
static RTCC_DATETIME lastTime = {0};
static volatile bool toggleBlinkAlive = false;
static volatile bool allowScreenUpdate = true;

#define MEMCMP_VALUES_IDENTICAL 0

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************


void spi_init()
{
   SPI1CON1Lbits.SPIEN = 0;      // disable the spi peripheral
   IFS0bits.SPI1IF = 0;         //  Clear spi interrupt flag
   IEC0bits.SPI1IE = 0;         //  Disable spi interrupt flag
   SPI1CON1bits.DISSCK = 0;      // Enable SCK signal generation
   SPI1CON1bits.DISSDO = 0;      // Enable sdo signal generation
   SPI1CON1bits.MODE16 = 1;   // Set 16-bit mode
   SPI1CON1bits.SMP = 0;         // Input is sample at mid-bit
   SPI1CON1bits.CKP = 0;         // clock is low when idle
   SPI1CON1bits.CKE = 1;          // Data changes on the falling edge (because idle is on low if ckp = 1 changes this bit)
   SPI1CON1bits.MSTEN = 1;       // Define device as master
   SPI1BRGLbits.BRG = 4;                 // fcy/BRG = freq of spi
   SPI1CON2 = 0;                 // disable frame mode
   RPINR20bits.SDI1R = 26;        // miso config pin to rp26 P11
   RPOR9bits.RP19R = 7;           // mosi config pin to rp19 P12
   RPOR10bits.RP21R = 8;           // clock config pin to rp21 P10
   
   // chip select pin RB1  P24
   ANSELBbits.ANSB1 = 0;
   TRISBbits.TRISB1 = 0;
   
   ANSELGbits.ANSG7 = 0;
   ANSELGbits.ANSG8 = 0;

   
   SPI1CON1Lbits.SPIEN = 1;      // Enable the spi peripheral
}

void SerialInit(){
    U1BRG = 103;//U1BRG = (4000000)/(16*9600) - 1

    //UART SETUP
    U1MODEbits.PDSEL = 0;//No parity, 8 bits
    U1MODEbits.STSEL = 0;//One Stop Bit
    U1MODEbits.UEN = 0;//only U1TX and U1RX
    U1MODEbits.BRGH = 0; //Standard speed mode

    U1STAbits.UTXISEL1 = 0;//tx isr when a write occurs to TX BUF
    U1STAbits.UTXISEL0 = 0;
    
    RPOR8bits.RP17R = 3;// Assign U1TX To Pin RP2 //Pino 50
    RPINR18bits.U1RXR = 10; // Assign U1RX To Pin RP10 //Pino 49
    //RPOR5bits.RP10R = 3;//U1TX -> RP10

    
    U1MODEbits.UARTEN = 1;//enable UART

    U1STAbits.UTXEN = 1;//enable transmission
}

void spi_com(uint16_t data, uint16_t *rx_data)
{
    LATBbits.LATB1 = 0;
    __delay_us(5);// select slave
    SPI1BUFL  = data;                    // write to buffer for Transmit
    while(!SPI1STATLbits.SPIRBF);       // Wait until tx is completed
    rx_data[data] = SPI1BUFL;               // read the received value 
    LATBbits.LATB1 = 1;
}






int main ( void )
{
    uint16_t adcResult;
    uint16_t lastAdcResult = 0xFFFF;
    
    /* Call the System Initialize routine*/
    SYS_Initialize ( );
    
    /* To determine how the LED and Buttons are mapped to the actual board
     * features, please see io_mapping.h. */
    LED_Enable ( LED_BLINK_ALIVE );
    LED_Enable ( LED_BUTTON_PRESSED );

    BUTTON_Enable ( BUTTON_DEMO );
    
    /* Get a timer event once every 100ms for the blink alive. */
    TIMER_SetConfiguration ( TIMER_CONFIGURATION_1MS );
    TIMER_RequestTick( &BlinkAliveEventHandler, 500 );
    TIMER_RequestTick( &ScreenUpdateEventHandler, 170 );
    
    /* The TIMER_1MS configuration should come before the RTCC initialization as
     * there are some processor modules that require the TIMER_1MS module to be
     * configured before the RTCC module, as the RTCC module is emulated using
     * the TIMER_1MS module. */
    time.bcdFormat = false;
    lastTime.bcdFormat = false;
    RTCC_BuildTimeGet( &time );
    RTCC_Initialize( &time );
    memset(&lastTime,0,sizeof(lastTime)); 

    ADC_SetConfiguration ( ADC_CONFIGURATION_DEFAULT );
    ADC_ChannelEnable ( ADC_CHANNEL_POTENTIOMETER );
    
    /* Clear the screen */
    printf( "\f" );   
    
    uint16_t select_message = 0;
    /* 0 -- power, 1 -- corrente, 2 -- tensao*/
    uint16_t rx_data[4];
    char metrics[4];
    metrics[0] = 'w';
    metrics[1] = 'A';
    metrics[2] = 'm';
    metrics[3] = 'V';
    
    spi_init();
    SerialInit();
    
    printf("olaaaaaaa");
    while(1)
    {
        printf("\f");
        printf("Nova medicao");
        LATBbits.LATB1 = 1;
        __delay_ms(1000);
        for(select_message = 0; select_message < 4; select_message++){
            spi_com(select_message, rx_data);
            __delay_ms(3000);
        }
        printf("\f");
        for (uint16_t i = 0; i < 3; i++){
            if(i == 2){ 
                printf("%d %c%c", rx_data[i+1], metrics[i], metrics[i+1]);
            }else{
                printf("%d %c", rx_data[i+1], metrics[i]);
            }
            __delay_ms(1000);
            printf("\f");
        }
    }
    
}

static void BlinkAliveEventHandler(void)
{    
    toggleBlinkAlive = true;
}

static void ScreenUpdateEventHandler(void)
{
    allowScreenUpdate = true;
}

