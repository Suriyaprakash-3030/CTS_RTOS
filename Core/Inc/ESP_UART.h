/*
 * ESP_UART.h
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#ifndef ESP_UART_H
#define ESP_UART_H

#include <main.h>

/* Buffer to store incoming UART data */
extern char ESP_UART_RxBuffer[30];

/* Send character data over UART4 without a new line */
void ESP_UART_Send(const char* Data);

/* Send character data over UART4 with a new line */
void ESP_UART_Sendln(const char* Data);

/* Send numeric data over UART4 with a new line */
void ESP_UART_SendlnINT(int Data);

/* Send numeric data over UART4 without a new line */
void ESP_UART_SendINT(int Data);

/* Receive data from UART4 and store it in the buffer */
void ESP_UART_Receive(void);

/* Clear the UART4 receive buffer */
void ClearESP_UART_RxBuffer(void);

#endif // ESP_UART_H

