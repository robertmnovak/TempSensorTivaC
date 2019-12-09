/*
 * I2C driver including read(1, 2, and 3 bytes), write(single and multiple bytes)
 * Created on: Nov 12, 2019
 * Author: Robert Novak
 */
#include "i2c.h"

/**************************************************************************************
 * I2C1 initialization Function
 * This function initializes I2C1 to be used with pins PA6(SCL) and PA7(SDA)
 ***************************************************************************************
*/
void I2C_init(void){
    //Enable I2C1 Module Clock
    SYSCTL->RCGCI2C |= (1<<1);
    //Enable clock for GPIOA
    SYSCTL->RCGCGPIO |= (1<<0);

    /*
    * The next 2 lines are for defining the specific peripheral to the pins PA6 and PA7
    * Alternative Function Select (AFSEL) Page 671 of TM4C123GH6PM Datasheet
    * GPIO Port Control (PCTL) Page 688 of TM4C123GH6PM Datasheet
    */
    GPIOA->AFSEL |= (1<<6)|(1<<7);
    GPIOA->PCTL |= (3<<28)|(3<<24);

    //Enables the digital function of pins PA6 and PA7
    GPIOA->DEN |= (1<<6)|(1<<7);
    GPIOA->ODR = (1<<7);

    //Initalize the I2C Master
    I2C1->MCR = (1<<4);

    /*
     * Set desired SCL clock speed of 100 Kbps by writing the value from the formula into
     * the MTPR register
     *  TPR = (System Clock/(2*(SCL_LP + SCL_HP)*SCL_CLK))-1
     *  TPR = (16MHz / 2*(6+4)*100000))-1
     *  TPR = 7
     */
    I2C1->MTPR = (7<<0);
}

/**************************************************************************************
 * I2C Set Slave Address Function
 * Sets the given given slave address and either clears or sets the lsb which tells
 * I2C which mode it should be in(either read or write)
 ***************************************************************************************
*/
void setSlaveAddress(uint8_t slaveAddress, uint8_t mode){
    if(mode == 0){
        I2C1->MSA = ((slaveAddress<<1) & ~(1<<0)); //write mode
    } else {
        I2C1->MSA = ((slaveAddress<<1) | (1<<0)); //read mode
    }
}


/**************************************************************************************
* I2C Wait Function
* This function polls MCS register and waits for the controller and the I2C module
* to not be busy
***************************************************************************************
*/
void I2C_Wait(void){
    while((I2C1->MCS & (1<<6)) == 1);
    while((I2C1->MCS & (1<<0)) != 0);
}

/**************************************************************************************
* I2C Read Byte Function
* Following the flow charts on page 1008 of the TM4C123GH6PM Datasheet
* This function sets conditions(stop, start, run, ack) then waits for the controller
* and I2C module to not be busy. Then returns the data.
 ***************************************************************************************
*/
uint8_t readByte(uint8_t conditions){
    uint8_t tempRead;

    I2C1->MCS = conditions;
    I2C_Wait();

    tempRead = I2C1->MDR;
    return tempRead;
}

/**************************************************************************************
* I2C Write Byte Function
* Following the flow chart on page 1008 of the TM4C123GH6PM Datasheet
* This function writes a byte of data, then sends the conditions(stop, start, run, ack)
***************************************************************************************
*/
void writeByte(uint8_t dataByte, uint8_t conditions){
    I2C1->MDR = dataByte;

    I2C1->MCS = conditions;
    I2C_Wait();
}


/**************************************************************************************
 * I2C Read 1 byte function
 * To read a byte at a specific register address you first need to write 1 byte to
 * the slave address telling it which address you want to read from, then you can switch
 * the device to read mode and read the byte
 ***************************************************************************************
*/
uint8_t I2C_Read8(uint8_t slaveAddress, uint8_t regAddress){
    uint8_t tempRead;
    setSlaveAddress(slaveAddress, 0);
    writeByte(regAddress, ((1<<0)|(1<<1)|(1<<2)));
    setSlaveAddress(slaveAddress, 1);
    tempRead = readByte((1<<0)|(1<<1)|(1<<2));
    return tempRead;
}

