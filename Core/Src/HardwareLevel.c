/*
 * HardwareLevel.c
 *
 *  Created on: Sep 6, 2024
 *      Author: mhmaq
 */

#include <main.h>

#define ADC_Sample 5

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

void moveMotorSpindleToLocBlock(long posM2) {
	int DelayRequired = FALSE;
	while (!moveMotorSpindleToLocNoneBlock(posM2)) {
		DelayRequired = TRUE;
	}
	if (DelayRequired == TRUE) {
		for(uint16_t j = 0; j < 1000; j++){
			for (uint16_t i = 0; i < 14000; i++){ //14000 is supposed to be 1mS
				__NOP();
			}
		}
	}

}

void moveMotorClampToLocBlock(long posM1) {
	int DelayRequired = FALSE;
	while (!moveMotorClampToLocNoneBlock(posM1)) {
		DelayRequired = TRUE;
	}
	if (DelayRequired == TRUE) {
		for(uint16_t j = 0; j < 1000; j++){
			for (uint16_t i = 0; i < 14000; i++){ //14000 is supposed to be 1mS
				__NOP();
			}
		}
	}
}

uint8_t moveMotorSpindleToLocNoneBlock(long posM2) {

	int M2Dir;
	uint8_t isSuccess = FALSE; // 0 for false, 1 for true

	if (posM2 == M2POS1VAL && Spindle_M3_EncA_Pulses > M2POS1VAL) { //Going back home
		M2Dir = CCW;
	}
	else if (abs(Spindle_M3_EncA_Pulses - posM2) <= M2ACCEPTEDERROR || posM2 == NONE) {
		M2Dir = OFF;
		posM2 = Spindle_M3_EncA_Pulses;
		moveMotor(CAPMOTOR, OFF, INT_INF);
		isSuccess = TRUE;
		return isSuccess;
	}
	else if (Spindle_M3_EncA_Pulses < posM2) {
		M2Dir = CW; //before CW changed while testing
	}
	else {
		M2Dir = CCW; //before CCw
	}

	if (Spindle_M3_EncA_Pulses != posM2) {

		// Motor 2 Movement Logic
		if (M2Dir == CW) {
			if (Spindle_M3_EncA_Pulses < posM2) {
				moveMotor(CAPMOTOR, CW, posM2);
			} else {
				moveMotor(CAPMOTOR, OFF, INT_INF);
				posM2 = Spindle_M3_EncA_Pulses;
			}
		} else if (M2Dir == CCW) {
			if (Spindle_M3_EncA_Pulses > posM2) {
				moveMotor(CAPMOTOR, CCW, posM2);
			} else {
				moveMotor(CAPMOTOR, OFF, INT_INF);
				posM2 = Spindle_M3_EncA_Pulses;
			}
		} else {
			moveMotor(CAPMOTOR, OFF, INT_INF);
			posM2 = Spindle_M3_EncA_Pulses;
		}
	} else {
		moveMotor(CAPMOTOR, OFF, INT_INF);
		isSuccess = TRUE;
	}
	return isSuccess;
}

uint8_t moveMotorClampToLocNoneBlock(long posM1) {
	int M1Dir;

	uint8_t isSuccess = FALSE; // 0 for false, 1 for true

	if (posM1 == M1POS1VAL && Clamp_M2_EncA_Pulses > M1POS1VAL) { //Going back home
		M1Dir = CW;
	} else if (abs(Clamp_M2_EncA_Pulses - posM1) <= M1ACCEPTEDERROR || posM1 == NONE) {
		M1Dir = OFF;
		posM1 = Clamp_M2_EncA_Pulses;
		moveMotor(CLAMPMOTOR, OFF, INT_INF);
		isSuccess = TRUE;
		SerialPrintln("SUCCESS CLAMP");
		return isSuccess;
	} else if (Clamp_M2_EncA_Pulses < posM1) {
		M1Dir = CCW;
	} else {
		M1Dir = CW;
	}

	if (Clamp_M2_EncA_Pulses != posM1) {

		// Motor 1 Movement Logic
		if (M1Dir == CCW) {
			if (Clamp_M2_EncA_Pulses < posM1) {
				moveMotor(CLAMPMOTOR, CCW, posM1);
			} else {
				moveMotor(CLAMPMOTOR, OFF, INT_INF);
				posM1 = Clamp_M2_EncA_Pulses;
			}
		} else if (M1Dir == CW) {
			if (Clamp_M2_EncA_Pulses > posM1) {
				moveMotor(CLAMPMOTOR, CW, posM1);
			} else {
				moveMotor(CLAMPMOTOR, OFF, INT_INF);
				posM1 = Clamp_M2_EncA_Pulses;
			}
		} else {
			moveMotor(CLAMPMOTOR, OFF, INT_INF);
			posM1 = Clamp_M2_EncA_Pulses;
		}

		isSuccess = FALSE;
	} else {
		SerialPrintln("POSITION REACHED");
		moveMotor(CLAMPMOTOR, OFF, INT_INF);
		isSuccess = TRUE;
	}
	return isSuccess;
}

