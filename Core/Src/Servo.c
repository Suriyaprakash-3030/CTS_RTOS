/*
 * Servo.c
 *
 *  Created on: Sep 2, 2024
 *      Author: mhmaq
 */

#include "servo.h"

int16_t angle = 0; //Servo Angle
int percent = 0; //Servo Percentage
int pulse = 0;
int16_t current_angle_offset = 0;
extern TIM_HandleTypeDef htim11; // Timer 11 is dedicated to the servo

// Initialize the servo control
void Servo_Init(void) {
	// Start the PWM signal on Timer 11, Channel 1
	HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
}

// Set the servo angle
void Servo_SetAngle(int16_t degree) {
	// Constrain the angle within the defined minimum and maximum limits
	 //HAL_GPIO_WritePin(GPIOC,EN_DCDC_8V_Output_Pin, GPIO_PIN_RESET);
	degree += current_angle_offset;
	if (degree > SERVO_MAX_ANGLE) {
		degree = SERVO_MAX_ANGLE;
	} else if (degree < SERVO_MIN_ANGLE) {
		degree = SERVO_MIN_ANGLE;
	}

	// Map the angle to the pulse width in microseconds
	uint32_t pulseWidth = SERVO_MIN_PULSE_WIDTH + (uint32_t) ((float) (degree - SERVO_MIN_ANGLE)
			/ (float) (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE) * (SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH));

	pulse = pulseWidth;

	// Set the pulse width for PWM
	__HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, pulseWidth);
	 HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
}

// Set the servo position based on a percentage
void Servo_SetPercent(int percentage) {
	static int16_t old_angle = 1000; //Illogical number to ensure its not same as 1st input to function

	// Constrain the percentage to be between 0% and 100%
	if (percentage > 100) {
		percentage = 100;
	} else if (percentage < 0) {
		percentage = 0;
	}
   percent = percentage;
  // Map the percentage to the corresponding angle
  	if(ISSERVOINTERTED == 1){
  		percentage = 100 - percentage;
  	}
	// Map the percentage to the corresponding angle
	int16_t angle = SERVO_MIN_ANGLE + (int16_t) ((float) percentage / 100.0f * (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE));

	Servo_SetAngle(angle - current_angle_offset); // Account for the offset
	// Set the servo angle
	//Servo_SetAngle(angle);

	//Relax servo only if new angle is more than 10 degrees different than old one, plus conditions inside this
	if (abs(old_angle - angle) > 10) {
		if (abs(percentage - ValveOpen) < 10) {
			HAL_Delay(1000);
			if (ISSERVOINTERTED == 1) {
				Servo_SetAngle(SERVO_MAX_ANGLE - 5);
			} else {
				Servo_SetAngle(SERVO_MIN_ANGLE + 5);
			}
		} else if (abs(percentage - ValveClose) < 10) {
			HAL_Delay(1000);
			if (ISSERVOINTERTED == 1) {
				Servo_SetAngle(SERVO_MIN_ANGLE + 5);
			} else {
				Servo_SetAngle(SERVO_MAX_ANGLE - 5);
			}
		}
	}
	old_angle = angle;
}


uint32_t Servo_GetPulseWidth(uint8_t angle) {
    return ((SERVO_MIN_PULSE_WIDTH) + ((SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH) * angle) / 180);
}

// Function to set servo angle (0 to 180 degrees)
void Servo_SetAngle_1(uint8_t angle) {
    if (angle > 180) angle = 180; // Cap the angle to 180 degrees

    uint32_t pulseWidth = Servo_GetPulseWidth(angle);

    // Update the PWM duty cycle (CCR1) with the new pulse width
    __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, pulseWidth);
}

/*
void Servo_Home_Position(void) {
	 static int16_t homing_angle = SERVO_MAX_ANGLE; // Start from maximum angle
    // Drive the servo to the limit switch
    while (HAL_GPIO_ReadPin(ServoDC_M1_Prox_Interrupt_GPIO_Port, ServoDC_M1_Prox_Interrupt_Pin) != 1) {
        // Gradually move the servo angle toward the limit
        static int16_t homing_angle = SERVO_MAX_ANGLE; // Start from maximum angle
        Servo_SetAngle(homing_angle);
        homing_angle--;

        // Ensure the angle does not go below the minimum
        if (homing_angle < SERVO_MIN_ANGLE) {
            homing_angle = SERVO_MIN_ANGLE;
            break; // Exit if the servo reaches the minimum without triggering the limit switch
        }

        HAL_Delay(10); // Small delay to allow the servo to move and stabilize
    }

    // Once the limit switch is triggered, stop the servo
    Servo_SetAngle(SERVO_MIN_ANGLE);

    // Set the servo's current position as zero
    Servo_SetAngle(3); // Update internal logic if necessary to mark zero position
}

*/
void Servo_Home_Position(void) {
    static int16_t homing_angle = SERVO_MAX_ANGLE; // Start from maximum angle

    // Drive the servo to the limit switch
    while (HAL_GPIO_ReadPin(ServoDC_M1_Prox_Interrupt_GPIO_Port, ServoDC_M1_Prox_Interrupt_Pin) != 1){
        // Gradually move the servo angle toward the limit
        Servo_SetAngle(homing_angle);
        homing_angle--;

        // Ensure the angle does not go below the minimum
        if (homing_angle < SERVO_MIN_ANGLE) {
            homing_angle = SERVO_MIN_ANGLE;
            break; // Exit if the servo reaches the minimum without triggering the limit switch
        }

        HAL_Delay(10); // Small delay to allow the servo to move and stabilize
    }
    HAL_Delay(2000);

    // Once the limit switch is triggered, stop the servo
    Servo_SetAngle(homing_angle);
#if DEBUG == 1
    SerialPrint("Servo During Homing");
#endif
   	SerialPrintINT(homing_angle);

    // Update internal logic to mark the current position as zero
    current_angle_offset = homing_angle; // Store the "zero" offset
    angle = 0; // Reset the angle variable to zero for consistency
    percent = 0; // Reset percentage to zero if used in your logic

    // Adjust all future angle calculations to account for the offset
    //Servo_SetAngle(0); // Set servo to the logical zero position
}


