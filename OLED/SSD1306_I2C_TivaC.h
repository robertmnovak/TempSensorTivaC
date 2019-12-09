#ifndef SSD1306_I2C_TIVAC_H_
#define SSD1306_I2C_TIVAC_H_

#include "BSP\bsp.h"
#include "I2C\i2c.h"

void SSD_setPosition(uint8_t column, uint8_t page);
void SSD_command(unsigned char command);
void SSD_init(void);
void SSD_clearScreen(void);
void SSD_printText_6x8(uint8_t x, uint8_t y, char *strPtr);

#define SSD_ADDRESS                 0x3C
#define SSD_LCDWIDTH                128
#define SSD_LCDHEIGHT               64
#define SSD_SETCONTRAST             0x81
#define SSD_DISPLAYALLON_RESUME     0xA4
#define SSD_DISPLAYALLON            0xA5
#define SSD_NORMALDISPLAY           0xA6
#define SSD_INVERTDISPLAY           0xA7
#define SSD_DISPLAYOFF              0xAE
#define SSD_DISPLAYON               0xAF
#define SSD_SETDISPLAYOFFSET        0xD3
#define SSD_SETCOMPINS              0xDA
#define SSD_SETVCOMDETECT           0xDB
#define SSD_SETDISPLAYCLOCKDIV      0xD5
#define SSD_SETPRECHARGE            0xD9
#define SSD_SETMULTIPLEX            0xA8
#define SSD_SETLOWCOLUMN            0x00
#define SSD_SETHIGHCOLUMN           0x10
#define SSD_SETSTARTLINE            0x40
#define SSD_MEMORYMODE              0x20
#define SSD_COLUMNADDR              0x21
#define SSD_PAGEADDR                0x22
#define SSD_COMSCANINC              0xC0
#define SSD_COMSCANDEC              0xC8
#define SSD_SEGREMAP                0xA0
#define SSD_CHARGEPUMP              0x8D
#define SSD_EXTERNALVCC             0x1
#define SSD_SWITCHCAPVCC            0x2
#define SSD_DEACTIVATE_SCROLL       0x2E
#define SSD_MAX_PAGE_NUMBER         7
#define SSD_MAX_COLUMN_NUMBER       127

#endif /* SD1306_I2C_TIVAC_H_ */
