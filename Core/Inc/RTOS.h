/*
 * RTOS.h
 *
 *  Created on: Apr 1, 2025
 *      Author: suriya.prakash
 */

#ifndef INC_RTOS_H_
#define INC_RTOS_H_
#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t LCD_Q;

typedef struct
{
    char message[32];
    int symbol;
} LCDMessage_t;

void LCD_Task(void *pvParameters);

#endif /* INC_RTOS_H_ */
