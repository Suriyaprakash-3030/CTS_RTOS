/**
 * ******************************************************************************
	* @file    ISR.c
	* @brief   This file provides code for the Interrupt Service Routine (ISR) Callbacks
	******************************************************************************
 *  Created on: Sep 3, 2024
 *      Author: mhmaq
 */

#include "main.h"

volatile uint8_t actionTaken = 0;            // Flag to prevent repeated toggling

#define BUTTON_HOLD_TIME_ON 2000             // 2 seconds to turn ON
#define BUTTON_HOLD_TIME_OFF 500


extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim2;


// Copying this to global.c and h
long ServoDC_M1_EncA_Pulses = 1; /// Encoder pulses for Motor 1 ServoDC
long ServoDC_M1_EncB_Pulses = 1; /// Encoder pulses for Motor 1 ServoDC

long Clamp_M2_EncA_Pulses = 1; /// Encoder pulses for Motor 2 Clamp
long Clamp_M2_EncB_Pulses = 1; /// Encoder pulses for Motor 2 Clamp

long Spindle_M3_EncA_Pulses = 1; /// Encoder pulses for Motor 3 Spindle
long Spindle_M3_EncB_Pulses = 1; /// Encoder pulses for Motor 3 Spindle

uint8_t isClampLimitSwitchReached = TRUE; /// State of Clamp Motor 1 limit switch
uint8_t isSpindleLimitSwitchReached = TRUE; /// State of Spindle Motor 2 limit switch
uint8_t isServoLimitSwitchReached = TRUE; /// State of Motor 2 limit switch

int ClampMotorState = OFF; /// State of Motor
int SpindleMotorState = OFF; /// State of Motor 2
int ServoDCMotorState=OFF;
int count1 = 0;
int temp_c11 =0 ,temp_c22 = 0, temp_c33 = 0;


/************************************* Interrupt Handling Functions******************************************************/
/**
 * @brief  GPIO Interrupt Check Callback Function when Triggered for using interrupts.
 * @param  GPIO_Pin :Pin number for the interrupt
 * @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) /* @fn Callback Function*/
{
// Check which pin triggered the interrupt and Callbacks Power Button Interrupts for turning on and turning off the machine
	// Check button state
	if (GPIO_Pin == Button_Power_Interrupt_PullUp_Pin) {
	        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin); // Clear interrupt flag

		if (HAL_GPIO_ReadPin(Button_Power_Interrupt_PullUp_GPIO_Port, Button_Power_Interrupt_PullUp_Pin) == GPIO_PIN_RESET) {
			// Button pressed: Record press time and reset action flag
			PowerButtonPressTime = HAL_GetTick();
			actionTaken = 0;
			SerialPrintln("Power Button Pressed");
		} else {
			// Button released: Reset press time
			PowerButtonPressTime = 0;
			SerialPrintln("Power Button Released");
		}
	}
	/*** CLEAN BUTTON  ACTIONS ***/
