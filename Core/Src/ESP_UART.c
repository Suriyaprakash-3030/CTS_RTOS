/*
 * ESP_UART.c
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#include "ESP_UART.h"

extern UART_HandleTypeDef huart4;

/** Buffer to store incoming UART data */
char ESP_UART_RxBuffer[30];

/** Send character data over UART4 without a new line */
void ESP_UART_Send(const char* Data)
{
    HAL_UART_Transmit(&huart4, (uint8_t*)Data, strlen(Data), HAL_MAX_DELAY);
}

/** Send character data over UART4 with a new line */
void ESP_UART_Sendln(const char* Data)
{
    HAL_UART_Transmit(&huart4, (uint8_t*)Data, strlen(Data), HAL_MAX_DELAY);
    const char newline[2] = "\r\n"; // Newline characters for UART
    HAL_UART_Transmit(&huart4, (uint8_t*)newline, 2, HAL_MAX_DELAY);
}

/** Send numeric data over UART4 with a new line */
void ESP_UART_SendlnINT(int Data)
{
    char buffer[10];
    sprintf(buffer, "%d", Data);
    ESP_UART_Sendln(buffer);
}

/** Send numeric data over UART4 without a new line */
void ESP_UART_SendINT(int Data)
{
    char buffer[10];
    sprintf(buffer, "%d", Data);
    ESP_UART_Send(buffer);
}

/** Receive data from UART4 and store it in the buffer */
void ESP_UART_Receive(void)
{
    HAL_UART_Receive(&huart4, (uint8_t*)ESP_UART_RxBuffer, sizeof(ESP_UART_RxBuffer), HAL_MAX_DELAY);
}

/** Clear the UART4 receive buffer */
void ClearUART4_RxBuffer(void)
{
    for (int i = 0; i < sizeof(ESP_UART_RxBuffer); i++)
    {
    	ESP_UART_RxBuffer[i] = '\0';
    }
}

