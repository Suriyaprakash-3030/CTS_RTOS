/*
 * Test.c
 *
 *  Created on: Sep 6, 2024
 *      Author: mhmaq
 */

#include <main.h>
#include "CustomerApp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void DeveloperMenu(void);
int EnterPassword(void);
static uint64_t StoringTimedev = 0;

extern TaskHandle_t xCustomerAppTaskEndHandle;


void MotorTest() {
	//TareAll();
	LCDRGBControl(WHITE); //Turn on LCD backlight and set it to color
	printLCD(0, "Initializing", NO_SYMBOL);
	//TareAll();
	while (TRUE) {
		 if (HAL_GetTick() - StoringTimedev >= 1000) {  // Check if 2 seconds passed
			 StoringTimedev = HAL_GetTick();  // Update the last tick time
			 tareSend = Tareweight;  // Update the variable
		  }
		DeveloperMenu();
		ReadKeypadVar(CONTROL);
		if (lastKeyPressed == PLAYSTOPKEY) {
			   	TareAll();
				printLCD(0, "TARE", NO_SYMBOL);
				//TareAll();
				HAL_Delay(1000);
				lastKeyPressed = 255;
				SerialPrintln("ALways here");
				CleaningExitCounter =0;
		}

		if (isAutoOperation == FALSE) {
			int var = readKeypadNonBlock(ALL);
			if (var == DELKEY) {
				lastKeyPressed = DELKEY;
				uint32_t hours, minutes, seconds;
				LoadTimeFromSDCard(&hours,&minutes,&seconds);
				// Display the time on the LCD
				LCD_Clear();
				printLCD(1, "System Run Time", NO_SYMBOL);
				char time_display[20];
				snprintf(time_display, sizeof(time_display), "Time: %02lu:%02lu:%02lu", hours, minutes, seconds);
				printLCD(2, time_display, NO_SYMBOL);
				while(readKeypadNonBlock(ALL) != ENTERKEY);
			} else if (var == MANAUTOKEY) {
				isAutoOperation = !isAutoOperation;
				HAL_Delay(1000);
			}
			else if (var == PLAYSTOPKEY) {
				//lastKeyPressed = PLAYSTOPKEY;
				TareAll();
				printLCD(0, "TARE", NO_SYMBOL);
				HAL_Delay(1000);
				lastKeyPressed = 255;
				CleaningExitCounter =0;
			}
			char buffer1[21]; // Buffer to hold the formatted string
			char buffer2[21]; // Buffer to hold the formatted string
			sprintf(buffer1, "MC-W:%ld S:%d", Total_Weight,percent); //Point # 7
			printLCD(0, buffer1, NO_SYMBOL);
			printLCD(1, "CAP    CLAMP   SERVO", NO_SYMBOL);
			printLCD(2, "0/8     4/6      7/9", NO_SYMBOL);
			sprintf(buffer2, "Valve: 1/3 %s", ValveStatus);
			printLCD(3, buffer2, NO_SYMBOL);

			//Make sure safety is active and motor turns off whenever it reaches min or max positions
			if ((Spindle_M3_EncA_Pulses <= M2POS1VAL|| Spindle_M3_EncB_Pulses <= M2POS1VAL) || (Spindle_M3_EncA_Pulses >= M2POS4VAL || Spindle_M3_EncB_Pulses >= M2POS4VAL)) {
				moveMotor(CAPMOTOR, OFF, INT_INF);
			}
			if (Clamp_M2_EncA_Pulses <= M1POS1VAL  || percent >= ValveOpen) {
				moveMotor(CLAMPMOTOR, OFF, INT_INF);
			}
			if (var == 0) {
				while (readKeypadNonBlock(ALL) == 0) {
					if (Spindle_M3_EncA_Pulses > M2POS1VAL || Spindle_M3_EncB_Pulses > M2POS1VAL) {
						moveMotor(CAPMOTOR, CCW, M2POS1VAL);

					} else {
						moveMotor(CAPMOTOR, OFF, INT_INF);
					}
				}
				moveMotor(CAPMOTOR, OFF, INT_INF);
			}
			else if (var == 8) {
				while (readKeypadNonBlock(ALL) == 8) {
					if (Spindle_M3_EncA_Pulses < M2POS4VAL ||  Spindle_M3_EncB_Pulses < M2POS4VAL) {
						moveMotor(CAPMOTOR, CW, M2POS4VAL);
					} else {
						moveMotor(CAPMOTOR, OFF, INT_INF);
					}
				}
				moveMotor(CAPMOTOR, OFF, INT_INF);
			}

			if (var == 4) {
				while (readKeypadNonBlock(ALL) == 4) {
					if (Clamp_M2_EncA_Pulses > M1POS1VAL || Clamp_M2_EncB_Pulses > M1POS1VAL) {
						SerialPrintlnINT(Clamp_M2_EncA_Pulses);
						moveMotor(CLAMPMOTOR, CW, M1POS1VAL);
					} else {
						moveMotor(CLAMPMOTOR, OFF, INT_INF);
					}
				}
				moveMotor(CLAMPMOTOR, OFF, INT_INF);
			}
			else if (var == 6) {
				while (readKeypadNonBlock(ALL) == 6) {
					if (Clamp_M2_EncA_Pulses < (M1POS2VAL)  || Clamp_M2_EncB_Pulses < (M1POS2VAL)) {
						SerialPrintlnINT(Clamp_M2_EncA_Pulses);
						moveMotor(CLAMPMOTOR, CCW, (M1POS2VAL));
					} else {
						moveMotor(CLAMPMOTOR, OFF, INT_INF);
					}
				}
				moveMotor(CLAMPMOTOR, OFF, INT_INF);
			}
			else if (var == 9) { //Which thing should be controlled with it?
				while (readKeypadNonBlock(ALL) == 9) {
					if(ValveOpen > ValveClose){
						if (percent <= ValveOpen) {
							Servo_SetPercent(percent +1);//(angle + 1);
							HAL_Delay(20);
						} else { //Do Nothing?
							pwm(Servo_PWM_OR_DC_M1_EncA_Interrupt, 0);
						}
					}
					else{
						if (percent >= ValveOpen) {
							Servo_SetPercent(percent -1);
							HAL_Delay(20);
						} else { //Do Nothing?
							pwm(Servo_PWM_OR_DC_M1_EncA_Interrupt, 0);
						}
					}
				}
				//moveMotor(CLAMPMOTOR, OFF, INT_INF);
				pwm(ServoDC_M1_PWM, 0);

			}
			else if (var == 7) { //Which thing should be controlled with it?
				while (readKeypadNonBlock(ALL) == 7) {
					if(ValveOpen > ValveClose){
						if (percent >= ValveClose) {
							Servo_SetPercent(percent - 1);
							HAL_Delay(20);
						} else { //Do Nothing?
							pwm(Servo_PWM_OR_DC_M1_EncA_Interrupt, 0);
						}
					}
					else{
						if (percent <= ValveClose) {
							Servo_SetPercent(percent + 1);
							HAL_Delay(20);
						} else { //Do Nothing?
							pwm(Servo_PWM_OR_DC_M1_EncA_Interrupt, 0);
						}
					}
				}
				//moveMotor(CLAMPMOTOR, OFF, INT_INF);
				pwm(Servo_PWM_OR_DC_M1_EncA_Interrupt, 0);

			} else if (var == 1) {
				//moveMotor(VALVEMOTOR, OFF, INT_INF);		//Relay Control ON
			} else if (var == 3) {
				//moveMotor(VALVEMOTOR, ON, INT_INF);	//Relay Control OFF
			}
		} else {
			ReadKeypadVar(ALL);
			char buffer[21]; // Buffer to hold the formatted string
			char buffer2[21]; // Buffer to hold the formatted string
			sprintf(buffer, "AC-W:%ld S:%d", Total_Weight,percent);
			printLCD(0, buffer, NO_SYMBOL);
			printLCD(1, "CAP    CLAMP   SERVO", NO_SYMBOL);
			printLCD(2, "0,2,5,8   4/6    7/9", NO_SYMBOL);
			sprintf(buffer2, "Valve: 1/3 %s", ValveStatus);
			printLCD(3, buffer2, NO_SYMBOL);

			switch(lastKeyPressed){
			case  4:
				moveMotorClampToLocBlock(M1POS1VAL);
				break;
			case  6:
				moveMotorClampToLocBlock(M1POS2VAL);
				break;

			case  1:
				moveMotor(VALVEMOTOR, OFF, INT_INF);	//Relay Control ON
				break;

			case  3:
				moveMotor(VALVEMOTOR, ON, INT_INF);		//Relay Control OFF
				break;

			case  7:
				Servo_SetPercent(percent - 10);
				break;
			case  9:
				Servo_SetPercent(percent + 10);
				break;
			case  0:
				moveMotorSpindleToLocBlock(M2POS1VAL);
				break;

			case  2:
				moveMotorSpindleToLocBlock(M2POS2VAL);
				break;

			case  5:
				moveMotorSpindleToLocBlock(M2POS3VAL);
				break;

			case  8:
				moveMotorSpindleToLocBlock(M2POS4VAL);
				break;

			case  10:
				xTaskNotifyGive(xCustomerAppTaskEndHandle);
				break;
			case  13:
				TareAll();
				break;
			default :
				lastKeyPressed =255;
				break;
		 }
		}
	}

}

