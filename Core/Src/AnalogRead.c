/*
 * AnalogRead.c
 *
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#include <AnalogRead.h>

extern ADC_HandleTypeDef hadc1; // Handler for ADC1
extern ADC_HandleTypeDef hadc2; // Handler for ADC2
extern ADC_HandleTypeDef hadc3; // Handler for ADC3


long map(long x, long in_min, long in_max, long out_min, long out_max);

/* Read ADC value based on the provided label */
uint32_t analogRead(ADC_Label label)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    uint32_t adc_value = 0;

    switch (label)
    {
        case Curr_Sensor_ServoDC_M1:
            // Configure ADC3 for Curr_Sensor_ServoDC (GPIOC_PIN_2)
            sConfig.Channel = ADC_CHANNEL_12;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
            HAL_ADC_ConfigChannel(&hadc3, &sConfig);
            HAL_ADC_Start(&hadc3);
            HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
            adc_value = HAL_ADC_GetValue(&hadc3);
            HAL_ADC_Stop(&hadc3);
            break;

        case Curr_Sensor_Clamp_M2:
            // Configure ADC3 for Curr_Sensor_Clamp (GPIOC_PIN_3)
            sConfig.Channel = ADC_CHANNEL_13;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
            HAL_ADC_ConfigChannel(&hadc3, &sConfig);
            HAL_ADC_Start(&hadc3);
            HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
            adc_value = HAL_ADC_GetValue(&hadc3);
            HAL_ADC_Stop(&hadc3);
            break;

        case Curr_Sensor_Spindle_M3:
            // Configure ADC3 for Curr_Sensor_Spindle (GPIOA_PIN_0)
            sConfig.Channel = ADC_CHANNEL_0;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
            HAL_ADC_ConfigChannel(&hadc3, &sConfig);
            HAL_ADC_Start(&hadc3);
            HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY);
            adc_value = HAL_ADC_GetValue(&hadc3);
            HAL_ADC_Stop(&hadc3);
            break;

        case V12:
            // Configure ADC2 for V12 (GPIOA_PIN_1)
            sConfig.Channel = ADC_CHANNEL_1;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
            HAL_ADC_ConfigChannel(&hadc2, &sConfig);
            HAL_ADC_Start(&hadc2);
            HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
            adc_value = HAL_ADC_GetValue(&hadc2);
            HAL_ADC_Stop(&hadc2);
            break;

        case Sensor_Pressure:
            // Configure ADC1 for Sensor_Pressure (GPIOC_PIN_4)
            sConfig.Channel = ADC_CHANNEL_14;
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
            HAL_ADC_ConfigChannel(&hadc1, &sConfig);
            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
            adc_value = HAL_ADC_GetValue(&hadc1);
            HAL_ADC_Stop(&hadc1);
            break;

        default:
            // Handle invalid label case
            return 0;
    }
    return adc_value;
}

/* map Function to the sensor voltage to pressure */
float pressureSensorReading(void){

  uint32_t PressureRead  =  analogRead(Sensor_Pressure);
   pressurechange	 =  map(PressureRead,314,3030,0,5300); //5300 -5 bar  4300 -4  //3300 -3 bar //2200 -2bar // 1120 - 1bar
  return pressurechange;

}

/* map Function  */
long map(long x, long in_min, long in_max, long out_min, long out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

// Function to convert ADC value to current in milliAmperes
int Convert_ADC_To_Current(uint16_t adc_value) {
	int16_t current =   map(adc_value,0,4095,-10000,10000);
	 if(current < 0)
	 {
		 current = current *(-1);
	 }
    return (int)current * 2;  // Return the calculated current in amperes
}



