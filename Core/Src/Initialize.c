/*
 * Initialize.c
 *
 *  Created on: Sep 6, 2024
 *      Author: mhmaq
 */

#include <main.h>
#include <Initialize.h>

extern const char valuetosend;
void InitializeMachine2(void);

void InitializeMachine() {
	InitComp = FALSE;
	LCD_Init();
		do{
			//UpdateTimeToSDCard( hours,  minutes,  seconds);
			InitializeMachine2();
			HAL_Delay(1000);
		}while(isClampLimitSwitchReached == FALSE || isSpindleLimitSwitchReached == FALSE);

		InitComp = TRUE;
}


void CTS_Home_position(void)
{
	 while(HAL_GPIO_ReadPin(Spindle_M3_Prox_Interrupt_GPIO_Port, Spindle_M3_Prox_Interrupt_Pin) != 1)
	  {
		   digitalWrite(Spindle_M3_Dir_Output, HIGH); //LOW
		   pwm(Spindle_M3_PWM, 90);
	  }
	 digitalWrite(Spindle_M3_Dir_Output, LOW); //HIGH
	 pwm(Spindle_M3_PWM, 0);
		while(HAL_GPIO_ReadPin(Clamp_M2_Prox_Interrupt_GPIO_Port, Clamp_M2_Prox_Interrupt_Pin) != 1)
		{
		   digitalWrite(Clamp_M2_Dir_Output, LOW);
		   pwm(Clamp_M2_PWM,50);
		}
		digitalWrite(Clamp_M2_Dir_Output, HIGH);
		pwm(Clamp_M2_PWM, 0);

		Servo_Home_Position();
		//HAL_Delay(2000);// this is delaying the system
}

void InitializeMachine2(void) {//Close valves, Drive down the Probe, Open clamps, tare load cells
    // Print beginning initialization message
#if DEBUG == 1
    SerialPrintln("Begin Initialize");
#endif
    HAL_Delay(500);
    Clamp_M2_EncA_Pulses = 1;
    Clamp_M2_EncB_Pulses = 1;
    Spindle_M3_EncA_Pulses = 1;
    Spindle_M3_EncB_Pulses = 1;
    // Make sure valve is closed
    moveMotor(VALVEMOTOR, OFF, INT_INF); // Assuming 0 is a valid target point for VALVEMOTOR

    moveMotor(VALVEMOTOR, OFF, INT_INF);
#if DEBUG == 1
    SerialPrintln("Read limits");
#endif
    if (HAL_GPIO_ReadPin(Clamp_M2_Prox_Interrupt_GPIO_Port, Clamp_M2_Prox_Interrupt_Pin) == 1) {
		Clamp_M2_EncA_Pulses = 0;
		Clamp_M2_EncB_Pulses = 0;
		pwm(Clamp_M2_PWM, 0);
		isClampLimitSwitchReached = TRUE;
#if DEBUG == 1
    	SerialPrint("M2 Limit HIGH\n");
#endif
    } else {
    	isClampLimitSwitchReached = FALSE;
#if DEBUG == 1
    	SerialPrint("M2 Limit LOW\n");
#endif
    }

    if (HAL_GPIO_ReadPin(Spindle_M3_Prox_Interrupt_GPIO_Port, Spindle_M3_Prox_Interrupt_Pin) == 1) {
		Spindle_M3_EncA_Pulses=0;
		Spindle_M3_EncB_Pulses=0;
		pwm(Spindle_M3_PWM, 0);
		isSpindleLimitSwitchReached = TRUE;
#if DEBUG == 1
    	SerialPrint("M3 Limit HIGH\n");
#endif
    } else {
    	isSpindleLimitSwitchReached = FALSE;
#if DEBUG == 1
    	SerialPrint("M3 Limit LOW\n");
    	SerialPrintln("Servo Close");
#endif
    	Servo_SetPercent(90); //Normally set to ValveClose
    }
#if DEBUG == 1
    SerialPrintln("Check M3 position");
#endif
    // Read the state of Motor2 limit switch and reset its position
    if (isSpindleLimitSwitchReached == FALSE) {
    	moveMotorSpindleToLocBlock(M2POS1VAL);
    }
#if DEBUG == 1
    SerialPrintln("Check M1 position");
#endif
    // Read the state of Motor1 limit switch and reset its position
    if (isClampLimitSwitchReached == FALSE) {
        moveMotorClampToLocBlock(M1POS1VAL);
    }
#if DEBUG == 1
    SerialPrintln("Servo Close");
#endif
    Servo_SetPercent(90);  //Normally set to ValveClose

    // Delay to give time for load cell values to stabilize after motor movement ends
    HAL_Delay(1); // Use HAL_Delay for STM32
    //SerialPrintln("Zero load cells");
#if DEBUG == 1
    // Print finishing initialization message
    SerialPrintln("Finish Initialize");
#endif
}

void SubInit(){								//Initialize LCD
	HAL_Delay(500);
	LCD_Clear();								//Clear LCD
	HAL_Delay(500);
	HAL_Delay(1000);     // Wait for a second after initializing
	return;
}

void goToSleep() { //Function goToSleep is only called by setup as it is a blocking function
    // Turn OFF LCD RGB
    LCDRGBControl(OFF);
    pwm(LCD_LED_PWM,0);
    pwm(LCD_Contrast_PWM,0);
    // Turn off power LED and stop loading screen
    LoadingFunc(OFF, NOT_LOADING);
    // Initialize the machine (the same initialization is used for shutdown and startup)
    InitializeMachine();
    // Print message to Serial
#if DEBUG == 1
    SerialPrintln("Sleeping..");
#endif
    HAL_Delay(500);   // Delay for a short period
    //UpdateTimeToSDCard( hours,  minutes,  seconds);
    // Wait while the machine is off
    while(isMachineOn == FALSE) {
        HAL_Delay(100); // Use HAL_Delay instead of delay
    }
    LoadingFunc(BLINK, LOADING);
    // On waking up, print message to Serial
#if DEBUG == 1
    SerialPrintln("Waking up..");
#endif
	 MX_TIM8_Init();
	 LCD_Init();
	 LCDRGBControl(WHITE);
	 controlLCDContrast(100);
    InitializeMachine();
}
