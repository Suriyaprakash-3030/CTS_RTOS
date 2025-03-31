/*
 * Keypad.h
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include <main.h>  // Include if needed for GPIO definitions

//* Keypad Defines
#define BUTTON_POWER_ACTIVE_TIME 1000  //2400 //How many mS to state that power button is press is effective after
#define NUMPAD 1
#define ALL 0
#define CONTROL 2
#define COMMAKEY 10
#define ENTERKEY 11
#define MANAUTOKEY 12
#define PLAYSTOPKEY 13
#define DELKEY 14
#define CLEANKEY 15
#define BLINK 2
#define LOADING 1
#define NOT_LOADING 0
#define NONE -1


extern uint32_t PowerButtonPressTime;// = 0; // Time when power button is pressed
extern uint8_t isAutoOperation;// = TRUE; // Auto operation state
extern uint8_t isCleaningOperation;// = FALSE; // Cleaning operation state
extern int lastKeyPressed;// = 255; // Last key pressed
extern uint8_t isPausedOperation;// = FALSE; // Paused operation state
extern uint8_t monitorEnterKey;// = FALSE; // Monitor enter key state
extern uint8_t EnterPressed;// = FALSE; // Enter key pressed state
extern uint8_t monitorCleanKey;// = FALSE; // Monitor clean key state
volatile extern uint8_t cleanLongPressed;// = FALSE; // State of long press of clean key
extern uint8_t delLongPressed;// = FALSE; // State of long press of del key
extern uint8_t isDemoCleaning;// = FALSE; // Demo cleaning state

// Function prototype
int ReadKeypadVar(int group);
int readKeypadNonBlock(int group);
int readKeypad(int group);
void LoadingFunc(int blinkLED, uint8_t ScreenLoad);
void Take2RowEntries(char *Row1StaticString, char *Row2StaticString);


#endif // KEYPAD_H
