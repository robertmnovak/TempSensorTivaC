// SSD1306 128x64 OLED screen using I2C
// Datasheet: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

#include <OLED\font_6x8.h>
#include "SSD1306_I2C_TivaC.h"

/**************************************************************************************
 * SSD1306 send command function
 * To write commands to SSD1306 you need to send a value of 0x80 before each command
 ***************************************************************************************
*/
void SSD_command(unsigned char command){
    uint8_t ptr[2] = {0x80, command};
    I2C_Write(SSD_ADDRESS, ptr, 2);
}

/**************************************************************************************
 * SSD1306 initialization Function
 * This function initializes SSD1306
 ***************************************************************************************
*/
unsigned char ssdInit[26] =
{
    SSD_DISPLAYOFF,
    SSD_SETDISPLAYCLOCKDIV,
    0x80,
    SSD_SETMULTIPLEX,
    0xBF,
    SSD_SETDISPLAYOFFSET,
    0x0,
    (SSD_SETSTARTLINE | 0x0),
    SSD_CHARGEPUMP,
    0x14,
    SSD_MEMORYMODE,
    0x20,
    (SSD_SEGREMAP | 0x1),
    SSD_COMSCANDEC,
    SSD_SETCOMPINS,
    0x12,
    SSD_SETCONTRAST,
    0xCF,
    SSD_SETPRECHARGE,
    0xF1,
    SSD_SETVCOMDETECT,
    0x40,
    SSD_DISPLAYALLON_RESUME,
    SSD_NORMALDISPLAY,
    SSD_DEACTIVATE_SCROLL,
    SSD_DISPLAYON

};

void SSD_init(void) {
    setSlaveAddress(SSD_ADDRESS, 0);
    uint8_t i;
    for(i = 0; i < 26 ; i++){
        SSD_command(ssdInit[i]);
    }
    SSD_clearScreen();
    SSD_setPosition(0,0);
}


/**************************************************************************************
 * SSD1306 Set Position Function
 * This function sets the position for cursor. Will be used in print text function so
 * that you can specify what row and column to start printing text
 ***************************************************************************************
*/
void SSD_setPosition(uint8_t column, uint8_t page) {
    uint8_t sendCommand[6];
    sendCommand[0] = 0x80;
    //Register Address for Column
    sendCommand[1] = SSD_COLUMNADDR;
    sendCommand[2] = 0x80;
    //Column start
    sendCommand[3] = column;
    sendCommand[4] = 0x80;
    //Column End
    sendCommand[5] = SSD_LCDWIDTH-1;

    I2C_Write(SSD_ADDRESS, sendCommand, 6);
    sendCommand[0] = 0x80;
    //Register Address for Page
    sendCommand[1] = SSD_PAGEADDR;
    sendCommand[2] = 0x80;
    //Page start
    sendCommand[3] = page;
    sendCommand[4] = 0x80;
    //Page end
    sendCommand[5] = 7;
    I2C_Write(SSD_ADDRESS, sendCommand, 6);
}

/**************************************************************************************
 * SSD1306 Print Text Function
 * Given a string it will print the string onto the display using the font_6x8 library
 * Each character is 6 segments long or x+6 in this situation. This function will print
 * a segment at a time. Each segment is 8 bits long, LSB at top and MSB at bottom.
 ***************************************************************************************
*/
void SSD_printText_6x8(uint8_t x, uint8_t y, char *strPtr) {
    uint8_t dataBuffer[8];
    SSD_setPosition(x, y);
    while (*strPtr) {
        //0x40 command is what tells the SSD you are writing to the screen
        dataBuffer[0] = 0x40;
        uint8_t i;
        for(i = 0; i< 6; i++) {
            dataBuffer[i+1] = font_6x8[((*strPtr - ' ')*6)+i];
        }
        dataBuffer[7] = 0x0; //Prints space after letter
        setSlaveAddress(SSD_ADDRESS, 0);

        /*
         * This loop writes the dataBuffer array to each bit in the current segment
         * Cases to watch out for are start and stop (i == 7 and i == 0), in which
         * you need to write specific start and stop commands to.
         */
        for(int i = 0 ; i < 8 ; i++){
            if(i == 7){
                writeByte(dataBuffer[i],(1<<0)|(1<<2));
            } else if(i == 0){
                writeByte(dataBuffer[i],(1<<0)|(1<<1));
            } else {
                writeByte(dataBuffer[i],(1<<0));
            }
        }
        strPtr++;
        x+=7;
    }
}

/**************************************************************************************
 * SSD Clear Screen Function
 * This function scrolls through the screen and clears it one row at a time
 ***************************************************************************************
*/
void SSD_clearScreen(void){
    uint8_t i = 0, j = 0;
    setSlaveAddress(SSD_ADDRESS, 0);
    for(j = 0; j <= SSD_MAX_PAGE_NUMBER; j++){
        SSD_setPosition(0, j);
        writeByte(0x40, (1<<0)|(1<<1));
        for(i = 0; i <= SSD_MAX_COLUMN_NUMBER ; i++){
            writeByte(0x00, (1<<0));
        }
        writeByte(0x00, (1<<0)|(1<<2));
    }
}

