/*
 * LCD.h
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#ifndef LCD_H
#define LCD_H

#include "stm32f4xx_hal.h"
#include <main.h>
#include <string.h>

//LCD defines
#define LCD_ROW_COUNT 4
#define LCD_COLUMN_COUNT 20
#define MAX_STRING_LENGTH 21    // Maximum string length + 1 for null terminator
#define LCD_DEFAULT_CONTRAST 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 100
#define NO_SYMBOL 0
#define ENTER_SYMBOL 1
#define REFRESH_SYMBOL 2
#define CURSOR_SYMBOL 3
#define CURSOR_DEMO_SYMBOL 4
#define MAX_ENTRY_KEYS 7 //The maximum number of characters per entry for the project

extern char lastLCDData[LCD_ROW_COUNT][MAX_STRING_LENGTH];
extern int lastLCDCustom[LCD_COLUMN_COUNT];
extern uint8_t isPowerLEDBlinking; // = FALSE; // Power LED blinking state
extern uint8_t isPowerLEDOn;// = FALSE; // Power LED state
extern uint8_t isScreenLoading;// = FALSE; // Screen loading state
extern char DotString[LCD_COLUMN_COUNT + 1];// = ""; // String for screen loading dots

extern char* LCD_BUF_ARR[4];
extern int LCD_SMILE_ARR[4];
extern int New_LCD_Data[4];

extern uint8_t Custom_Blank_Symbol[8];
extern uint8_t Custom_Enter_Symbol[8];
extern uint8_t Custom_Reclean_Symbol[8];
extern uint8_t Custom_Cursor_Symbol[8];
extern uint8_t Custom_demo_cursor_Symbol[8];

//##############################################################################################//
//Functions for LCD System
/// Functions for LCD System
void LCD_Init(void);
// Initializes the LCD display. This should be called before any other LCD functions.

void LCD_SendCommand(uint8_t cmd);
// Sends a command byte to the LCD. Used for various LCD control commands.

void LCD_SendData(uint8_t data);
// Sends a data byte to the LCD. Used for writing characters to the display.

void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]);
// Creates a custom character in the LCD's CGRAM at the specified location with the given bitmap.

void LCD_SetCursor(uint8_t row, uint8_t col);
// Sets the cursor position on the LCD screen to the specified row and column.

void LCD_Clear(void);
// Clears the LCD display and returns the cursor to the home position (top-left corner).

void LCD_SendHalfByte(uint8_t data);
// Sends half a byte (4 bits) to the LCD. Used internally for 4-bit mode communication.

void LCD_ClearStringBuffer(void);
// Clears the global string buffer by setting all its elements to '\0'.

char* combineStrings(const char* string1, float x);
// Combining a String and a float/integer variable.

void LCD_StoreValueInBuffer(float value);
// Stores a floating-point number into the global string buffer, formatted as a string.

//##############################################################################################//
// Custom Build Functions
void printLCD(int row, char* data, int smiley);
// Displays and store the given string data on the specified row of the LCD and appends a smiley at the end if there is enough space.

void LCDRGBControl(int color);
// Controls the RGB backlight of the LCD (if applicable). The color parameter specifies the color settings.

void retrieveLCDLastData();
// Retrieves the last data or message displayed on the LCD. Specific implementation details may vary.

void powerLEDControl(int state);
// Controls the power state of the LCD's LED backlight. The state parameter typically represents on/off.

void controlLCDContrast(int contrastLevel);
// Adjusts the contrast of the LCD display. The contrastLevel parameter specifies the desired contrast setting.

void printLCD_CMD();

void printLCD_PERFORM(int row, char *data, int smiley);


void printLCDInt(int row, char *data, int number,  int totalCycle );

#endif
