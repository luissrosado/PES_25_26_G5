/*
 * File:   main.c
 * Author: Utilizador
 *
 * Created on 2 de Outubro de 2025, 16:28
 */
#include "config_bits.h"

#include "sensor_node.h"

uint16_t rx_data[4];	// SPI buffer for Receiving
uint16_t tx_data[4] = {0x0000, 0x0000, 0x0000,  0x0000};

void init_all()
{
    OSCCONbits.IOLOCK = 0b0;
 
    SPI1Init();
    I2C_init();
    I2C_Calibrate_INA(INA_ADDR, INA_CALIBRATION);
    PWM_init(1, 0);
    PWM_init(2, 0);
    PWM_init(3, 0);
    
    OSCCONbits.IOLOCK = 0b1;

}



int main(void) {
    
    init_all();
    uint16_t reading_raw[3];

    
    PWM_updDcyc(1, 75);
    
    while(1){
        reading_raw[0] = I2C_Read_INA(INA_ADDR, INA_POWER) * 0.00001526 * 20;
        reading_raw[1] = I2C_Read_INA(INA_ADDR, INA_CURRENT) * 0.00001526;
        reading_raw[2] = (I2C_Read_INA(INA_ADDR, INA_VOLTAGE) >> 3) * 4;
        for(uint8_t i = 0; i<3 ; i++){
        tx_data[i+1] = reading_raw[i];
        }
    }
    
    return 0;
}