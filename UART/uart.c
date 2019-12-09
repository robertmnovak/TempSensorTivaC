/*
 * uart.c
 *
 *  Created on: Nov 29, 2019
 *      Author: Robert
 */

#include "uart.h"
#include <stdbool.h>
#include <string.h>
char txChar;

/**************************************************************************************
 * UART0 initialization Function
 * This function initializes UART0 which is used to communicate with the PC
 *
 ***************************************************************************************
*/
void UART0_Init(void){
    //Enable UART0 module
    SYSCTL->RCGCUART |= (1<<0);

    //Enables Clock for GPIOA Module
    SYSCTL->RCGCGPIO |= (1<<0);

    /*
     * The next 2 lines are for defining the specific peripheral to the pins PA0 and PA1
     * Alternative Function Select (AFSEL) Page 671 of TM4C123GH6PM Datasheet
     * GPIO Port Control (PCTL) Page 688 of TM4C123GH6PM Datasheet
     */
    GPIOA->AFSEL |= (1<<1)|(1<<0);
    GPIOA->PCTL |= (1<<0)|(1<<4);

    //Enables the digital function of pins PA0 and PA1
    GPIOA->DEN |= (1<<0)|(1<<1);

    //Disabling UART module to write UART configuration
    UART0->CTL &= ~(1<<0);

    //Setting Baud Rate to 115200
    UART0->IBRD = 8;//8
    UART0->FBRD = 44;//44

    //Setting word length to 8 bits
    UART0->LCRH = (0x3<<5);

    //Setting UART clock source to system clock
    UART0->CC = 0x0;

    //Enable UART module and enable it for Transmit and Recieve
    UART0->CTL = (1<<0)|(1<<8)|(1<<9);
}

/**************************************************************************************
 * UART3 initialization Function
 * This function initializes UART0 which is used to Transmit data through the Bluetooth
 * module
 ***************************************************************************************
*/
void UART3_Init(void){
    //Enable UART3 module
    SYSCTL->RCGCUART |= (1<<3);

    //Enables Clock for GPIOC Module
    SYSCTL->RCGCGPIO |= (1<<2);

    /*
     * The next 2 lines are for defining the specific peripheral to the pins PA0 and PA1
     * Alternative Function Select (AFSEL) Page 671 of TM4C123GH6PM Datasheet
     * GPIO Port Control (PCTL) Page 688 of TM4C123GH6PM Datasheet
     */
    GPIOC->AFSEL |= (1<<6)|(1<<7);
    GPIOC->PCTL |= (1<<24)|(1<<28);

    //Enables the digital function of pins PA0 and PA1
    GPIOC->DEN |= (1<<6)|(1<<7);

    //Disabling UART module to write UART configuration
    UART3->CTL &= ~(1<<0);

    //Setting Baud Rate to 9600
    UART3->IBRD = 104;
    UART3->FBRD = 11;

    //Setting word length to 8 bits
    UART3->LCRH = (0x3<<5);

    //Setting UART3 clock source to system clock
    UART3->CC = 0x0;

    //Enabling UART Module and Enabling it to Transmit
    UART3->CTL = (1<<0)|(1<<8);
}


/**************************************************************************************
 * Print a char to UART function. This is used for writing data to the UART
 * Before printing char it polls the UART flag register's(UART->FR) 5th bit to determine
 * if the UART transmit FIFO is full or not. Once empty, it fills it with the next
 * char
 ***************************************************************************************
*/
void printCharToUart(char c, UART0_Type *UARTtemp){
    while((UARTtemp->FR & (1<<5)));
    UARTtemp->DR = c;
}


/**************************************************************************************
 * Print string to UART function. This scans the given string pointer and will
 * continue to print a char at a time until the string is empty.
 *
 ***************************************************************************************
*/
void printStringToUart(char * string, UART0_Type *UARTtemp){
    while(*string){
        printCharToUart(*(string++), UARTtemp);
    }
}