/*	if (GPIO_Pin == Button_Clean_Interrupt_PullUp_Pin) {
	        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin); // Clear the interrupt flag
	        static uint32_t buttonPressStartTime = 0; // Track when the button was pressed
	        static uint8_t buttonState = 0;          // 0: Released, 1: Pressed
	        static uint8_t buttonPressCount = 0;     // Tracks button presses during cleaning
	    	uint32_t InitialpressDuration = HAL_GetTick() - buttonPressStartTime; // Calculate press duration
	    	buttonPressStartTime = HAL_GetTick(); // Record press start time
	        if (HAL_GPIO_ReadPin(Button_Clean_Interrupt_PullUp_GPIO_Port, Button_Clean_Interrupt_PullUp_Pin) == GPIO_PIN_RESET) {  //GPIO_PIN_RESET
	            // Button pressed
				if ((buttonState == 0) && (InitialpressDuration >= 100 )) { // Detect initial press
					buttonPressStartTime = HAL_GetTick(); // Record press start time
					buttonState = 1;
					SerialPrintln("Button state \n");
				}
				else {
	            // Button released
				/*	if(buttonState == 1)
					{ // Detect release
						SerialPrintln("button  Started changed \n");
						uint32_t pressDuration = HAL_GetTick() - buttonPressStartTime; // Calculate press duration
						// buttonState = 0;  // check why it was changed here
							if (CleaningOperationFlag == FALSE && pressDuration >= BUTTON_HOLD_TIME_ON )
							{
							  // Not in cleaning mode - check for long press to start cleaning
								//CleaningOperationFlag = TRUE; // Start cleaning
								cleanLongPressed = TRUE;
								UserCleaningCycleNumber = 1;  // Initialize cleaning cycle count
								buttonPressCount = 0;         // Reset button press count
#if DEBUG == 1
								SerialPrintln("Cleaning Started\n");
#endif
							}
						if (buttonState == 1) { // Detect release
							uint32_t pressDuration = HAL_GetTick() - buttonPressStartTime; // Calculate press duration
							buttonState = 0;

							if (CleaningOperationFlag == FALSE && pressDuration >= BUTTON_HOLD_TIME_ON ) {
								// Not in cleaning mode - check for long press to start cleaning
							   // if (pressDuration >= 2000) { // Long press (2 seconds)
								CleaningOperationFlag = TRUE; // Start cleaning
								cleanLongPressed = TRUE;
								UserCleaningCycleNumber = 1;  // Initialize cleaning cycle count
								buttonPressCount = 0;         // Reset button press count
								SerialPrintln("Cleaning Started\n");
							  //  }
							}
							else if(pressDuration <= BUTTON_HOLD_TIME_OFF)
							{
								cleanLongPressed = FALSE;
								UserCleaningCycleNumber = 1;  // Initialize cleaning cycle count
								//buttonPressCount = 0;         // Reset button press count
							}
	                    // Cleaning mode active - handle short presses
							if ((CleaningOperationFlag == TRUE) && (pressDuration > 500))
							{ // Short press
								 buttonPressCount++;
#if DEBUG == 1
								 SerialPrintln("Button Count Cycle");
								 SerialPrintlnINT(buttonPressCount);
#endif
								if (UserCleaningCycleNumber < MAXCLEANCYCLESALLOWED)
								{
									UserCleaningCycleNumber++;
#if DEBUG == 1
									SerialPrint("Increased Cleaning Cycle :");
									SerialPrintlnINT(UserCleaningCycleNumber);
#endif
								 }
								else
								{
									if (buttonPressCount > 3) {
										buttonPressCount = 0;
									}// Reset press count
									if(UserCleaningCycleNumber >= 3)
									{
										UserCleaningCycleNumber = 1;
									}
									else
									{
										//UserCleaningCycleNumber--;
#if DEBUG == 1
										SerialPrintln("Reduced Cleaning Cycle");
										SerialPrintlnINT(UserCleaningCycleNumber);
#endif
									}

								}
								printLCDInt(3,"Cycle No: ",completedCycles, UserCleaningCycleNumber);  // need to test the code
									/*else {
									   if (UserCleaningCycleNumber < MAXCLEANCYCLESALLOWED) {
										   //UserCleaningCycleNumber++;
#if DEBUG == 1
									   SerialPrint("Increased Cleaning Cycle :");
									   SerialPrintlnINT(UserCleaningCycleNumber);
#endif
								 }
						   }
	                }
	            }
	        }
	   }
*/
	if (GPIO_Pin == Button_Clean_Interrupt_PullUp_Pin) {
		        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin); // Clear the interrupt flag
		        SerialPrintln("Cleaning Started at start\n");
		        static uint32_t buttonPressStartTime = 0; // Track when the button was pressed
		        static uint8_t buttonState = 0;          // 0: Released, 1: Pressed
		        static uint8_t buttonPressCount = 0;     // Tracks button presses during cleaning

		        if (HAL_GPIO_ReadPin(Button_Clean_Interrupt_PullUp_GPIO_Port, Button_Clean_Interrupt_PullUp_Pin) == GPIO_PIN_RESET) {
		            // Button pressed
		            if (buttonState == 0) { // Detect initial press
		                buttonPressStartTime = HAL_GetTick(); // Record press start time
		                buttonState = 1;
		                SerialPrintln("Button State Change at start\n");
		            }
		        } else {
		            // Button released
		            if (buttonState == 1) { // Detect release
		                uint32_t pressDuration = HAL_GetTick() - buttonPressStartTime; // Calculate press duration
		                buttonState = 0;

		                if (CleaningOperationFlag == FALSE && pressDuration >= BUTTON_HOLD_TIME_ON ) {
		                    // Not in cleaning mode - check for long press to start cleaning
		                   // if (pressDuration >= 2000) { // Long press (2 seconds)
		                	CleaningOperationFlag = TRUE; // Start cleaning
		                	cleanLongPressed = TRUE;
							UserCleaningCycleNumber = 1;  // Initialize cleaning cycle count
							buttonPressCount = 0;         // Reset button press count
							SerialPrintln("Cleaning Started\n");
		                  //  }
		                } else {
		                    // Cleaning mode active - handle short presses
		                    if (CleaningOperationFlag == TRUE && pressDuration < 500) { // Short press
		                        buttonPressCount++;
							   if (buttonPressCount > 3) {
								   if (UserCleaningCycleNumber > 1) {
									    if(UserCleaningCycleNumber > 3)
									    {
									    	UserCleaningCycleNumber = 1;
									    }
									    //else
									    	//UserCleaningCycleNumber--;
									   //SerialPrintln("Reduced Cleaning Cycle\n");
								   }
								   buttonPressCount = 0; // Reset press count
							   } else {
								   if (UserCleaningCycleNumber < MAXCLEANCYCLESALLOWED) {
									   UserCleaningCycleNumber++;
									   SerialPrintln("Increased Cleaning Cycle\n");
								   }
							   }
		                    }
		                }
		            }
		        }
		    }

