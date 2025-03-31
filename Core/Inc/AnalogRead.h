/*
 * AnalogRead.h
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#ifndef ANALOG_READ_H
#define ANALOG_READ_H

#include <main.h>

/* Enum for different ADC labels */
typedef enum {
	Curr_Sensor_ServoDC_M1,
	Curr_Sensor_Clamp_M2,
	Curr_Sensor_Spindle_M3,
    V12,
	Sensor_Pressure
} ADC_Label;

/* Read ADC value based on the provided label */
uint32_t analogRead(ADC_Label label);

/* Read Pressure value  */
float pressureSensorReading(void);

/* Read Current value based on the provided  ADC label   */
int Convert_ADC_To_Current(uint16_t adc_value);

#endif // ANALOG_READ_H
