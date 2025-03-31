/*
 * Keypad.c
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#include <keypad.h>
#include "main.h"
uint32_t PowerButtonPressTime = 0; // Time when power button is pressed
uint8_t isAutoOperation = TRUE; // Auto operation state
uint8_t isCleaningOperation = FALSE; // Cleaning operation state
int lastKeyPressed = 255; // Last key pressed
uint8_t isPausedOperation = FALSE; // Paused operation state
uint8_t monitorEnterKey = FALSE; // Monitor enter key state
uint8_t EnterPressed = FALSE; // Enter key pressed state
uint8_t monitorCleanKey = FALSE; // Monitor clean key state
volatile uint8_t cleanLongPressed = FALSE; // State of long press of clean key
uint8_t delLongPressed = FALSE;
uint8_t isDemoCleaning = FALSE; // Demo cleaning state


int ReadKeypadVar(int group) {
    lastKeyPressed = readKeypad(group);
    if (lastKeyPressed == MANAUTOKEY) {
        isAutoOperation = !isAutoOperation;
    } else if (lastKeyPressed == CLEANKEY) {
        isCleaningOperation = !isCleaningOperation;
    } else if (lastKeyPressed == PLAYSTOPKEY) {
        isPausedOperation = !isPausedOperation;
    }
    if (lastKeyPressed != 255) {
        HAL_Delay(5); // Delay for debounce, using HAL_Delay for STM32
    }
    return lastKeyPressed;
}

int readKeypadNonBlock(int group)
{
	//group=ALL;
  if (group == NUMPAD || group == ALL)
  {
    if (digitalRead(Button_1_Input_PullUp) == LOW) {

      return 1;
    } else if (digitalRead(Button_2_Input_PullUp) == LOW) {

      return 2;
    } else if (digitalRead(Button_3_Input_PullUp) == LOW) {

      return 3;
    } else if (digitalRead(Button_4_Input_PullUp) == LOW) {

      return 4;
    } else if (digitalRead(Button_5_Input_PullUp) == LOW) {

      return 5;
    }else if (digitalRead(Button_6_Input_PullUp) == LOW) {

      return 6;
    } else if (digitalRead(Button_7_Input_PullUp) == LOW) {

      return 7;
    } else if (digitalRead(Button_8_Input_PullUp) == LOW) {

      return 8;
    } else if (digitalRead(Button_9_Input_PullUp) == LOW) {

      return 9;
    } else if (digitalRead(Button_0_Input_PullUp) == LOW) {

      return 0;
    } else if (digitalRead(Button_Comma_Input_PullUp) == LOW) {

      return COMMAKEY;
    } else if (digitalRead(Button_Enter_Input_PullUp) == LOW) {

      return ENTERKEY;
    }
  }
  if (group == CONTROL || group == ALL)
  {
    if (digitalRead(Button_Enter_Input_PullUp) == LOW) {

      return ENTERKEY;
    } else if (digitalRead(Button_ManAuto_Input_PullUp) == LOW) {

      return MANAUTOKEY;
    } else if (digitalRead(Button_PlayStop_Input_PullUp) == LOW) {

      return PLAYSTOPKEY;
    } else if (digitalRead(Button_DelBack_Input_PullUp) == LOW) {

      return DELKEY;
    }
  }
  return 255;
}

int readKeypad(int group)
{
	//group = ALL;
  if (group == NUMPAD || group == ALL)
  {
    if (digitalRead(Button_1_Input_PullUp) == LOW){
      while (digitalRead(Button_1_Input_PullUp) == LOW) {
        HAL_Delay(1);
      }
      return 1;
    } else if (digitalRead(Button_2_Input_PullUp) == LOW){
      while (digitalRead(Button_2_Input_PullUp) == LOW){
    	  HAL_Delay(1);
      }
      return 2;
    } else if (digitalRead(Button_3_Input_PullUp) == LOW) {
    	while (digitalRead(Button_3_Input_PullUp) == LOW) {
      HAL_Delay(1);
      }
      return 3;
    } else if (digitalRead(Button_4_Input_PullUp) == LOW) {
      while (digitalRead(Button_4_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }
      return 4;
    } else if (digitalRead(Button_5_Input_PullUp) == LOW) {
      while (digitalRead(Button_5_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }
      return 5;
    } else if (digitalRead(Button_6_Input_PullUp) == LOW) {
      while (digitalRead(Button_6_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }
      return 6;
    } else if (digitalRead(Button_7_Input_PullUp) == LOW) {
      while (digitalRead(Button_7_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }
      return 7;
    } else if (digitalRead(Button_8_Input_PullUp) == LOW) {
      while (digitalRead(Button_8_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }
      return 8;
    } else if (digitalRead(Button_9_Input_PullUp) == LOW) {
      while (digitalRead(Button_9_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }
      return 9;
    } else if (digitalRead(Button_0_Input_PullUp) == LOW) {
      while (digitalRead(Button_0_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }

      return 0;
    } else if (digitalRead(Button_Comma_Input_PullUp) == LOW) {
      while (digitalRead(Button_Comma_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }

      return COMMAKEY;
    } else if (digitalRead(Button_Enter_Input_PullUp) == LOW) {
      while (digitalRead(Button_Enter_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }

      return ENTERKEY;
    }
  }
  if (group == CONTROL || group == ALL) {
    if (digitalRead(Button_Enter_Input_PullUp) == LOW) {
      while (digitalRead(Button_Enter_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }

      return ENTERKEY;
    } else if (digitalRead(Button_ManAuto_Input_PullUp) == LOW) {
      while (digitalRead(Button_ManAuto_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }

      return MANAUTOKEY;
    } else if (digitalRead(Button_PlayStop_Input_PullUp) == LOW) {
      while (digitalRead(Button_PlayStop_Input_PullUp) == LOW) {
    	  HAL_Delay(1);
      }

      return PLAYSTOPKEY;
    } else if (digitalRead(Button_DelBack_Input_PullUp) == LOW) {
    	unsigned long localDelTimer = HAL_GetTick();
    	delLongPressed = FALSE;
    	while (digitalRead(Button_DelBack_Input_PullUp) == LOW) {
    		HAL_Delay(1);
    		if (HAL_GetTick() - localDelTimer >= 2000) {
    			delLongPressed = TRUE;
    			break;
    		}
    	}

    	return DELKEY;
    }
  }
  return 255;
}


void LoadingFunc(int blinkLED, uint8_t ScreenLoad) {
    if (blinkLED == OFF) {
        powerLEDControl(OFF);
        isPowerLEDBlinking = FALSE;
    } else if (blinkLED == ON) {
       powerLEDControl(ON);
        isPowerLEDBlinking = FALSE;
    } else if (blinkLED == BLINK) {
        isPowerLEDBlinking = TRUE;
    }

    if (ScreenLoad==TRUE) {
        strcpy(DotString, ""); // Clear the DotString
        isScreenLoading = TRUE;
        printLCD(0, " ", NO_SYMBOL);
    } else {
        isScreenLoading = FALSE;
    }
}

void Take2RowEntries(char *Row1StaticString, char *Row2StaticString) {

	uint8_t row2done = 0;  // false represented as 0
	char row1entryS[LCD_COLUMN_COUNT + 1]; // Assuming a max size of 50 for the string +1 is for null terminator
	row1entryS[0] = '\0';
	char row2entryS[50];   // Adjust the size as needed
	row2entryS[0] = '\0';
	float row1entryF = 0.0;
	float row2entryF = 0.0;

	// Convert lastKeyPressed to a string
	char lastKeyPressedStr[3]; //For storing lastKeyPressed as a character array/string
	char SerialBuffer[32];		//For storing rowXentryF as a character array/string

	char CombStr1[LCD_COLUMN_COUNT + 1]; 	//For storing the combined strings of row 1
	CombStr1[0] = '\0';
	char CombStr2[LCD_COLUMN_COUNT + 1];   //For storing the combined strings of row 2
	CombStr2[0] = '\0';

	strcpy(CombStr1, Row1StaticString);
	strcat(CombStr1, row1entryS);

	strcpy(CombStr2, Row2StaticString);
	strcat(CombStr2, row2entryS);

	printLCD(2, CombStr2, NO_SYMBOL);
	printLCD(1, CombStr1, CURSOR_SYMBOL);

	while (row2done == FALSE) {
		printLCD(2, CombStr2, NO_SYMBOL); //Remove any symbol from 2nd row. Useful when we hit delete on 2nd row and go to 1st row
		//Take 1st row entry
		while (ReadKeypadVar(ALL) != ENTERKEY || row1entryF == 0.0) {
			  if (lastKeyPressed == MANAUTOKEY)
				{
				  CustomerApp();
				}

			if (lastKeyPressed >= 0 && lastKeyPressed <= 9) { //If last key pressed is a number
				//int temp;
				//temp = atoi(row1entryS);  // Convert string to integer
				if (lastKeyPressed == 0 && strcmp(row1entryS,"0")==0/*temp == 0*/) { //If there is a leading 0 and user presses another 0, do not add it
					//Do nothing
				} else if (strlen(row1entryS) >= MAX_ENTRY_KEYS) {
					//Do nothing
				} else {
					lastKeyPressedStr[0] = '\0';
					sprintf(lastKeyPressedStr, "%d", lastKeyPressed);
					strcat(row1entryS, lastKeyPressedStr);
				}
			}else if (lastKeyPressed == DELKEY && (strlen(row1entryS)) > 0) {
				row1entryS[(strlen(row1entryS)) - 1] = '\0'; //Delete last entry

			} else if (lastKeyPressed == COMMAKEY) {
				if ((strlen(row1entryS)) == 0) { //If the user presses comma 1st thing in string, add a leading 0
					strcat(row1entryS, "0.");
				} else if (strchr(row1entryS, '.') == NULL) { //If there is no comma in string, add one
					strcat(row1entryS, ".");
				}
			}
			strcpy(CombStr1, Row1StaticString);
			strcat(CombStr1, row1entryS);
			printLCD(1, CombStr1, CURSOR_SYMBOL);
			row1entryF = strtod(row1entryS, NULL);
		}

		SerialBuffer[0] = '\0';
		sprintf(SerialBuffer, "%.2f", row1entryF);
		SerialPrint("Row1: ");
		SerialPrintln(SerialBuffer);

		//printLCD(1, CombStr1, NO_SYMBOL); //Used to remove cursor symbol when entry is done
		row2done = TRUE;

		//Take 2nd row entry
		while (ReadKeypadVar(ALL) != ENTERKEY || row2entryF == 0.0) {
      if (lastKeyPressed == MANAUTOKEY)
			{
    	  	  CustomerApp();
			}
			if (lastKeyPressed >= 0 && lastKeyPressed <= 9) { //If last key pressed is a number
				//int temp;
				//temp = atoi(row2entryS);  // Convert string to integer
				if (lastKeyPressed == 0 && strcmp(row2entryS,"0")==0/*temp == 0*/) { //If there is a leading 0 and user presses another 0, do not add it
				//Do nothing
				} else if (strlen(row2entryS) >= MAX_ENTRY_KEYS) {
					//Do nothing
				} else {
					lastKeyPressedStr[0] = '\0';
					sprintf(lastKeyPressedStr, "%d", lastKeyPressed);
					strcat(row2entryS, lastKeyPressedStr);
				}
			} else if (lastKeyPressed == DELKEY) {
				if (strlen(row2entryS) > 0) {
					row2entryS[(strlen(row2entryS)) - 1] = '\0'; //Delete last entry
				} else { //Reaching here means del key is pressed in 2nd row, and that row is empty. go back to prev row!
					row2done = FALSE;
					break; //Exit the while loop. This should lead back to the 1st while loop
				}
			}  else if (lastKeyPressed == COMMAKEY) {
				if ((strlen(row2entryS)) == 0) { //If the user presses comma 1st thing in string, add a leading 0
					strcat(row2entryS, "0.");
				} else if (strchr(row2entryS, '.') == NULL) { //If there is no comma in string, add one
					strcat(row2entryS, ".");
				}
			}
			strcpy(CombStr2, Row2StaticString);
			strcat(CombStr2, row2entryS);
			printLCD(2, CombStr2, CURSOR_SYMBOL);
			row2entryF = strtod(row2entryS, NULL);
		}
		SerialBuffer[0] = '\0';
		sprintf(SerialBuffer, "%.2f", row2entryF);
		SerialPrint("Row2: ");
		SerialPrintln(SerialBuffer);
	}
	// Update global variables
	LitersVar = row1entryF;
	DensityVar = row2entryF;
}
