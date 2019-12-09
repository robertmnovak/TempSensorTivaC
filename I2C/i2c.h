/*
 * i2c.h
 *
 *  Created on: Nov 12, 2019
 *      Author: Robert
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "BSP\TM4C123GH6PM.h"



void        writeByte(uint8_t dataByte, uint8_t conditions);
void        I2C_init(void);
void        I2C_Wait(void);
void        I2C_Write(uint8_t slaveAddress, uint8_t *dataByte, uint8_t numberOfBytes);
void        setSlaveAddress(uint8_t slaveAddress, uint8_t mode);
uint8_t     I2C_Read8(uint8_t slaveAddress, uint8_t regAddress);
int16_t     readS16(uint8_t slaveAddress, uint8_t reg);
uint16_t    I2C_Read16(uint8_t slaveAddress, uint8_t regAddress);
uint16_t    read16_Reverse(uint8_t slaveAddress, uint8_t reg);
int16_t     readS16_Reverse(uint8_t slaveAddress, uint8_t reg);
uint32_t    I2C_Read24(uint8_t slaveAddress, uint8_t regAddress);
int32_t     readS24(uint8_t slaveAddress, uint8_t reg);
uint8_t     readByte(uint8_t conditions);

#endif /* I2C_H_ */
