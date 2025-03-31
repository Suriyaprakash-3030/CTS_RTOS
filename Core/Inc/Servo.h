/*
 * Servo.h
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#ifndef SERVO_H
#define SERVO_H

#include <main.h>

#define SERVO_MIN_PULSE_WIDTH 170   // Minimum pulse width in microseconds
#define SERVO_MAX_PULSE_WIDTH 720  // Maximum pulse width in microseconds
#define SERVO_MAX_ANGLE 175        // Maximum angle for servo
#define SERVO_MIN_ANGLE 5           // Minimum angle for servo
#define ISSERVOINTERTED	1

extern int percent; 				//Servo Angle Percentage

/* Initialize the servo motor control (Timer 11, Channel 1) */
void Servo_Init(void);

/* Set the servo to a specific angle (between SERVO_MIN_ANGLE and SERVO_MAX_ANGLE) */
void Servo_SetAngle(int16_t degree);

/* Set the servo position based on a percentage (0% to 100%) */
void Servo_SetPercent(int percentage);

void Servo_Home_Position(void);

#endif // SERVO_H
