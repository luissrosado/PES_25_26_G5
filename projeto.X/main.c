/*
 * File:   main.c
 * Author: guilh
 *
 * Created on October 1, 2025, 4:42 PM
 */
// spi pode-se mandar 3 mensagens com dados válidos primeiro envio sao bytes dummies
// master vai apenas "cumprimentar"
// alterar tx_data a partir da posição 1 até posição 3

#include "xc.h"
#include "i2c.h"
void init_all()
{
    SPI1Init();
    I2C_init();
    I2C_Calibrate_INA(INA_ADDR, INA_CALIBRATION);
}

int main(void) {
    init_all();
    uint16_t reading_raw[3];
    
    while(1){
        reading_raw[0] = I2C_Read_INA(INA_ADDR, INA_POWER);
        reading_raw[1] = I2C_Read_INA(INA_ADDR, INA_CURRENT);
        reading_raw[2] = I2C_Read_INA(INA_ADDR, INA_VOLTAGE);
        for(uint8_t i = 0; i<3 ; i++){
        tx_data[i+1] = reading_raw[i];
        }
    }
    return 0;
}
