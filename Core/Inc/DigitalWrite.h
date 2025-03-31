/*
 * DigitalWrite.h
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#ifndef DIGITALWRITE_H
#define DIGITALWRITE_H

#include "stm32f4xx_hal.h"

// Enumeration for output pin labels
typedef enum {
	Power_LED_Output,
	ServoDC_M1_Dir_Output,
	nRESET_LPC_Output,
	Relay_Output,
	Spindle_M3_Dir_Output,
	Clamp_M2_Dir_Output,
	LCD_RS_Output,
	LCD_EN_Output,
	LCD_D4_Output,
	LCD_D5_Output,
	LCD_D6_Output,
	LCD_D7_Output,
	USB_EN_Output,
	EN_DCDC_5V_Output,
	EN_DCDC_8V_Output
} OutputLabel;

void digitalWrite(OutputLabel label, int state);

#endif
