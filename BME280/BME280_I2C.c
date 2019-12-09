/*
 * BME280_I2C.c
 *
 *  Created on: Nov 20, 2019
 *      Author: Robert
 */

#include "BME280_I2C.h"


/**************************************************************************************
 * BME280 initialization Function
 * This function sets up configurations for the BME280
 * BME280_REGISTER_CONTROLHUMID: 0x01 = oversampling for humidity
 * BME280_REGISTER_CONTROL: 0x27 sets oversampling for temp, pressure, and sensor mode
 ***************************************************************************************
*/
void BME280_Init(void){
    BME280_I2C_readSensorCoefficients();

    uint8_t initVar[4] = {BME280_REGISTER_CONTROLHUMID, 0x01, BME280_REGISTER_CONTROL, 0x27};
    I2C_Write(BME280_ADDRESS, initVar, 4);
}

/**************************************************************************************
 * BME280 Reading calibration coefficients
 * This functions reads preset calibration coefficients that can be different in
 * each device. These are used to convert temperature and humidity into
 * readable data
 ***************************************************************************************
*/
void BME280_I2C_readSensorCoefficients(void){
    cal_data.dig_T1 = read16_Reverse(BME280_ADDRESS, BME280_DIG_T1_REG);
    cal_data.dig_T2 = readS16_Reverse(BME280_ADDRESS, BME280_DIG_T2_REG);
    cal_data.dig_T3 = readS16_Reverse(BME280_ADDRESS, BME280_DIG_T3_REG);
    cal_data.dig_H1 = I2C_Read8(BME280_ADDRESS, BME280_DIG_H1_REG);
    cal_data.dig_H2 = readS16_Reverse(BME280_ADDRESS, BME280_DIG_H2_REG);
    cal_data.dig_H3 = I2C_Read8(BME280_ADDRESS,BME280_DIG_H3_REG);
    cal_data.dig_H4 = (I2C_Read8(BME280_ADDRESS, BME280_DIG_H4_REG) << 4) | (I2C_Read8(BME280_ADDRESS, BME280_DIG_H4_REG+1) & 0xF);
    cal_data.dig_H5 = (I2C_Read8(BME280_ADDRESS, BME280_DIG_H5_REG+1) << 4) | (I2C_Read8(BME280_ADDRESS, BME280_DIG_H5_REG) >> 4);
    cal_data.dig_H6 = (int8_t)I2C_Read8(BME280_ADDRESS, BME280_DIG_H6_REG);

}

/**************************************************************************************
 * BME280 Read Temperature Function
 * This function was given in the BME280 datasheet page 23
 ***************************************************************************************
*/
void BME280_I2C_readTemperature(void){
    adc_T = I2C_Read24(BME280_ADDRESS, BME280_REGISTER_TEMPDATA);
    adc_T >>= 4;

    var1  = ((((adc_T>>3) - ((int64_t)cal_data.dig_T1 <<1))) * ((int64_t)cal_data.dig_T2)) >> 11;
    var2  = ((((((adc_T>>4) - ((int64_t)cal_data.dig_T1)) * ((adc_T>>4) - ((int64_t)cal_data.dig_T1))) >> 12) * ((int64_t)cal_data.dig_T3)) >> 14);
    t_fine = var1 + var2;


    temperature  = (t_fine * 5 + 128) >> 8;
    temperatureF = ((temperature / 100)* 1.8 + 32);

}

/**************************************************************************************
 * BME280 Read Humidity Function
 * This function was given in the BME280 datasheet page 50
 ***************************************************************************************
*/
void BME280_I2C_readHumidity(void){
    adc_H = I2C_Read16(BME280_ADDRESS, BME280_REGISTER_HUMIDDATA);
    humidity = (((double)t_fine) - 76800.0);
    humidity = (adc_H - (((double)cal_data.dig_H4) * 64.0 + ((double)cal_data.dig_H5) / 16384.0 * humidity))*(((double)cal_data.dig_H2) / 65536.0 * (1.0 + ((double)cal_data.dig_H6) / 67108864.0 * humidity *(1.0 + ((double)cal_data.dig_H3) / 67018864.0 * humidity)));
    humidity = humidity * (1.0 - ((double)cal_data.dig_H1) * humidity / 524288.0);
    if(humidity > 100.0) {
        humidity = 100.0;
    } else if(humidity < 0.0){
        humidity = 0.0;
    }
}

