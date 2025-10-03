#define INA_ADDR 0b10000000     // 7 MSB correspond to address (check p.14 on INA datasheet), last bit will be set or cleared depending on operation (R/~W)
#define INA_CAL_ADDR 0x05       // INA calibration register address
#define INA_CALIBRATION 0x68DC  // no value for now. Need to make calculations (check p.12 on INA datasheet)
#define INA_POWER 0x03          // register to read power from ina
#define INA_CURRENT 0X04        // register to read current from ina
#define INA_VOLTAGE 0X02        // register to read current from ina

void I2C_init();
uint16_t I2C_Read_INA(uint8_t slave_addr, uint8_t register_addr);
void I2C_Calibrate_INA(uint8_t slave_addr, uint16_t cal);