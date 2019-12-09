/*
 * BME280_I2C.h
 *
 *  Created on: Nov 20, 2019
 *      Author: Robert
 */

#ifndef BME280_I2C_H_
#define BME280_I2C_H_
#include <stdint.h>
#include "BSP\bsp.h"
#include "I2C\i2c.h"

void BME280_I2C_readSensorCoefficients(void);
void BME280_Init(void);
void BME280_I2C_readTemperature(void);
void BME280_I2C_readHumidity(void);

//Define name of BME280 address
#define     BME280_ADDRESS                   0x76

//List of registers needed in code
#define    BME280_DIG_T1_REG                0x88
#define    BME280_DIG_T2_REG                0x8A
#define    BME280_DIG_T3_REG                0x8C

#define    BME280_DIG_H1_REG                0xA1
#define    BME280_DIG_H2_REG                0xE1
#define    BME280_DIG_H3_REG                0xE3
#define    BME280_DIG_H4_REG                0xE4
#define    BME280_DIG_H5_REG                0xE5
#define    BME280_DIG_H6_REG                0xE7

#define    BME280_REGISTER_CONTROLHUMID     0xF2
#define    BME280_REGISTER_CONTROL          0xF4
#define    BME280_REGISTER_TEMPDATA         0xFA
#define    BME280_REGISTER_HUMIDDATA        0xFD

volatile float      tempcal;        // stores the temp offset calibration
int32_t             temperature;    //stors temperature in Celsius
volatile float      temperatureF;   // stores temperature value in fahrenheit
volatile double     humidity;       //stores humidity


/*
 * These values are used in the compenstation functions
 * All are used by functions given by the BME280 datasheet
 */
int64_t             t_fine;
uint64_t            adc_T;
uint64_t            adc_H;
int64_t             var1, var2;

//Struct used to hold values for compensation functions
struct BME280_Calibration_Data
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;

};

struct BME280_Calibration_Data cal_data;
#endif /* BME280_I2C_H_ */
