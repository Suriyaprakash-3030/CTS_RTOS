/*
 * DigitalWrite.c
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#include "main.h"
#include "DigitalWrite.h"

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_OutputMap;

static GPIO_OutputMap outputMap[] = {									//Labels:
    {Power_LED_Output_GPIO_Port, Power_LED_Output_Pin},  				// Power_LED_Output
	{ServoDC_M1_Dir_Output_GPIO_Port, ServoDC_M1_Dir_Output_Pin},  		// ServoDC_M1_Dir_Output
	{nRESET_LPC_Output_GPIO_Port, nRESET_LPC_Output_Pin},  				// nRESET_LPC_Output
	{Relay_Output_GPIO_Port, Relay_Output_Pin},  						// Relay_Output
	{Spindle_M3_Dir_Output_GPIO_Port, Spindle_M3_Dir_Output_Pin},		//Spindle_M3_Dir_Output
	{Clamp_M2_Dir_Output_GPIO_Port, Clamp_M2_Dir_Output_Pin},			// Clamp_M2_Dir_Output
    {LCD_RS_Output_GPIO_Port, LCD_RS_Output_Pin},  						// LCD_RS_Output
    {LCD_EN_Output_GPIO_Port, LCD_EN_Output_Pin},  						// LCD_EN_Output
    {LCD_D4_Output_GPIO_Port, LCD_D4_Output_Pin},  						// LCD_D4_Output
    {LCD_D5_Output_GPIO_Port, LCD_D5_Output_Pin},  						// LCD_D5_Output
    {LCD_D6_Output_GPIO_Port, LCD_D6_Output_Pin},  						// LCD_D6_Output
    {LCD_D7_Output_GPIO_Port, LCD_D7_Output_Pin},  						// LCD_D7_Output
	{USB_EN_Output_GPIO_Port, USB_EN_Output_Pin},						// USB_EN_Output
	{EN_DCDC_5V_Output_GPIO_Port, EN_DCDC_5V_Output_Pin},				// EN_DCDC_5V_Output
	{EN_DCDC_8V_Output_GPIO_Port, EN_DCDC_8V_Output_Pin},				// EN_DCDC_8V_Output
};

void digitalWrite(OutputLabel label, int state)
{
    if (label >= sizeof(outputMap) / sizeof(GPIO_OutputMap))
    {
        // Handle invalid label
        return;
    }

    GPIO_OutputMap map = outputMap[label];
    HAL_GPIO_WritePin(map.port, map.pin, (state != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

