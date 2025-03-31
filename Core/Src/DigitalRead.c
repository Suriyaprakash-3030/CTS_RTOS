/*
 * DigitalRead.c
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#include "main.h"
#include "DigitalRead.h"

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_Map;

static GPIO_Map gpioMap[] =
{																				// Labels:
    {Button_ManAuto_Input_PullUp_GPIO_Port, Button_ManAuto_Input_PullUp_Pin}, 	// BUTTON_MANAUTO_Input_Pullup
    {Button_PlayStop_Input_PullUp_GPIO_Port, Button_PlayStop_Input_PullUp_Pin}, // BUTTON_PLAYSTOP_Input_Pullup
    {Button_DelBack_Input_PullUp_GPIO_Port, Button_DelBack_Input_PullUp_Pin}, 	// BUTTON_DEL_Input_Pullup
    {Button_1_Input_PullUp_GPIO_Port, Button_1_Input_PullUp_Pin}, 				// BUTTON_1_Input_Pullup
    {Button_2_Input_PullUp_GPIO_Port, Button_2_Input_PullUp_Pin}, 				// BUTTON_2_Input_Pullup
    {Button_3_Input_PullUp_GPIO_Port, Button_3_Input_PullUp_Pin}, 				// BUTTON_3_Input_Pullup
    {Button_4_Input_PullUp_GPIO_Port, Button_4_Input_PullUp_Pin}, 				// BUTTON_4_Input_Pullup
    {Button_5_Input_PullUp_GPIO_Port, Button_5_Input_PullUp_Pin}, 				// BUTTON_5_Input_Pullup
    {Button_6_Input_PullUp_GPIO_Port, Button_6_Input_PullUp_Pin}, 				// BUTTON_6_Input_Pullup
    {Button_7_Input_PullUp_GPIO_Port, Button_7_Input_PullUp_Pin}, 				// BUTTON_7_Input_Pullup
    {Button_8_Input_PullUp_GPIO_Port, Button_8_Input_PullUp_Pin}, 				// BUTTON_8_Input_Pullup
    {Button_9_Input_PullUp_GPIO_Port, Button_9_Input_PullUp_Pin}, 				// BUTTON_9_Input_Pullup
    {Button_0_Input_PullUp_GPIO_Port, Button_0_Input_PullUp_Pin}, 				// BUTTON_0_Input_Pullup
    {Button_Enter_Input_PullUp_GPIO_Port, Button_Enter_Input_PullUp_Pin}, 		// BUTTON_ENTER_Input_Pullup
    {Button_Comma_Input_PullUp_GPIO_Port, Button_Comma_Input_PullUp_Pin}  		// BUTTON_COMMA_Input_Pullup
};


GPIO_PinState digitalRead(UserLabel label) {
    if (label >= sizeof(gpioMap) / sizeof(GPIO_Map)) {
        // Handle invalid label
        return GPIO_PIN_RESET;
    }

    GPIO_Map map = gpioMap[label];
    return HAL_GPIO_ReadPin(map.port, map.pin);
}

