/*
 * DigitalRead.h
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

/*This is the header file for DigitalRead functions that will provide similar code reading as Arduino*/
#ifndef DIGITALREAD_H
#define DIGITALREAD_H

#include "stm32f4xx_hal.h"

typedef enum {
	Button_ManAuto_Input_PullUp,
	Button_PlayStop_Input_PullUp,
	Button_DelBack_Input_PullUp,
	Button_1_Input_PullUp,
	Button_2_Input_PullUp,
	Button_3_Input_PullUp,
	Button_4_Input_PullUp,
	Button_5_Input_PullUp,
	Button_6_Input_PullUp,
	Button_7_Input_PullUp,
	Button_8_Input_PullUp,
	Button_9_Input_PullUp,
	Button_0_Input_PullUp,
	Button_Enter_Input_PullUp,
	Button_Comma_Input_PullUp
} UserLabel;

GPIO_PinState digitalRead(UserLabel label);

#endif
