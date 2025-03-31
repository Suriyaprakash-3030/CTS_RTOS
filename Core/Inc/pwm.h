/*
 * pwm.h
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#ifndef PWM_H
#define PWM_H

#include "tim.h"
#include <main.h>

extern int PWM_FAST;
extern int PWM_SLOW;
extern int PWM_CLEANING;

/* Enum for user labels */

typedef enum {
    ServoDC_M1_PWM,
    Clamp_M2_PWM,
    Spindle_M3_PWM,
    LCD_Contrast_PWM,
    LCD_LED_PWM
#ifdef Servo
	,Servo_PWM_OR_DC_M1_EncA_Interrupt
#endif
} PWM_Label;

/* Function to set PWM duty cycle */
void pwm(PWM_Label label, float percentage);

#endif // PWM_H
