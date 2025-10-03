/*
 * File:   main.c
 * Author: Utilizador
 *
 * Created on 2 de Outubro de 2025, 16:28
 */

#include "sensor_node.h"

void init_all()
{
    SPI1Init();
    I2C_init();
    I2C_Calibrate_INA(INA_ADDR, INA_CALIBRATION);
    PWM_init(1, 0);
    PWM_init(2, 0);
    PWM_init(3, 0);
}

int main(void) {
    init_all();
    uint16_t reading_raw[3];
    
    PWM_updDcyc(1, 75);
    
    while(1){
        reading_raw[0] = I2C_Read_INA(INA_ADDR, INA_POWER);
        reading_raw[1] = I2C_Read_INA(INA_ADDR, INA_CURRENT);
        reading_raw[2] = I2C_Read_INA(INA_ADDR, INA_VOLTAGE);
        //for(uint8_t i = 0; i<3 ; i++){
        //tx_data[i+1] = reading_raw[i];
        //}
    }
    
    return 0;
}