/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SENSOR_NODE_H
#define	SENSOR_NODE_H


#define FOSC 8000000UL
#define FCY FOSC/2

#include "config_bits.h"
#include <xc.h> // include processor files - each processor file is guarded.  

#include <libpic30.h>
#include "p24FJ256GA702.h"
#include <stdint.h>

// TODO Insert C++ class definitions if appropriate

#define INA_ADDR 0b10000000     // 7 MSB correspond to address (check p.14 on INA datasheet), last bit will be set or cleared depending on operation (R/~W)
#define INA_CAL_ADDR 0x05       // INA calibration register address
#define INA_CALIBRATION 0x68DC  // no value for now. Need to make calculations (check p.12 on INA datasheet)
#define INA_POWER 0x03          // register to read power from ina
#define INA_CURRENT 0X04        // register to read current from ina
#define INA_VOLTAGE 0X02        // register to read current from ina

#define PWM_FREQ 31300UL            // PWM frequency defined as 31.3 kHz

#define SPI_SS_ANS       ANSELBbits.ANSB3 
#define SPI_SS_TRIS      TRISBbits.TRISB3
#define SPI_SS_PORT      PORTBbits.RB3
#define LED_TRIS         TRISBbits.TRISB14
#define LED_LAT          LATBbits.LATB14


void I2C_init();
uint16_t I2C_Read_INA(uint8_t slave_addr, uint8_t register_addr);
void I2C_Calibrate_INA(uint8_t slave_addr, uint16_t cal);

void timer1_setup(uint16_t period);
void __attribute__((__interrupt__, __shadow__)) _T1Interrupt(void);

void PWM_init(uint16_t channel, uint16_t dcyc);
void PWM_updDcyc(uint16_t channel, uint16_t dcyc);

void SPI1Init(void);
void __attribute__((interrupt, no_auto_psv)) _SPI1RXInterrupt(void);

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif SENSOR_NODE_H

