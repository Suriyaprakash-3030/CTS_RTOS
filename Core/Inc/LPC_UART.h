/*
 * LPC_UART.h
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#ifndef LPC_UART_H
#define LPC_UART_H

#include <main.h>

/* Buffer to store incoming UART data */
extern char LPC_UART_RxBuffer[30];

/* Send character data over UART3 without a new line */
void LPC_UART_Send(const char* Data);

/* Send character data over UART3 with a new line */
void LPC_UART_Sendln(const char* Data);

/* Send numeric data over UART3 with a new line */
void LPC_UART_SendlnINT(int Data);

/* Send numeric data over UART3 without a new line */
void LPC_UART_SendINT(int Data);

/* Receive data from UART3 and store it in the buffer */
void LPC_UART_Receive(void);

/* Clear the UART3 receive buffer */
void ClearLPC_UART_RxBuffer(void);

#endif // TO_LPC_UART_H