void moveMotor(int motorNum, int Dir, int targetPoint) {
	if (motorNum == CLAMPMOTOR) {
		if (Dir == CW) {
			ClampMotorState = CW;
			digitalWrite(Clamp_M2_Dir_Output, LOW);
		} else if (Dir == CCW) {
			ClampMotorState = CCW;
			digitalWrite(Clamp_M2_Dir_Output, HIGH);
		} else {
			pwm(Clamp_M2_PWM, OFF);
		}

		if (Dir != OFF) {
			checkFaultsMotors(CLAMPMOTOR);
			if (abs(Clamp_M2_EncA_Pulses - targetPoint) < MOVEM1SLOWPULSES) { //If we are close to our target, move slowly
				pwm(Clamp_M2_PWM, PWM_SLOW);
			} else { //Else go with high speed
				pwm(Clamp_M2_PWM, PWM_FAST);
			}
		}

	} else if (motorNum == CAPMOTOR) {
		if (Dir == CW) {
			SpindleMotorState = CW;
			digitalWrite(Spindle_M3_Dir_Output, LOW);
		} else if (Dir == CCW) {
			SpindleMotorState = CCW;
			digitalWrite(Spindle_M3_Dir_Output, HIGH);
		} else {
			pwm(Spindle_M3_PWM, OFF);
		}

		if (Dir != OFF) {
			//checkFaultsMotors(CAPMOTOR); //commented as we are not using the Analog yet as the functionality is commented
			if (abs(Spindle_M3_EncA_Pulses - targetPoint) < MOVEM2SLOWPULSES) { //If we are close to our target, move slowly
				pwm(Spindle_M3_PWM, PWM_SLOW);
			} else if(CleaningFlag == TRUE){ //Else go with high speed
				pwm(Spindle_M3_PWM, PWM_CLEANING);
			}
			else{ //Else go with high speed
				pwm(Spindle_M3_PWM, PWM_FAST);
			}
		}
	} else if (motorNum == VALVEMOTOR) {
		if (Dir == ON) {
			digitalWrite(Relay_Output, HIGH);
			strcpy(ValveStatus, "Open");
			FreshValveStatus = 1;
		} else {
			digitalWrite(Relay_Output, LOW);
			strcpy(ValveStatus, "Close");
			FreshValveStatus = 0;

		}
	}

}


void checkFaultsMotors(uint8_t x) {

	GetADC();
	// Used to check pause status
	ReadKeypadVar(CONTROL);
	if (isPausedOperation == TRUE) {
		printLCD(3, "PAUSE", NO_SYMBOL);
		int isvalveopenOld = isValveOpen; // Store last valve state

		while (isPausedOperation == TRUE) {
			SerialPrintln("STUCK IN PAUSE");
			ReadKeypadVar(CONTROL);
			moveMotor(CAPMOTOR, OFF, INT_INF);
			moveMotor(CLAMPMOTOR, OFF, INT_INF);
			moveMotor(VALVEMOTOR, OFF, INT_INF);
		}

		// Display relevant information on LCD based on the operation mode
		if (isDemoCleaning == TRUE) {
			printLCD(3, "CLEANING", NO_SYMBOL);
		} else {
			printLCD(3, " ", NO_SYMBOL);
		}

		// Resume operation after pause
		moveMotor(VALVEMOTOR, isvalveopenOld, INT_INF); // Revert valve to its previous state
	}
}

void GetADC()
{

}


