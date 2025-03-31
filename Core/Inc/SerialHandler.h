/*
 * SerialHandler.h
 *
 *  Created on: Aug 29, 2024
 *      Author: mhmaq
 */

/*SerialHandler.h*/

/*Header file for the Serial i-e UART1, UART3 & UART4*/
#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include <main.h>

/*Print character data without the new line at the end*/
void SerialPrint(const char* Data);

/*Print character data with the new line at the end*/
void SerialPrintln(const char* Data);

/*Print Numeric data with the new line at the end*/
void SerialPrintlnINT(int Data);

/*Print Numeric data without the new line at the end*/
void SerialPrintINT(int Data);

void SerialPrintDOUBLE(double Data);

void SerialPrintlnDOUBLE(double Data);

#endif