/**************************************************************************************
 * I2C Read 2 bytes function
 * To read at a specific register address you first need to write 1 byte to
 * the slave address telling it which address you want to read from, then you can switch
 * the device to read mode and read the bytes. Temp read needs to be left shifted by
 * 8 bits to store the next 8 bits in the LSB
***************************************************************************************
*/
uint16_t I2C_Read16(uint8_t slaveAddress, uint8_t regAddress){
    uint16_t tempRead;
    setSlaveAddress(slaveAddress, 0);
    writeByte(regAddress, ((1<<0)|(1<<1)|(1<<2)));

    setSlaveAddress(slaveAddress, 1);

    tempRead = readByte((1<<0)|(1<<1)|(1<<3));
    tempRead <<= 8;
    tempRead |= readByte((1<<0)|(1<<2));
    return tempRead;
}

/**************************************************************************************
* I2C Read Reverse 2 Bytes Function
* This is used with BME280 sensor since to read the calibrated data, some of the data
* is stored in registers with their LSB being read first. This function reads the two
* bytes then reverse them.
 ***************************************************************************************
*/
uint16_t read16_Reverse(uint8_t slaveAddress, uint8_t reg) {

    uint16_t tempRead = I2C_Read16(slaveAddress, reg);

    return ((tempRead >> 8) | (tempRead << 8));
}

/**************************************************************************************
* I2C Read 2 Signed Bytes
* Based on our read 2 Bytes, this function returns a signed value by type casting
 ***************************************************************************************
*/
int16_t readS16(uint8_t slaveAddress, uint8_t reg){
    return (int16_t)I2C_Read16(slaveAddress, reg);
}

/**************************************************************************************
* I2C read 2 Signed Bytes then reverse
* Similar to above, but the 8 LSB and 8 MSB are swapped. This is due to BME280
* calibration numbers having LSB stored in memory before the MSB
 ***************************************************************************************
*/
int16_t readS16_Reverse(uint8_t slaveAddress, uint8_t reg){
    return (int16_t)read16_Reverse(slaveAddress, reg);
}

/**************************************************************************************
 * I2C Read 3 bytes function
 * To read at a specific register address you first need to write 1 byte to
 * the slave address telling it which address you want to read from, then you can switch
 * the device to read mode and read the bytes. Temp read needs to be left shifted by
 * 8 bits after each read to make room for the next 8 bits.
***************************************************************************************
*/
uint32_t I2C_Read24(uint8_t slaveAddress, uint8_t regAddress){
    uint32_t tempRead;
    setSlaveAddress(slaveAddress, 0);
    writeByte(regAddress, ((1<<0)|(1<<1)|(1<<2)));
    setSlaveAddress(slaveAddress, 1);
    tempRead = readByte((1<<0)|(1<<1)|(1<<3));
    tempRead <<= 8;
    tempRead |= readByte((1<<0)|(1<<3));
    tempRead <<= 8;
    tempRead |= readByte((1<<0)|(1<<2));
    return tempRead;
}

/**************************************************************************************
 * I2C Read 3 Signed Bytes Function
 * This function uses the read 3 bytes function, then returns a typecasted version
 ***************************************************************************************
*/
int32_t readS24(uint8_t slaveAddress, uint8_t reg){
    return (int32_t)I2C_Read24(slaveAddress, reg);
}

/**************************************************************************************
* I2C Write Bytes Function
* This function will write a number of bytes to the slave address. There are three
* cases to look out for when writing the byte.
* 1) First Byte Sent ( i == 0 ): We need to send run and start conditions
* 2) Last byte Sent ( i == (numberofBytes)-1 ): We need to send a run and stop condition
* 3) Else: We need to send only a run condition
* We need to wait for controller and I2C module to not be busy after writing each byte
 ***************************************************************************************
*/
void I2C_Write(uint8_t slaveAddress, uint8_t *dataByte, uint8_t numberOfBytes){
    setSlaveAddress(slaveAddress, 0);

    for(int i = 0; i < numberOfBytes ; i++){
        if(i == 0){
            I2C1->MDR = *dataByte;
            I2C1->MCS = ((1<<0)|(1<<1));
            I2C_Wait();
            dataByte++;
        } else if (i == (numberOfBytes-1)){
            I2C1->MDR = *dataByte;
            I2C1->MCS = ((1<<0)|(1<<2));
            I2C_Wait();
        } else {
            I2C1->MDR = *dataByte;
            I2C1->MCS = (1<<0);
            I2C_Wait();
            dataByte++;
        }
    }
}
