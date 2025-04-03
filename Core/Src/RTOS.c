/*
 * RTOS.c
 *
 *  Created on: Apr 1, 2025
 *      Author: suriya.prakash
 */


#include "FreeRTOS.h"
#include "CustomerApp.h"
#include "queue.h"
#include "task.h"
#include "RTOS.h"

QueueHandle_t LCD_Q;


void LCD_Task(void *pvParameters)
{
    LCDMessage_t receivedMessage;

    while (1)
    {
        if (xQueueReceive(LCD_Q, &receivedMessage, portMAX_DELAY) == pdPASS)
        {
            printLCD(0, receivedMessage.message, receivedMessage.symbol);
        }
    }
}
void LoadCell(void *Tare_Flag)
{

}


void RTOS_Task(void)
{

}
