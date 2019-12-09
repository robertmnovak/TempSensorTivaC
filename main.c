/**************************************************************************************
 * Temperature + Humidity Sensor with the TM4C123GH6PM
 * Reads data from the BME280 Sensor via I2C and displays the data on an OLED screen
 * via I2C. Also displays data through Bluetooth using the HM-10 module via UART3
 * and to the PC via UART0 when the launchpad is connected through USB. All data is
 * refreshed on displays every 5 seconds based on the SysTick counter.
 *
 * All code is based around CMSIS framework for the TM4C123GH6PM
 * Created on: Nov 21, 2019
 * Author: Robert Novak
 ***************************************************************************************
*/

#include <stdint.h>
#include <stdio.h>
#include "BSP\bsp.h"
#include "OLED\SSD1306_I2C_TivaC.h"
#include "I2C\i2c.h"
#include "BME280\BME280_I2C.h"
#include "UART\uart.h"

void init_Peripherals(void);
void set_OLED_Screen(void);
void print_Info_On_OLED(int hasBeenPrinted);
void parse_Celsius(void);
void parse_Fahrenheit(void);
void parse_Humidity(void);

volatile int intpart;
volatile double decpart;
volatile double tempTemp;
char tempPrint[5];

int main() {
    int printInfoOnce = 0;
    init_Peripherals();
    SysTick_Handler();
    BME280_I2C_readTemperature();
    BME280_I2C_readTemperature();
    set_OLED_Screen();
    while(1){
        if(systemCtr == 5){
            print_Info_On_OLED(printInfoOnce);
            printInfoOnce = 1;
        } else {
            printInfoOnce = 0;
        }
    }
  return 0;
}

/**************************************************************************************
 * Temperature Celsius Parse Function
 * This function breaks the Temperature(Celsius) value up into an array with 2 decimals. This is to
 * feed the array in functions such as SSD_printText_6x8 and printStringToUart
 ***************************************************************************************
*/
void parse_Celsius(void){
    tempTemp = (double)(temperature)/100;
    intpart = (int)tempTemp;
    decpart = tempTemp - intpart;
    tempPrint[1] = (intpart%10)+ '0';
    intpart = intpart - (intpart%10);
    tempPrint[0] = (intpart / 10) + '0';
    tempPrint[2] = '.';
    tempPrint[4] = (char)((int)(decpart*100)%10)+ '0';
    tempPrint[3] = (((int)(decpart*100)-((int)(decpart*100)%10))/10)+ '0';
}

/**************************************************************************************
 * Temperature Fahrenheit Function
 * This function breaks the Temperature(Fahrenheit) value up into an array with 2 decimals. This is to
 * feed the array in functions such as SSD_printText_6x8 and printStringToUart
 ***************************************************************************************
*/
void parse_Fahrenheit(void){
    tempTemp = (double)temperatureF;
    intpart = (int)tempTemp;
    decpart = tempTemp - intpart;
    tempPrint[1] = (intpart%10)+ '0';
    intpart = intpart - (intpart%10);
    tempPrint[0] = (intpart / 10) + '0';
    tempPrint[2] = '.';
    tempPrint[4] = (char)((int)(decpart*100)%10)+ '0';
    tempPrint[3] = (((int)(decpart*100)-((int)(decpart*100)%10))/10)+ '0';
}

/**************************************************************************************
 * Humidity Parse Function
 * This function breaks the humidity value up into an array with 2 decimals. This is to
 * feed the array in functions such as SSD_printText_6x8 and printStringToUart
 ***************************************************************************************
*/
void parse_Humidity(void){
    tempTemp = (double)humidity;
    intpart = (int)tempTemp;
    decpart = tempTemp - intpart;
    tempPrint[1] = (intpart%10)+ '0';
    intpart = intpart - (intpart%10);
    tempPrint[0] = (intpart / 10) + '0';
    tempPrint[2] = '.';
    tempPrint[4] = (char)((int)(decpart*100)%10)+ '0';
    tempPrint[3] = (((int)(decpart*100)-((int)(decpart*100)%10))/10)+ '0';
}

/**************************************************************************************
 * Print Values On OLED Function
 * This function parses each data(Temperature and Humidity) into an array that
 * can then be fed into the SSD_printText function. The purpose of this function
 * is to take the data and display it on all the different displays.
 * 1)OLED display through SSD_printText_6x8
 * 2)Bluetooth via UART3
 * 3)PC via UART0 (When the launchpad is connected via USB to the PC)
 ***************************************************************************************
*/
void print_Info_On_OLED(int hasBeenPrinted){
    if(!hasBeenPrinted){
        BME280_I2C_readHumidity();
        BME280_I2C_readTemperature();
        //Parse out Celcius temperature and print
        parse_Celsius();
        SSD_printText_6x8(35,1, tempPrint);
        printStringToUart("Temp: ", UART3 );
        printStringToUart(tempPrint, UART3);
        printStringToUart("(C) -> ", UART3 );
        printStringToUart("Temperature(C): " , UART0);
        printStringToUart(tempPrint, UART0);
        printStringToUart("     Temperature(F): ", UART0);

        //Parse out Fahrenheit temperature and print
        parse_Fahrenheit();
        SSD_printText_6x8(35,2, tempPrint);
        printStringToUart(tempPrint, UART3);
        printStringToUart("(F)         Humidity: ", UART3);
        printStringToUart(tempPrint, UART0);
        printStringToUart("\n", UART0);

        //Parse out Fahrenheit temperature and print
        parse_Humidity();
        SSD_printText_6x8(35,4, tempPrint);
        printStringToUart(tempPrint, UART3);
        printStringToUart(" %rH\n", UART3);
    }
}

/**************************************************************************************
 * OLED Screen Print Set-up
 * This function lays a quick template on the OLED screen that is then later filled
 * with values every 5 seconds
 ***************************************************************************************
*/
void set_OLED_Screen(void){
    SSD_printText_6x8(0,0, "Temperature");
    SSD_printText_6x8(0,1, "(C): ");
    SSD_printText_6x8(0,2, "(F): ");
    SSD_printText_6x8(0,3, "Humidity");
    SSD_printText_6x8(0,4, "%rH: ");
}

/**************************************************************************************
 * Peripheral Initialize Function
 * This function contains all peripherals I will be need to set-up for use in the Main
 * function. This is used to reduce code in Main()
 ***************************************************************************************
*/
void init_Peripherals(void){
    SysTick_Init();
    __enable_irq();
    I2C_init();
    UART0_Init();
    UART3_Init();
    BME280_Init();
    SSD_init();
}


