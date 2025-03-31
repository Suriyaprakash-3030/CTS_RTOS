/*
 * SerialHandler.c
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */


#include <SerialHandler.h>

extern UART_HandleTypeDef huart1;


/*Print character data without the new line at the end*/
void SerialPrint(const char* Data)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)Data, strlen(Data), HAL_MAX_DELAY);
}

/*Print character data with the new line at the end*/
void SerialPrintln(const char* Data)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)Data, strlen(Data), HAL_MAX_DELAY);
    const char newline[2] = "\r\n"; // Newline characters for UART
    HAL_UART_Transmit(&huart1, (uint8_t*)newline, 2, HAL_MAX_DELAY);
}

/*Print Numeric data with the new line at the end*/
void SerialPrintlnINT(int Data)
{
	/*Here the UART Buffer is being used to store the numeric values and present then as character array*/
	char buffer[10];
	sprintf(buffer, "%d", Data);
	SerialPrintln(buffer);
}

/*Print Numeric data without the new line at the end*/
void SerialPrintINT(int Data)
{
	/*Here the UART Buffer is being used to store the numeric values and present then as character array*/
	char buffer[10];
	sprintf(buffer, "%d", Data);
	SerialPrint(buffer);
}

/* Print double data with a newline at the end */
void SerialPrintlnDOUBLE(double Data)
{
    /* Use a buffer to store the double value as a character array */
    char buffer[20];
    sprintf(buffer, "%.8f", Data); // Adjust precision as needed
    SerialPrintln(buffer);
}

/* Print double data without a newline at the end */
void SerialPrintDOUBLE(double Data)
{
    /* Use a buffer to store the double value as a character array */
    char buffer[20];
    sprintf(buffer, "%.8f", Data); // Adjust precision as needed
    SerialPrint(buffer);
}
