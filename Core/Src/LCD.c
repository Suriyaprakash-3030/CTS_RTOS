/*
 * LCD.c
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#include <LCD.h>


char lastLCDData[LCD_ROW_COUNT][MAX_STRING_LENGTH];
int lastLCDCustom[LCD_COLUMN_COUNT];
uint8_t isPowerLEDBlinking = FALSE; //* Power LED blinking state boolean variable
uint8_t isPowerLEDOn = FALSE; //* Power LED state boolean variable
uint8_t isScreenLoading = FALSE; //* Screen loading state boolean variable
char DotString[LCD_COLUMN_COUNT + 1] = ""; //* String for screen loading dots

//*LCD Variables
char stringBuffer[21]; //buffer to store variables as stirng


char* LCD_BUF_ARR[4];
int LCD_SMILE_ARR[4] = {0, 0, 0, 0};
int New_LCD_Data[4] = {0, 0, 0, 0};

uint8_t Custom_Blank_Symbol[8] = {
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};
uint8_t Custom_Enter_Symbol[8] = {
  0B00000,
  0B00001,
  0B00001,
  0B00101,
  0B01001,
  0B11111,
  0B01000,
  0B00100
};
//* Define the custom refresh-like character
uint8_t Custom_Reclean_Symbol[8] = {
  0B00000,
  0B11111,
  0B10001,
  0B10001,
  0B10100,
  0B11110,
  0B00100,
  0B00000
};
//*Define the custom cursor-like character*/
uint8_t Custom_Cursor_Symbol[8] = {
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B11111,
  0B11111
};

uint8_t Custom_demo_cursor_Symbol[8] = {
  0B01010,
  0B01010,
  0B11111,
  0B01010,
  0B11111,
  0B01010,
  0B01010
};


//Functions for LCD System
void LCD_Init() {
	// Assuming GPIO has been initialized

	HAL_Delay(15); // Wait for more than 15ms after Vcc rises to 4.5V

	// Function set: 8-bit mode
	LCD_SendHalfByte(0x03);
	HAL_Delay(5); // Wait for more than 4.1ms
	LCD_SendHalfByte(0x03);
	HAL_Delay(1); // Wait for more than 100us
	LCD_SendHalfByte(0x03);

	// Function set: Switch to 4-bit mode
	LCD_SendHalfByte(0x02);

	// Complete function set: 4-bit mode, 2-line display, 5x8 font
	LCD_SendCommand(0x28);

	// Display ON, cursor OFF, blink OFF
	LCD_SendCommand(0x0C);

	// Entry mode set: Increment cursor, no shift
	LCD_SendCommand(0x06);

	// Clear display
	LCD_SendCommand(0x01);
	HAL_Delay(2); // Clearing the display takes 1.52ms

	LCD_CreateCustomChar(0, Custom_Blank_Symbol);
	LCD_CreateCustomChar(1, Custom_Enter_Symbol);
	LCD_CreateCustomChar(2, Custom_Reclean_Symbol);
	LCD_CreateCustomChar(3, Custom_Cursor_Symbol);
	LCD_CreateCustomChar(4, Custom_demo_cursor_Symbol);

	pwm(LCD_LED_PWM,50);
	pwm(LCD_Contrast_PWM,10);
}

void LCD_SendCommand(uint8_t cmd) {

	HAL_GPIO_WritePin(LCD_RS_Output_GPIO_Port, LCD_RS_Output_Pin,
			GPIO_PIN_RESET); // RS = 0 for command
	//HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET); // RW = 0 for write
	LCD_SendHalfByte(cmd >> 4); // Send higher nibble
	LCD_SendHalfByte(cmd & 0x0F); // Send lower nibble
}

void LCD_SendData(uint8_t data) {
	HAL_GPIO_WritePin(LCD_RS_Output_GPIO_Port, LCD_RS_Output_Pin, GPIO_PIN_SET); // RS = 1 for data
	//HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET); // RW = 0 for write

	LCD_SendHalfByte(data >> 4); // Send higher nibble
	LCD_SendHalfByte(data & 0x0F); // Send lower nibble
}

