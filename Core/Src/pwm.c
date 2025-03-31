/*
 * pwm.c
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#include "pwm.h"

int PWM_FAST=98;
int PWM_SLOW=90;
int PWM_CLEANING = 50;

void pwm(PWM_Label label, float percentage)
{
    if (percentage < 0.0f) percentage = 0.0f;
    if (percentage > 100.0f) percentage = 100.0f;

    uint32_t pulse = 0;
    TIM_HandleTypeDef* htim = NULL;
    uint32_t channel = 0;

    switch (label)
    {
        case ServoDC_M1_PWM:
            htim = &htim3;
            channel = TIM_CHANNEL_3;
            break;

        case Clamp_M2_PWM:
            htim = &htim4;
            channel = TIM_CHANNEL_1;
            break;

        case Spindle_M3_PWM:
            htim = &htim4;
            channel = TIM_CHANNEL_2;
            break;

        case LCD_Contrast_PWM:
            htim = &htim8;
            channel = TIM_CHANNEL_1;
            break;

        case LCD_LED_PWM:
            htim = &htim8;
            channel = TIM_CHANNEL_2;
            break;
#ifdef Servo
        case Servo_PWM_OR_DC_M1_EncA_Interrupt:
            htim = &htim11;
            channel = TIM_CHANNEL_1;
            break;
#endif
        default:
            return; // Invalid label
    }

    // Calculate the pulse value based on percentage
    pulse = (uint32_t)((percentage / 100.0f) * (htim->Init.Period + 1));

    // Set the pulse value for the corresponding channel
    __HAL_TIM_SET_COMPARE(htim, channel, pulse);

    // Start PWM signal generation if not already running
    HAL_TIM_PWM_Start(htim, channel);
}