/*********************************************************************************************************/
// Clamp Motor Interrupts such that Limit Switch, Feedback Pulses
	else if (GPIO_Pin == Clamp_M2_Prox_Interrupt_Pin)
	{ /// Handle the interrupt caused by Clamp Motor Limit Switch
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if (HAL_GPIO_ReadPin(Clamp_M2_Prox_Interrupt_GPIO_Port, Clamp_M2_Prox_Interrupt_Pin) == 1) {
			Clamp_M2_EncA_Pulses = 0;
			Clamp_M2_EncB_Pulses = 0;
			pwm(Clamp_M2_PWM, 0);
			isClampLimitSwitchReached = TRUE;
			SerialPrint("Clamp Interrupt M2 Limit rising\n");
		} else {
			Clamp_M2_EncA_Pulses = 1;
			Clamp_M2_EncB_Pulses = 1;
			isClampLimitSwitchReached = FALSE;
			SerialPrint("Clamp Interrupt M2 Limit falling\n");
		}
	}
	else if (GPIO_Pin == Clamp_M2_EncA_Interrupt_Pin)
	{ /// Handle the interrupt caused by Clamp Motor Feedback Encoder A
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if(!isClampLimitSwitchReached){
			if(ClampMotorState == CCW){
				Clamp_M2_EncA_Pulses++;
			}
			else if(ClampMotorState == CW && Clamp_M2_EncA_Pulses > 1){
				Clamp_M2_EncA_Pulses--;
			}
		}
	}
	else if (GPIO_Pin == Clamp_M2_EncB__Interrupt_Pin)
		{ /// Handle the interrupt caused by Clamp Motor Feedback Encoder B
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
			if(!isClampLimitSwitchReached){
				if(ClampMotorState == CCW){
					Clamp_M2_EncB_Pulses++;

				}
				else if(ClampMotorState == CW && Clamp_M2_EncB_Pulses > 1){
					Clamp_M2_EncB_Pulses--;
				}
			}
		}