void LCD_CreateCustomChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x07; // Only 8 locations (0-7)
	LCD_SendCommand(0x40 | (location << 3)); // Set CGRAM address
	for (int i = 0; i < 8; i++) {
		LCD_SendData(charmap[i]);
	}
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
	uint8_t address;
	// Each row has different starting addresses
	switch (row) {
	case 0:
		address = 0x00 + col;
		break; // 1st row starting from 0x00
	case 1:
		address = 0x40 + col;
		break; // 2nd row starting from 0x40
	case 2:
		address = 0x14 + col;
		break; // 3rd row starting from 0x14
	case 3:
		address = 0x54 + col;
		break; // 4th row starting from 0x54
	default:
		address = 0x00; // Default to first row if invalid row number
	}
	// Send command to set DDRAM address
	LCD_SendCommand(0x80 | address);
}

void LCD_Clear(void) {
	LCD_ClearStringBuffer();
	LCD_SendCommand(0x01); // Clear display command
	HAL_Delay(2); // Wait for the command to be processed (clearing takes time)
}
void LCD_SendHalfByte(uint8_t data) {
	HAL_GPIO_WritePin(LCD_D4_Output_GPIO_Port, LCD_D4_Output_Pin,
			(data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D5_Output_GPIO_Port, LCD_D5_Output_Pin,
			(data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D6_Output_GPIO_Port, LCD_D6_Output_Pin,
			(data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D7_Output_GPIO_Port, LCD_D7_Output_Pin,
			(data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	// Pulse the Enable pin
	HAL_GPIO_WritePin(LCD_EN_Output_GPIO_Port, LCD_EN_Output_Pin, GPIO_PIN_SET);
	//HAL_Delay(1); // Delay for enable pulse width
	for (uint16_t i = 0; i < 400; i++){ //140 is supposed to be 10uS
		__NOP();
	}
	HAL_GPIO_WritePin(LCD_EN_Output_GPIO_Port, LCD_EN_Output_Pin,
			GPIO_PIN_RESET);
	//HAL_Delay(1);
	for (uint16_t i = 0; i < 400; i++){
		__NOP();
	}
}

void LCD_ClearStringBuffer(void) {
	for (int i = 0; i < sizeof(stringBuffer); i++) {
		stringBuffer[i] = '\0';
	}

//	memset(stringBuffer, 0, 21);
}

char* combineStrings(const char *string1, float x) {
	static char combinedString[21]; // Fixed size buffer for LCD, including null terminator

	// Convert the float value to a string and store it in stringBuffer
	LCD_StoreValueInBuffer(x);

	// Clear the combinedString buffer
	memset(combinedString, '\0', sizeof(combinedString));

	// Concatenate the string1 and the string representation of the float
	snprintf(combinedString, sizeof(combinedString), "%s%s", string1,
			stringBuffer);

	return combinedString;
}

void LCD_StoreValueInBuffer(float value) {
	LCD_ClearStringBuffer();
	sprintf(stringBuffer, "%.2f", value);  // Can adjust the format as needed
}

void printLCD_PERFORM(int row, char*data, int smiley){
	LCD_BUF_ARR[row] = data;
	LCD_SMILE_ARR[row] = smiley;
	New_LCD_Data[row] = 1;
}

void printLCD_CMD(){
	for(int i = 0; i < 4; i++){
		if(New_LCD_Data[i] == 1){
			//printLCD_PERFORM(i, LCD_BUF_ARR[i], LCD_SMILE_ARR[i]);
			New_LCD_Data[i] = 0;
		}

	}
}

void printLCDInt(int row, char *data, int number,int totalCycle) {
    int localcounter = 0;
    char numberStr[22];
    if (row < 0 || row >= LCD_ROW_COUNT) {
        return;
    }
    strncpy(lastLCDData[row], data, MAX_STRING_LENGTH - 1);
    lastLCDData[row][MAX_STRING_LENGTH - 1] = '\0';
    if(totalCycle > 0)
    	snprintf(numberStr, sizeof(numberStr), "%d/%d", number,totalCycle);
    else
       	snprintf(numberStr, sizeof(numberStr), "%d", number);
    LCD_SetCursor(row, 0);
    	// Print the data
    	while (*data && localcounter < (LCD_COLUMN_COUNT)) {
    		LCD_SendData(*data++);
    		localcounter++;
    	}
    	for (int i = 0; numberStr[i] != '\0' && localcounter < LCD_COLUMN_COUNT; i++) {
    	        LCD_SendData(numberStr[i]);
    	        localcounter++;
    	    }

    	// Check the length of the data and print the smiley if there is space
    	/*if (strlen(lastLCDData[row]) < LCD_COLUMN_COUNT) {

    	LCD_SendData(numberStr);
    	localcounter++;
    	}
    	*/
    	//Fill rest of column with empty space
    	while(localcounter < LCD_COLUMN_COUNT){
    		LCD_SendData(0);
    		localcounter++;
    }
}

/*
void printLCDINT(int row, char *data, int value)
{
	int localcounter = 0;
	if (row < 0 || row >= LCD_ROW_COUNT) {
		return; // Row is out of bounds
	}
	// Store the data in the global array lastLCDData
	strncpy(lastLCDData[row], data, MAX_STRING_LENGTH - 1);
	lastLCDData[row][MAX_STRING_LENGTH - 1] = '\0'; // Ensure null termination
	// Store the smiley (custom character) code in lastLCDCustom
	lastLCDCustom[row] = value;
	// Set cursor position
	LCD_SetCursor(row, 0);
	// Print the data
	while (*data && localcounter < (LCD_COLUMN_COUNT)) {
		LCD_SendData(*data++);
		localcounter++;
	}
	// Check the length of the data and print the smiley if there is space
	if (strlen(lastLCDData[row]) < LCD_COLUMN_COUNT) {
		LCD_SendData(value);
		localcounter++;
	}
	//Fill rest of column with empty space
	while(localcounter < LCD_COLUMN_COUNT){
		LCD_SendData(0);
		localcounter++;
	}
}
*/

void printLCD(int row, char *data, int smiley) {
	int localcounter = 0;
	if (row < 0 || row >= LCD_ROW_COUNT) {
		return; // Row is out of bounds
	}
	// Store the data in the global array lastLCDData
	strncpy(lastLCDData[row], data, MAX_STRING_LENGTH - 1);
	lastLCDData[row][MAX_STRING_LENGTH - 1] = '\0'; // Ensure null termination
	// Store the smiley (custom character) code in lastLCDCustom
	lastLCDCustom[row] = smiley;
	// Set cursor position
	LCD_SetCursor(row, 0);
	// Print the data
	while (*data && localcounter < (LCD_COLUMN_COUNT)) {
		LCD_SendData(*data++);
		localcounter++;
	}
	// Check the length of the data and print the smiley if there is space
	if (strlen(lastLCDData[row]) < LCD_COLUMN_COUNT) {
		LCD_SendData(smiley);
		localcounter++;
	}
	//Fill rest of column with empty space
	while(localcounter < LCD_COLUMN_COUNT){
		LCD_SendData(0);
		localcounter++;
	}
}

void LCDRGBControl(int brightness) {
	if (brightness > 100) {
		brightness = 100;
		} else if (brightness < 0) {
			brightness = 0;
		}
		int contrast = 100 - brightness;
	pwm(LCD_Contrast_PWM, contrast);  //Pins are Common Anode
}

void retrieveLCDLastData() {
	for (int i = 0; i < LCD_ROW_COUNT; i++) {
		printLCD(i, lastLCDData[i], lastLCDCustom[i]);
	}
}

void powerLEDControl(int state) {
	if (state == OFF) {
		digitalWrite(Power_LED_Output, LOW);
		isPowerLEDOn = FALSE;
	} else if (state == ON) {
		digitalWrite(Power_LED_Output, HIGH);
		isPowerLEDOn = TRUE;
	}
}

void controlLCDContrast(int contrastLevel) {
	if (contrastLevel > 100) {
		contrastLevel = 100;
	} else if (contrastLevel < 0) {
		contrastLevel = 0;
	}
	int contrast = 100 - contrastLevel;
	pwm(LCD_Contrast_PWM, contrast);  //Change LCD contrast
}
