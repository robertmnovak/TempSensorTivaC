/*
 * uart.h
 *
 *  Created on: Nov 29, 2019
 *      Author: Robert
 */

#ifndef UART_H_
#define UART_H_
#include "BSP\bsp.h"

void UART0_Init(void);
void UART2_Init(void);
void UART3_Init(void);
void printCharToUart(char c, UART0_Type *UARTtemp);
void printStringToUart(char * string, UART0_Type *UARTtemp);
#endif /* UART_H_ */