/****************************************************************************************************/
// Spindle Motor Interrupts such that Limit Switch, Feedback Pulses
	else if (GPIO_Pin == Spindle_M3_Prox_Interrupt_Pin)
	{ /// Handle the interrupt caused by Spindle Motor Limit switch
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if (HAL_GPIO_ReadPin(Spindle_M3_Prox_Interrupt_GPIO_Port, Spindle_M3_Prox_Interrupt_Pin) == 1) {
			Spindle_M3_EncA_Pulses=0;
			Spindle_M3_EncB_Pulses=0;
			pwm(Spindle_M3_PWM, 0);
			isSpindleLimitSwitchReached = TRUE;
			SerialPrint("Spindle Interrupt M3 Limit rising\n");
		} else {
			Spindle_M3_EncA_Pulses = 1;
			Spindle_M3_EncB_Pulses = 1;
			isSpindleLimitSwitchReached = FALSE;
			SerialPrint("Spindle Interrupt M3 Limit falling\n");
		}
	}
	else if (GPIO_Pin == Spindle_M3_EncA_Interrupt_Pin)
	{ /// Handle the interrupt caused by Spindle Motor Feedback Encoder A
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if(!isSpindleLimitSwitchReached){
			if(SpindleMotorState == CW){
				Spindle_M3_EncA_Pulses++;
			}
			else if(SpindleMotorState == CCW && Spindle_M3_EncA_Pulses > 1){
				Spindle_M3_EncA_Pulses--;
			}
		}
	}
	else if (GPIO_Pin == Spindle_M3_EncB_Interrupt_Pin)
	{ /// Handle the interrupt caused by Spindle Motor Feedback Encoder B
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if(!isSpindleLimitSwitchReached){
			if(SpindleMotorState == CW){
				Spindle_M3_EncB_Pulses++;
			}
			else if(SpindleMotorState == CCW && Spindle_M3_EncB_Pulses > 1){
				Spindle_M3_EncB_Pulses--;
			}
		}
	}
/************************************************************************************************************************/
// Servo Motor Interrupts such that Limit Switch, Feedback Pulses plus also if the DC is attached instead of Servo

	else if (GPIO_Pin == ServoDC_M1_Prox_Interrupt_Pin)
	{ /// Handle the interrupt caused by Servo Motor Limit Switch
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
		if (HAL_GPIO_ReadPin(ServoDC_M1_Prox_Interrupt_GPIO_Port, ServoDC_M1_Prox_Interrupt_Pin) == 1) {
			isServoLimitSwitchReached = TRUE;
			SerialPrint("Interrupt Servo/DC Limit rising\n");
			pwm(Servo_PWM_OR_DC_M1_EncA_Interrupt,0);
		} else {
			isServoLimitSwitchReached = FALSE;
			SerialPrint("Interrupt Servo/DC Limit falling\n");
		}
	}

/************************************************************************************************************************/
// This part will run only if the DC motor is being used instead of the servo motor.
#ifndef Servo
	else if (GPIO_Pin == Servo_PWM_OR_DC_M1_EncA_Interrupt_Pin)
		{ /// Handle the interrupt caused by Spindle Motor Limit switch
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
			if(!isServoLimitSwitchReached){
				if(ServoDCMotorState == CW){
					ServoDC_M1_EncA_Pulses++;}
				else if(ServoDCMotorState == CCW && ServoDC_M1_EncA_Pulses > 1){
					ServoDC_M1_EncA_Pulses--;
				}
			}
		}
#endif
	else if (GPIO_Pin == ServoDC_M1_EncB_Interrupt_Pin)
		{ /// Handle the interrupt caused by Spindle Motor Feedback Encoder A
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
			if(!isServoLimitSwitchReached){
				if(ServoDCMotorState == CW){
					ServoDC_M1_EncB_Pulses++;
				}
				else if(ServoDCMotorState == CCW && ServoDC_M1_EncB_Pulses > 1){
					ServoDC_M1_EncB_Pulses--;
				}
			}
		}

}

