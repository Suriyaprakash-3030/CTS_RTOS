/*
 * ISR.h
 *
 *  Created on: Sep 3, 2024
 *      Author: mhmaq
 */

#ifndef ISR_H
#define ISR_H

// Include directives for necessary headers, e.g., HAL library
#include <main.h>  // Adjust as per your STM32 series

extern long ServoDC_M1_EncA_Pulses; /// Encoder pulses for Motor 1 ServoDC
extern long ServoDC_M1_EncB_Pulses; /// Encoder pulses for Motor 1 ServoDC

extern long Clamp_M2_EncA_Pulses; /// Encoder pulses for Motor 2 Clamp
extern long Clamp_M2_EncB_Pulses; /// Encoder pulses for Motor 2 Clamp

extern long Spindle_M3_EncA_Pulses; /// Encoder pulses for Motor 3 Spindle
extern long Spindle_M3_EncB_Pulses; /// Encoder pulses for Motor 3 Spindle

extern uint8_t isClampLimitSwitchReached;/// State of Motor 1 limit switch
extern uint8_t isSpindleLimitSwitchReached;/// State of Motor 2 limit switch
extern uint8_t isServoLimitSwitchReached;/// State of Motor 2 limit switch

extern int ClampMotorState; /// State of Motor 1
extern int SpindleMotorState; /// State of Motor 2
/// Function prototypes for ISR callbacks (to be defined as needed)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Blink(void);
#endif // ISR_H