void DeveloperMenu(void) { //Enter developer menu under next condition
	int pass = 0;
	if (digitalRead(Button_ManAuto_Input_PullUp) == LOW) { // Man/Auto, comma and delete are all pressed at same time
		if (digitalRead(Button_Comma_Input_PullUp) == LOW) {
			if (digitalRead(Button_DelBack_Input_PullUp) == LOW) {
				pass = EnterPassword();
				if (pass == 4869) { //Then this password is entered
					MotorTest();
				} else if (pass == 1234) {
					//DemoApp();
				} else if (pass == 1412) {
					//CustomerFrontEnd();
				}
			}
		}
	}
}

int EnterPassword(void) {
	int Key = 0;
	char stars[21] = { '\0' }; // Allocate a char array for the password asterisks
	char buffer[21];          // Buffer for converting numbers to strings

	LCD_Clear();
	printLCD(0, "Enter password:", NO_SYMBOL);
	ReadKeypadVar(ALL);

	while (lastKeyPressed != ENTERKEY) {
		ReadKeypadVar(ALL);
		if (lastKeyPressed >= 0 && lastKeyPressed <= 9) {
			Key = Key * 10 + lastKeyPressed;

			int len = strlen(stars);
			if (len < sizeof(stars) - 1) {
				stars[len] = '*';
				stars[len + 1] = '\0';  // Null-terminate the string
			}

			printLCD(1, stars, NO_SYMBOL);
		}
		// If DELKEY is pressed, remove the last entered character
		if (lastKeyPressed == DELKEY) {
			Key /= 10;

			int len = strlen(stars);
			if (len > 0) {
				stars[len - 1] = '\0';
			}
			printLCD(1, stars, NO_SYMBOL);
		}
	}
	// Convert integer Key to string and print
	snprintf(buffer, sizeof(buffer), "Pass: %d", Key);
	//SerialPrint(buffer);

	return Key;
}