/**
 * @brief  PowerButton Interrupt Check Callback Function when Triggered for using interrupts.
 * @param  None
 * @retval None
*/
void Blink() { // If the power button is not held, we can check for loadcell reading!
	if (HAL_GPIO_ReadPin(Button_Power_Interrupt_PullUp_GPIO_Port, Button_Power_Interrupt_PullUp_Pin) == GPIO_PIN_RESET) { // GPIO_PIN_RESET)
	        // Button is being held
	        uint32_t holdTime = HAL_GetTick() - PowerButtonPressTime;

	        if (!actionTaken) {
	            if (!isMachineOn && holdTime >= BUTTON_HOLD_TIME_ON) {
	                // Turn ON the system if held for 2 seconds
	            	isMachineOn = 1; // Set system state to ON
	            	SerialPrintln("MACHINE ON");
	                actionTaken = 1; // Prevent repeated toggling
	             //just testing
				if (isPowerLEDBlinking == TRUE) { /// If LED supposed to blink
						powerLEDControl(!isPowerLEDOn);
					}
	            } else if (isMachineOn && holdTime >= BUTTON_HOLD_TIME_OFF) {
	                // Turn OFF the system if held for 500 ms
	            	isMachineOn = 0; // Set system state to ON
	            	SerialPrintln("MACHINE OFF");
	            	NVIC_SystemReset();  /// Reset the STM32 when turning off
	                actionTaken = 1; // Prevent repeated toggling
	            }
	        }
	    }

	/*if (isPowerLEDBlinking == TRUE) { /// If LED supposed to blink
		powerLEDControl(!isPowerLEDOn);
	}*/
	if (isScreenLoading == TRUE) {
		size_t len = strlen(DotString);
		if (len < LCD_COLUMN_COUNT) {
			DotString[len] = '.';
			DotString[len + 1] = '\0'; // Ensure string is null-terminated
		} else {
			memset(DotString, 0, strlen(DotString));
		}
		printLCD(0, DotString, NO_SYMBOL);
	}
	static uint8_t skipRead = FALSE;
	static long delTimer = 0;
	static uint8_t dellaststate = FALSE;
	if (skipRead == TRUE) {
		skipRead = FALSE;
	}
/*	else if (monitorCleanKey == TRUE) {
		if (HAL_GPIO_ReadPin(Button_Clean_Interrupt_PullUp_GPIO_Port, Button_Clean_Interrupt_PullUp_Pin) == LOW) {
			cleansRequired = (cleansRequired % MAXCLEANCYCLESALLOWED) + 1;
			skipRead = TRUE;
		}
		char buffer[21]; /// Buffer to hold the formatted string
		sprintf(buffer, "Clean Cycles %d ", cleansRequired);
		printLCD(3, buffer, REFRESH_SYMBOL);
		// printLCD(3, "Clean cycles: " + (String)cleansRequired + " ", REFRESH_SYMBOL);
	}*/
	else if(digitalRead(Button_DelBack_Input_PullUp) == LOW){
		if(dellaststate == FALSE){
			dellaststate = TRUE;
			delTimer = HAL_GetTick();
		}
		else if(HAL_GetTick() > delTimer + 900){
			if(delLongPressed == FALSE){
				delLongPressed = TRUE;
			}
			else if(CleaningFlag == TRUE){
				//Not 1st time then
				//printLCD(3, "Ending cleaning!", 0);
			}

		}
	}
	else{
		dellaststate = FALSE;
	}

}

