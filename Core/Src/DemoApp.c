/*
 * DemoApp.c
 *
 *  Created on: Sep 6, 2024
 *      Author: mhmaq
 */

#include <main.h>
#include <DemoApp.h>

void DemoApp() {
	isPowerLEDBlinking = TRUE;
	SerialPrintln("DEMO");
	LCDRGBControl(WHITE);
	LCD_Clear();
	LoadingFunc(BLINK, LOADING);
	InitializeMachine();
	LoadingFunc(ON, NOT_LOADING);
	printLCD(0, "Put Cannister On ", ENTER_SYMBOL);
	printLCD(1, " ", NO_SYMBOL);
	printLCD(2, " ", NO_SYMBOL);
	printLCD(3, "DEMO", NO_SYMBOL);
	//While enter key is not pressed. Here we did not use read key func as we want to enter dev menu
	//Dev menu reads keys, and as read key func is a blocking one, it will cause issues
	while (digitalRead(Button_Enter_Input_PullUp) == HIGH) {
		//What is inside this loop is just for testing and developer use, not by customer app
		HAL_Delay(1);
		DeveloperMenu();
	}
	printLCD(3, " ", NO_SYMBOL);

	for (int k = 0; k < 5; k++) {
		LCD_Clear();                //Make sure nothing is written on LCD screen
		printLCD(0, "DEMO APP active", NO_SYMBOL);
		moveMotorClampToLocBlock(M1POS2VAL);
		LoadingFunc(ON, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
		printLCD(1, "Liters: ", CURSOR_DEMO_SYMBOL);
		printLCD(2, "Kg/Liters: ", NO_SYMBOL);
		HAL_Delay(1500);
		printLCD(1, "Liters: 2", CURSOR_DEMO_SYMBOL);
		HAL_Delay(400);
		printLCD(1, "Liters: 2.", CURSOR_DEMO_SYMBOL);
		HAL_Delay(400);
		printLCD(1, "Liters: 2.5", CURSOR_DEMO_SYMBOL);
		HAL_Delay(400);
		printLCD(1, "Liters: 2.5", NO_SYMBOL);
		printLCD(2, "Kg/Liters: ", CURSOR_DEMO_SYMBOL);
		HAL_Delay(400);
		printLCD(2, "Kg/Liters: 1", CURSOR_DEMO_SYMBOL);
		HAL_Delay(400);
		printLCD(2, "Kg/Liters: 1.", CURSOR_DEMO_SYMBOL);
		HAL_Delay(400);
		printLCD(2, "Kg/Liters: 1.1", CURSOR_DEMO_SYMBOL);
		HAL_Delay(400);
		printLCD(2, "Kg/Liters: 1.1", NO_SYMBOL);
		LoadingFunc(BLINK, NOT_LOADING);
		printLCD(0, "0.00 / 2.5L", NO_SYMBOL);
		float i = 0.0;
		char LitersOf[20]; // Ensure the array is large enough for your string
		strcpy(LitersOf, "0.00 / 2.5L"); // Initialize the array with your string

		unsigned long temptmr = HAL_GetTick(); //millis();
		//Clamp motor moves to open valve position
		//Once spindle motor (Cap) penetrates the CAN, then liquid starts flowing
		//While spindle is moving up inside the CAN, the liquid is still flawing
		//We need to show dispensed amount on the LCD, while the spindle is still moving!

		//Move M1 to clamp position POS2
		moveMotorClampToLocBlock(M1POS2VAL);
		if (Clamp_M2_EncA_Pulses >= M1POS2VAL) {
			//Move spindle to POS3
			moveMotorSpindleToLocBlock(M2POS3VAL);
			if (Spindle_M3_EncA_Pulses >= M2POS3VAL) {
				//Open valve servo by 100%
				Servo_SetPercent(ValveOpen);
				//Update LCD with dispensed amount until 2.0 L (quick update) -- liquid rush
				if (HAL_GetTick() > temptmr + 2 && i <= 2.0) { //Update screen only if M1 reaches open position, once every 100mS, and if dosing < needed 2.5L
					temptmr = HAL_GetTick(); //millis();
					sprintf(LitersOf, "%.2f / 2.0L", i); // Formats 'i' as a floating-point and stores in LitersOf
					printLCD(0, LitersOf, NO_SYMBOL);
					i = i + 0.1;
				}
				Servo_SetPercent(15);
				//update LCD with dispensed amount until 2.5 L (slower update) -- precise measurement
				while (i < 2.5) { //Complete dosing
					sprintf(LitersOf, "%.2f / 2.5L", i);
					printLCD(0, LitersOf, NO_SYMBOL);
					HAL_Delay(100);
					i = i + 0.1;
				}
			}
			//close valve servo to 0%
			Servo_SetPercent(ValveClose);
		}
		LoadingFunc(ON, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
		printLCD(0, "Transfer Complete", NO_SYMBOL);
		moveMotorClampToLocBlock(M1POS2VAL);
		HAL_Delay(1000);
		isDemoCleaning = TRUE;
		printLCD(3, "CLEANING", NO_SYMBOL);
		SerialPrintln("X0");
		LoadingFunc(BLINK, LOADING);
		SerialPrintln("X1");
		moveMotorSpindleToLocBlock(M2POS2VAL);
		SerialPrintln("X2");
		moveMotor(VALVEMOTOR, ON, INT_INF); //Relay On
		SerialPrintln("X3");
		moveMotorSpindleToLocBlock(M2POS4VAL);
		SerialPrintln("X4");
		moveMotorSpindleToLocBlock(M2POS3VAL);
		SerialPrintln("X5");
		moveMotorSpindleToLocBlock(M2POS4VAL);
		moveMotor(VALVEMOTOR, OFF, INT_INF); //Relay Off
		moveMotorSpindleToLocBlock(M2POS1VAL);
		moveMotor(VALVEMOTOR, ON, INT_INF); //Relay On
		HAL_Delay(3000);
		moveMotor(VALVEMOTOR, OFF, INT_INF); //Relay Off
		isDemoCleaning = FALSE;
		printLCD(3, " ", NO_SYMBOL);
		moveMotorClampToLocBlock(M1POS1VAL);
		;
		LoadingFunc(ON, NOT_LOADING);
		printLCD(0, "Cannister Unlocked", NO_SYMBOL);
		printLCD(1, "Next Cannister Please", NO_SYMBOL);
		printLCD(2, "Put Cannister On ", ENTER_SYMBOL);
		HAL_Delay(3000);
	}

}