/**
 * @brief  Timer Callback Function when Triggered for using interrupts.
 * @param  TIM_HandleTypeDef: Timer handler type.
 * @retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){ /// Timer 2 Interrupt, called every 500mS
	 Blink();
	// If the cleaning button is still held, start the cleaning process if not already running
/*		if (CleaningButtonStillHeld && !isCleaning && (currentTime - CleaningButtonPressTime) >= BUTTON_POWER_ACTIVE_TIME) {
			cleanLongPressed == TRUE;
			isCleaning = TRUE;
			SerialPrint("CLEANING PROCESS STARTED\n");
			CleaningButtonStillHeld = FALSE; // Prevent repeated toggling while held
		}
*/
	}
	if (htim->Instance == TIM6) {		        // Increment time counters
		seconds++;
		if (seconds >= 30) {
			seconds = 0;
			minutes++;
			// Update the time to the SD card
			//UpdateTimeToSDCard(hours, minutes, seconds);
			if (minutes >= 60) {
				minutes = 0;
				hours++;
				/*if (hours >= 24) {
					hours = 0; // Reset after 24 hours
				}*/
			}
		}
		log_to_sd_flag = 1;
 }
	if (htim->Instance == TIM7){/// Timer 7 Interrupt, called every 100mS
		// Your code to execute every 100 ms
		Tareweight = Total_Weight;
		if (CleaningOperationFlag == TRUE) {
			//Cleaning exit logic
			if (readKeypadNonBlock(ALL) == ENTERKEY) {
				CleaningExitCounter = CleaningExitCounter + 1;
				if (CleaningExitCounter > 10) //3 Seconds Read button
				{
					CleaningExit = TRUE;
					CleaningCancelFlag = TRUE;
#if DEBUG == 1
                    SerialPrintln("reached 10 times");
#endif
					CleaningExitCounter = 0;
					CleaningExit = FALSE;
				}
			}
			/*
			uint8_t cleancyclepressed = readKeypadNonBlock(NUMPAD);
			  if(cleancyclepressed <= 5 )
			  {
				   UserCleaningCycleNumber = cleancyclepressed;
			  }
			  */
		}
		if(adc_pressure > 100)
		{
			moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve OFF
		}


#if DEBUG == 1
		 SerialPrintINT(Total_Weight);
		 SerialPrint(",");
		 SerialPrintINT(Clamp_M2_EncA_Pulses);
		 SerialPrint(",");
		 SerialPrintINT(Spindle_M3_EncA_Pulses);
		 SerialPrint(",");
		 SerialPrintINT(percent);
		 SerialPrint(",");
		 SerialPrintINT(adc_pressure);
		 SerialPrint(",");
		 SerialPrintINT(adc_CurrentSpindle);
		 SerialPrint(",");
		 SerialPrintINT(adc_CurrentServo);
		 SerialPrint(",");
		 SerialPrintINT(adc_CurrentClamp);
		 SerialPrint(",");
		 SerialPrintINT(Weight_Initial);
		 SerialPrint(",");
		 SerialPrintINT(Target_Weight);
		 SerialPrint(",");
		 SerialPrintlnINT(FreshValveStatus);


#endif
	 }

	if(htim->Instance == TIM10){ /// Timer 10 Interrupt, called every 1000mS - 1 Second
		 count1++;
		 adc_pressure  = pressureSensorReading();
		 int temp_c1 =  analogRead(Curr_Sensor_Spindle_M3);
		 if(temp_c1 > temp_c11)
		 {
			 temp_c11 = temp_c1;
		 }

		 int  temp_c2 =  analogRead(Curr_Sensor_ServoDC_M1);
		 if(temp_c2 > temp_c22)
		 {
			 temp_c22 = temp_c2;
		 }
		 int temp_c3 =  analogRead(Curr_Sensor_Clamp_M2);
		 if(temp_c3 > temp_c33)
		 {
			 temp_c33 = temp_c3;
		 }
		if(count1 == 10){
			 adc_CurrentSpindle = Convert_ADC_To_Current(temp_c11);
			 adc_CurrentClamp =  Convert_ADC_To_Current(temp_c33);
			 adc_CurrentServo =  Convert_ADC_To_Current(temp_c22);
			 temp_c11 = temp_c1 , temp_c22 =temp_c2, temp_c33 = temp_c3;
			 count1 = 0;
		  }
	}
}
