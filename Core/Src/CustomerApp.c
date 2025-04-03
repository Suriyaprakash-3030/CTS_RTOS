/*
 * CustomerApp.c
 *
 *  Created on: Sep 12, 2024
 *      Author: mhmaq
 */

#include <main.h>
#include <CustomerApp.h>
#include <math.h>
#include <FreeRTOS.h>
#include <string.h>
#include "RTOS.h"
#include "queue.h"
#include "task.h"

static uint64_t StoringTime = 0;

static void cleaningCycleProcess(void);
static void capCleaning_Servo(void);
static void UnlockCannister(void);
static void cleanOperation(void);
static void CustomerAppSub(void);
static void autoOperation(void);
static void manualOperation(void);
static void servoCleaning(void);






void CustomerFrontEnd(void *pvParameters)
{
    LCDMessage_t msg;

    LCDRGBControl(WHITE);
    TareAll();

    while (TRUE)
    {
        LoadingFunc(BLINK, LOADING);
        LCD_Init();
        LCD_Clear();

        do
        {
            InitializeMachine();
            LoadingFunc(ON, NOT_LOADING);
            vTaskDelay(pdMS_TO_TICKS(10));
            LCD_Clear();

            // Send "Can on" message
            strcpy(msg.message, "Can on");
            msg.symbol = ENTER_SYMBOL;
            xQueueSend(LCD_Q, &msg, portMAX_DELAY);

            while (digitalRead(Button_Enter_Input_PullUp) == HIGH)
            {
            	vTaskDelay(pdMS_TO_TICKS(10));
                DeveloperMenu();

                if (HAL_GetTick() - StoringTime >= 1000)
                {
                    StoringTime = HAL_GetTick();
                    tareSend = Tareweight;
                }

                if (readKeypadNonBlock(ALL) == PLAYSTOPKEY)
                {
                    // Send "TARE" message
                    strcpy(msg.message, "TARE");
                    msg.symbol = NO_SYMBOL;
                    xQueueSend(LCD_Q, &msg, portMAX_DELAY);

                    SubInit();
                    TareAll();
                    vTaskDelay(pdMS_TO_TICKS(500));

                    // Send "Can on" message again
                    strcpy(msg.message, "Can on");
                    msg.symbol = ENTER_SYMBOL;
                    xQueueSend(LCD_Q, &msg, portMAX_DELAY);
                }

                printLCDInt(2, "Curr.Weigh ", Tareweight, 0);
                UpdateTimeToSDCard(hours, minutes, seconds);
            }

            Unclamped_Weight = Total_Weight;
            LoadingFunc(BLINK, LOADING);
            moveMotorClampToLocBlock(M1POS2VAL);
            LoadingFunc(BLINK, LOADING);
            vTaskDelay(pdMS_TO_TICKS(1000));

            if (Total_Weight <= NO_BOTTLE_WEIGHT)
            {
                LoadingFunc(BLINK, NOT_LOADING);
                LCD_Clear();

                // Send "no can" message
                strcpy(msg.message, "no can");
                msg.symbol = NO_SYMBOL;
                xQueueSend(LCD_Q, &msg, portMAX_DELAY);

                vTaskDelay(pdMS_TO_TICKS(5000));
                LoadingFunc(BLINK, NOT_LOADING);
                moveMotorSpindleToLocBlock(M2POS1VAL);
                vTaskDelay(pdMS_TO_TICKS(500));
                UnlockCannister();
            }
        } while (Total_Weight <= NO_BOTTLE_WEIGHT);

        Servo_SetPercent(90);
        isAutoOperation = TRUE;
       // CustomerApp();
    }

    UpdateTimeToSDCard(hours, minutes, seconds);
}




void CustomerApp(void)
{
	LCD_Clear();
	ReadKeypadVar(ALL);
	LoadingFunc(ON, NOT_LOADING);
	if(Total_Weight > EMPTY_BOTTLE_WEIGHT)
	{
		while (ReadKeypadVar(CONTROL) != ENTERKEY && cleanLongPressed == FALSE)
		{
			if (isAutoOperation == FALSE)
			{
				printLCD(0, "complete ", ENTER_SYMBOL);
			}
			else
			{
				printLCD(0, "automatic ", ENTER_SYMBOL);
			}
			printLCD(3, "Clean ", NO_SYMBOL);
			if(readKeypadNonBlock(CONTROL) == DELKEY)
			{
				moveMotorClampToLocBlock(M1POS1VAL);
//				CustomerFrontEnd();
				cleanLongPressed = 255;
				CleaningExitCounter = 0;
			}

			/*if(cleanLongPressed == TRUE)
			{
				break;
			}*/
		}
	}
	else
	{
		while (cleanLongPressed == FALSE)
		{
			ReadKeypadVar(CONTROL);
			printLCD(0, "CAN Empty! ", ENTER_SYMBOL);
			printLCD(3, "Cleaning? ", NO_SYMBOL);
			if(readKeypadNonBlock(CONTROL) == ENTERKEY)
			{
//				CustomerFrontEnd();
				cleanLongPressed = 255;
				CleaningExitCounter = 0;
				isAutoOperation = TRUE;
				break;
			}
		}
	}
		printLCD(3, " ", NO_SYMBOL);
		if (cleanLongPressed == TRUE) {
#if DEBUG == 1
			SerialPrint("CLEANING PROCESS STARTED\n"); //If cleaning is selected
#endif
			LCD_ClearStringBuffer();
			cleanOperation();
			cleanLongPressed = FALSE;
		}
		else if (isAutoOperation == FALSE) {
			LCD_ClearStringBuffer();
			manualOperation();                         //Manual Operation Is Selected
			LCD_ClearStringBuffer();
		}
		else {
			LCD_ClearStringBuffer();
			autoOperation();                          //Automatic Operation is Selected
			LCD_ClearStringBuffer();
		}
	UpdateTimeToSDCard( hours,  minutes,  seconds);
}

static int operationCancelled()
{
	if(CleaningCancelFlag || adc_pressure > 100 ){
			printLCD(3, "Stop", NO_SYMBOL);
		   return 0;
		}
	return 1;
}

static void manualOperation(void)
{
	HAL_Delay(2000);
	LCD_Clear();
	CleaningCancelFlag = FALSE;

	LoadingFunc(BLINK, LOADING);
	moveMotorClampToLocBlock(M1POS2VAL);
	Servo_SetPercent(ValveOpen);
	moveMotorSpindleToLocBlock(M2POS9VAL);
	HAL_Delay(500);
	LCD_Clear();
	int Total_Weight_Old = Total_Weight;
	HAL_Delay(2500);
	while (Total_Weight_Old - Total_Weight > 25) {
		Total_Weight_Old = Total_Weight;
		HAL_Delay(1000);
	}
	//moveMotor(VALVEMOTOR, ON, INT_INF); 			//Valve ON switching it on before so that the fresh water starts before the spindle moves down
	//moveMotorSpindleToLocBlock(M2POS2VAL); 			// Requirement from customer to stay position 2
	ManualOperation = TRUE;
	CleaningOperationFlag = TRUE;
	cleaningCycleProcess();
	uint32_t lastTime = 0;
	moveMotor(VALVEMOTOR, OFF, INT_INF); 			//Valve OFF // switching it off before so that water is not not present after cleaning
	moveMotorSpindleToLocBlock(M2POS1VAL);  		//M2 to P2
	lastTime = HAL_GetTick();
	while(HAL_GetTick() - lastTime < 2000) {}		//  non-blocking like serial print.}; //M2 to P4 - 1st time;
	servoCleaning();
	lastTime = HAL_GetTick();
	while(HAL_GetTick() - lastTime < 2000) {}		//  non-blocking like serial print.}; //M2 to P4 - 1st time;
	UserCleaningCycleNumber = 1;
	CleaningCancelFlag = TRUE;
	monitorEnterKey = FALSE;  						//Stop monitoring enter key presses in background
	monitorCleanKey = FALSE;  						//Stop monitoring clean key presses in background
	EnterPressed = FALSE;     						//Clean any presses of enter key in memory
	//Excel 44 - 52
	//capCleaning();  NO cap cleaning as requested by customer on 17.01.2025
	LoadingFunc(ON, NOT_LOADING); 					//Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	Servo_SetPercent(90);
	CleaningFlag = FALSE;
	UnlockCannister();
	HAL_Delay(100);
	SubInit();
	if(readKeypadNonBlock(CONTROL) == DELKEY)
	{
		LCD_Clear();
		return;
	}
}

//Excel 57 - 88
static void autoOperation(void)
{
	LCD_Clear();
	Servo_SetPercent(90);
	HAL_Delay(2000);
	LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	Servo_SetPercent(90); // Open valve at automatic (Servo before Cap Opening).
	HAL_Delay(2000);
	printLCD(0, " ", NO_SYMBOL);  //Clean 1st line
	printLCD(3, " ", NO_SYMBOL);  //Clean 4th line
	LoadingFunc(ON, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	Grams_Required = 0;
	while (Grams_Required <= DEAD_WEIGHT || Grams_Required > 100000) {
		Take2RowEntries("Liters: ", "kg/Liters: "); //Saves entries in LitersVar and DensityVar.
		Grams_Required = LitersVar * DensityVar * 1000.0; //Required weight is liters * density
		if(Grams_Required <= DEAD_WEIGHT ) {
			printLCD(0, " ", NO_SYMBOL);
			printLCD(0, "Too Low Value", NO_SYMBOL);
		}else if(Grams_Required > 100000 ) {
			printLCD(0, " ", NO_SYMBOL);
			printLCD(0, "Too High Value", NO_SYMBOL);
		}
	}
	LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	/*Valve Opne close for one second  currently only on A5 T2 */
	uint32_t ventileclean = HAL_GetTick();
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	while(HAL_GetTick() - ventileclean <= 500);
	moveMotor(VALVEMOTOR, ON, INT_INF); //Servo?
	ventileclean =  HAL_GetTick();
	while(HAL_GetTick() - ventileclean <= 500);
	ventileclean =  HAL_GetTick();
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	HAL_Delay(2000); // waiting here as it needs more more for the pressure to release after freshwater
	float error = 0;

	printLCD(2, " ", NO_SYMBOL);
	printLCD(1, " ", NO_SYMBOL);

	LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING
	Servo_SetPercent(5);
	moveMotorSpindleToLocBlock(M2POS2VAL); //Spindle/CAP Motor to Capping Position
	HAL_Delay(6000);

	/* Added Stabilizing code such that the initial weight taken for measurement is constant*/
	HAL_Delay(500);
	Weight_Initial = Total_Weight;
	//Weight_Initial = Unclamped_Weight;   //Normally Weight_Initial = Total_Weight
	long GramsToDispense = Grams_Required - DEAD_WEIGHT; //DEAD_WEIGHT is the dead weight in the system //Point # 11 for 20 grams bracket
	Target_Weight = Weight_Initial - GramsToDispense;

#if DEBUG == 1
	SerialPrint("Liters Entered: ");
	SerialPrintlnINT(LitersVar);
	SerialPrint("Density Entered: ");
	SerialPrintlnINT(DensityVar);
	SerialPrint("Grams Calculated: ");
	SerialPrintlnINT(Grams_Required);
	SerialPrint("Initial weight: ");
	SerialPrintlnINT(Weight_Initial);
	SerialPrint("Grams to dispense: ");
	SerialPrintlnINT(GramsToDispense);
	SerialPrint("UnClamped Weight: ");
	SerialPrintlnINT(Unclamped_Weight);
	SerialPrint("Target Weight: ");
	SerialPrintlnINT(Target_Weight);
#endif
	if (Target_Weight < 0)
		Target_Weight = (Target_Weight * (-1) + Weight_Initial);

	while(Total_Weight < Target_Weight)
	{
		CustomerAppSub();
		HAL_Delay(1000);
	}

	while (Total_Weight < Target_Weight + 10) {
			HAL_Delay(10);
			SerialPrint("Negative Weight: ");
			SerialPrintlnINT(Total_Weight);
	}
	uint32_t tickstart = HAL_GetTick();
	long Weight_Initial1 = Total_Weight;
	 int is_in_range = FALSE;
		int cnt = 0;
 //Reduced the Stablizing to 3seconds from 8 seconds
		while (!is_in_range)
		{
			while((HAL_GetTick() - tickstart) < 100);

			if((Total_Weight <= Weight_Initial1 + 4) && (Total_Weight >= Weight_Initial1 - 4))
			{
				cnt++;
			}
			else
			{
				cnt = 0;
			}

			if (cnt >= 15)
			{
				is_in_range = TRUE;
			}
			Weight_Initial1 = Total_Weight;
			tickstart = HAL_GetTick();
		}
	HAL_Delay(1000);
	//printLCD(1, "Dispensing....", NO_SYMBOL); // Clean 2nd line
	LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING
	Kp = -1337;
	uint64_t get_timeTick = HAL_GetTick();
	uint16_t WhileWeight = Total_Weight;
	uint8_t Kp_Counter = 0;
	//uint8_t Kp_Counter_2 = 0;
	float minServoPosition = 12.0; // Minimum servo position for slow flow
	float maxServoPosition = 50.0; // Maximum servo position for high flow
	uint8_t stagnation_detected = 0;
	uint64_t stagnation_start_time = 0;
	uint8_t stagnation_factor = 0;
	uint8_t Flag =0;
	uint8_t max_opening =0;
	uint8_t UserCancel = 0;
#if DEBUG == 1
	printLCD(1, "Dispensing....", NO_SYMBOL); // Clean 2nd line
#endif
	while (Total_Weight > Target_Weight) {
		   int keyread = readKeypadNonBlock(CONTROL);
		   if(keyread == ENTERKEY)
		   {
			   SerialPrint("User Cancelled the Dispensing");
			   printLCD(0, "Stop", NO_SYMBOL);
			   UserCancel = 1;
			   break;
		   }
		   else
		   {
				if(HAL_GetTick() - get_timeTick > 50)
				{
				  /* Automatically adjusting PID valves  for dispensing */
					 if(fabs(Total_Weight - Weight_Initial) > 30 && !Flag)
					{
						max_opening = output;
						minServoPosition = max_opening - 17;
						Flag = 1;
					}
					error = Total_Weight - Target_Weight;
					if (Kp < 0)
					{
						Kp = 20/error;
					}
					// P control with adjustment for small errors
					if (fabs(Total_Weight - WhileWeight) <= 10) {
						Kp_Counter++;
						if (Kp_Counter >= 10) {
							if(error >= 1)
							{
								Kp= Kp + (1/error);
							}
							Kp_Counter= 0;
						}
						/* if (!stagnation_detected) {
							stagnation_start_time = HAL_GetTick();
							stagnation_detected = 1;
						} else if (HAL_GetTick() - stagnation_start_time > 8000) { // 8 seconds
							stagnation_factor += 1; // Increment factor for prolonged stagnation
							if (stagnation_factor > 3) {
								//stagnation_factor = MAX_STAGNATION_FACTOR; // Cap the factor
								break;
							}
						}*/
					}
					/*else {
							stagnation_detected = 0;
							stagnation_factor = 0; // Reset if weight changes significantly
							stagnation_start_time = HAL_GetTick();
						}*/
					/*if (error < 200)
									{
					//					Kp_Counter_2++;
					//					if (Kp_Counter_2 >= 20)
					//					{
					//						Kp_Counter_2= 0;
					//						Kp= Kp + (0.2/error);
					//					}
										//output = (Kp * error);
									}
*/
					if (error < 100)
					{
						 SerialPrint("Error is less for Dispensing");
						 //Kp = Kp + 0.001;  // need to test
						//write logic for small dispense
					}
					else
					{
						output = (Kp * error);
					}
					if (output > maxServoPosition) {
							output = maxServoPosition;
						}
					else if (output < minServoPosition) {
						}
				Servo_SetPercent(output);
				get_timeTick = HAL_GetTick();
				WhileWeight = Total_Weight;   // Update WhileWeight
			}

		  }
	}
	//Reaching here means the Total_Weight (current measured weight by load cells) is not more than what it should be after extraction
	Servo_SetPercent(ValveClose);
	HAL_Delay(1000);
	output =0;
	LCD_Clear();
	LoadingFunc(BLINK, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	HAL_Delay(500);
	if(UserCancel == 1)
	{
		printLCD(0, "Stop", NO_SYMBOL);
	}
	else
	{
		printLCD(0, " Complete", NO_SYMBOL);
	}
	//printLCD(0, " Complete", NO_SYMBOL);
	Target_Weight = 0;
	moveMotorSpindleToLocBlock(M2POS1VAL); //Spindle/CAP to close/uncap position
	HAL_Delay(500);
	Servo_SetPercent(ValveOpen);
	HAL_Delay(3000);   //Wait for 3 seconds

	if (Total_Weight > NO_BOTTLE_WEIGHT) {
		//Bottle + Remaining Chemical
	}
	else {
		ReadKeypadVar(ALL); //Point # 2
		LCD_Clear();
		printLCD(0, "Press DEL-Skip,for", 0);
		printLCD(1, "Cleaning", 0);
		printLCD(2, "Press Enter Key", 0);
		printLCD(3, "To Continue", 0);

		while ((lastKeyPressed == 255 || lastKeyPressed == DELKEY)){ //&& (delLongPressed == FALSE)) {
			ReadKeypadVar(ALL);
			HAL_Delay(1);
		}
		if (cleanLongPressed == TRUE) {
			cleanOperation();
		}
		else if(lastKeyPressed == DELKEY)
		{
			CleaningExitCounter = 0;
//			CustomerFrontEnd();
			lastKeyPressed =255;
		}
		else{
			delLongPressed = FALSE;
		}
	}
	capCleaning_Servo();
	LoadingFunc(BLINK, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
/**** Servo Open and close for removing the extra water in the pipe ****/
	Servo_SetPercent(90);
	HAL_Delay(2000);
	Servo_SetPercent(ValveClose);
	HAL_Delay(1000);
	Servo_SetPercent(90);
	HAL_Delay(2000);
	Servo_SetPercent(ValveClose);
	HAL_Delay(1000);
/**** Servo Open and close for removing the extra water in the pipe end ****/
	Servo_SetPercent(90);
	UnlockCannister();
}

static void CustomerAppSub(void)
{	LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	int Total_Weight_Old = Weight_Initial;
	int tempWeight = Weight_Initial;
	int Target_Weight1 = Target_Weight ;
	moveMotorClampToLocBlock(M1POS2VAL);
	Servo_SetPercent(ValveOpen);
	moveMotorSpindleToLocBlock(M2POS2VAL);
	HAL_Delay(7000);
	while (Total_Weight_Old - Total_Weight > (EMPTY_BOTTLE_WEIGHT) || (Total_Weight <= Total_Weight_Old))
	{
		Total_Weight_Old = Total_Weight;
		HAL_Delay(3000);
	}
	moveMotorSpindleToLocBlock(M2POS1VAL);
	HAL_Delay(2000);
	Target_Weight = Target_Weight - tempWeight + DEAD_WEIGHT;
	HAL_Delay(2000);
	capCleaning_Servo();
	//Target_Weight = Target_Weight - tempWeight + DEAD_WEIGHT;
	//Target_Weight = Target_Weight - tempWeight + DEAD_WEIGHT;
	//Target_Weight = Target_Weight - tempWeight + DEAD_WEIGHT;
	HAL_Delay(5000);
	//printLCDInt(1, "Wt!", tempWeight,Target_Weight1);
	//printLCD(0, "Next Can",ENTER_SYMBOL);
	HAL_Delay(1000);
	UnlockCannister();
	//sLoadingFunc(ON, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	HAL_Delay(2000);
	//moveMotorClampToLocBlock(M1POS1VAL);
	while ((digitalRead(Button_Enter_Input_PullUp) == HIGH)) //&& (Total_Weight < NO_BOTTLE_WEIGHT))
	{
		HAL_Delay(100);
		//LoadingFunc(ON, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
		printLCDInt(1, "Wt!", tempWeight,Target_Weight1);
		printLCD(0, "Next Can",ENTER_SYMBOL);
	}
	printLCDInt(1, "Wt!", tempWeight,Target_Weight1);
	printLCD(0, "Next Can",ENTER_SYMBOL);
	moveMotorClampToLocBlock(M1POS2VAL);
	HAL_Delay(5000);
	int tempWeight2 = Total_Weight;
	//Target_Weight = tempWeight2 - (Target_Weight  - EMPTY_BOTTLE_WEIGHT);
	Target_Weight = tempWeight2 - Target_Weight;
	HAL_Delay(3000);
	Servo_SetPercent(ValveClose);
	moveMotorSpindleToLocBlock(M2POS2VAL);
	HAL_Delay(2000);
	LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
}


//Excel 90 - 104
static void cleanOperation(void) {
	LCD_Clear();
	CleaningCancelFlag = FALSE;
	int AverageValveforPressure = 0;
	while(AverageValveforPressure < 50 ){
		pressureSensorReading();
		AverageValveforPressure++;
		HAL_Delay(10);
	}
#if DEBUG == 1
	 SerialPrint("Pressure Valve ");
#endif
	 SerialPrintlnINT(pressurechange);
	if(pressurechange > 100)
	 {
		 moveMotor(VALVEMOTOR, OFF, INT_INF); //Servo?
#if DEBUG == 1
		 SerialPrint("Pressure Valve ");
		 SerialPrintlnINT(pressurechange);
#endif
		 printLCD(3, "Very High Pres.", NO_SYMBOL);
		 printLCD(2, "Fehler F1.", NO_SYMBOL);
		 HAL_Delay(5000);
		 return;
	 }
	else
	{
		LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
		while (Total_Weight < EMPTY_BOTTLE_WEIGHT) { //Point # 12
			ReadKeypadVar(CONTROL);
			printLCD(1, "No Cannister", 0);
			printLCD(2, "Press Enter", 0);
			Servo_SetPercent(ValveOpen);			//Open Servo
			moveMotorSpindleToLocBlock(M2POS1VAL); 	//UnCap
			moveMotorClampToLocBlock(M1POS1VAL);	//UnClamp
			LoadingFunc(ON, NOT_LOADING);
			HAL_Delay(500);
			while (lastKeyPressed != ENTERKEY) {
				ReadKeypadVar(CONTROL);
				HAL_Delay(500);
			}
			return;
		}
		  ManualOperation = TRUE;
		 if(Total_Weight < MIN_LIQUID_CHECK_FOR_CLEANING)
		 {
			SerialPrint("less weight so existing\n ");
			CleaningOperationFlag = TRUE;
			cleaningCycleProcess();
			uint64_t lastTime = HAL_GetTick();
			while(HAL_GetTick() - lastTime < 1000) {}//  non-blocking like serial print.}
			moveMotorSpindleToLocBlock(M2POS1VAL);  //M2 to P2
			servoCleaning();
			UnlockCannister();
			//lastTime =0;
			/**  Check the logic here   **/
		/*	lastTime = HAL_GetTick();
			while(HAL_GetTick() - lastTime < 1000) {}
			moveMotor(VALVEMOTOR, ON, INT_INF); // Servo?
			lastTime = HAL_GetTick();
			while(HAL_GetTick() - lastTime < 500) {}
			moveMotor(VALVEMOTOR, OFF, INT_INF); //Servo?
			lastTime = HAL_GetTick();
			while(HAL_GetTick() - lastTime < 1000) {}
			moveMotorSpindleToLocBlock(M2POS1VAL);  //M2 to P2
			lastTime = HAL_GetTick();
			while(HAL_GetTick() - lastTime < 1000) {}
			moveMotor(VALVEMOTOR, ON, INT_INF); // Servo?
			lastTime = HAL_GetTick();
			while(HAL_GetTick() - lastTime < 500) {}
			moveMotor(VALVEMOTOR, OFF, INT_INF); //Servo?
			UnlockCannister();
			LoadingFunc(ON, NOT_LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
			Servo_SetPercent(ValveClose);
			printLCD(0, "Cannister Unlocked", NO_SYMBOL);
			UpdateTimeToSDCard( hours,  minutes,  seconds);
			isCleaning = FALSE;
			CleaningOperationFlag = FALSE;*/
		 }
		 else
		 {
			 CleaningOperationFlag = FALSE;
			 //return;
		 }
	}
}


static void UnlockCannister(void)
{
	LCD_Clear();
	vTaskDelay(pdMS_TO_TICKS(500));
//	HAL_Delay(500);
	LoadingFunc(ON, NOT_LOADING);
	printLCD(0, "Unlock ", ENTER_SYMBOL);
	vTaskDelay(pdMS_TO_TICKS(1000));
//	HAL_Delay(1000);
	while(TRUE){
		ReadKeypadVar(CONTROL);
		if(lastKeyPressed == ENTERKEY){
			moveMotorSpindleToLocBlock(M2POS1VAL);
			moveMotorClampToLocBlock(M1POS1VAL);
			LCD_Clear();
			vTaskDelay(pdMS_TO_TICKS(200));
//			HAL_Delay(200);
			CleaningExitCounter = 0;
			LoadingFunc(BLINK, LOADING);
			return;
		}
		/*if(lastKeyPressed == DELKEY){
			//Cancel UnClamping of can
			CleaningExitCounter = 0;
			return;
		}*/
	}
	UpdateTimeToSDCard( hours,  minutes,  seconds);
}

static void cleaningCycleProcess(void)
{
	 CleaningCancelFlag = FALSE;
	 moveMotorClampToLocBlock(M1POS2VAL);      //(M1POS4VAL, NONE);  //M1 to P4
	 Servo_SetPercent(ValveOpen);
	 moveMotor(VALVEMOTOR, ON, INT_INF);
	 LoadingFunc(BLINK, LOADING);
     LCD_Clear();
	 HAL_Delay(500);
	 uint32_t tickstart = HAL_GetTick();
	printLCDInt(3,"Cycle No: ",1, UserCleaningCycleNumber);
	tickstart = HAL_GetTick();
	completedCycles = 0;
	 while (completedCycles < UserCleaningCycleNumber)
	 {
		for(int j = 0; j < 4; j++)
		{
			if(operationCancelled() == 0){ break;}
			while((HAL_GetTick() - tickstart) <= 1000 );
			tickstart = HAL_GetTick();
			printLCDInt(3,"Cycle No: ",completedCycles + 1, UserCleaningCycleNumber);
			moveMotorSpindleToLocBlock(M2POS7VAL);
			while((HAL_GetTick() - tickstart) <= 1000 );
			tickstart = HAL_GetTick();
			if(CleaningCancelFlag || adc_pressure > 100 ){
				printLCD(3, "Stop", NO_SYMBOL);
				break;
			}
			moveMotorSpindleToLocBlock(M2POS8VAL);
			while((HAL_GetTick() - tickstart) <= 1000 );
			tickstart = HAL_GetTick();
			if(CleaningCancelFlag || adc_pressure > 100 ){
				printLCD(3, "Stop", NO_SYMBOL);
				break;
			}
			moveMotorSpindleToLocBlock(M2POS9VAL);
			while((HAL_GetTick() - tickstart) <= 1000 );
			tickstart = HAL_GetTick();
			if(CleaningCancelFlag || adc_pressure > 100 ){
				printLCD(3, "Stop", NO_SYMBOL);
				break;
			}
			moveMotorSpindleToLocBlock(M2POS8VAL);
			while((HAL_GetTick() - tickstart) <= 1000 );
			tickstart = HAL_GetTick();
			if(CleaningCancelFlag || adc_pressure > 100 ){
				printLCD(3, "Stop", NO_SYMBOL);
				break;
			}

			printLCDInt(3,"Cycle No: ",completedCycles + 1, UserCleaningCycleNumber);
			moveMotor(VALVEMOTOR, ON, INT_INF); //Valve ON
	}

// Check for updates to UserCleaningCycleNumber
		if (completedCycles >= UserCleaningCycleNumber) {
			break; // Stop if cycles are reduced below the current index
		}

		completedCycles++; // Increment completed cycles
		if(CleaningCancelFlag|| adc_pressure > 100){break;}
		moveMotorSpindleToLocBlock(M2POS9VAL);
		while((HAL_GetTick() - tickstart) <= 1000 );
		tickstart = HAL_GetTick();
		if(CleaningCancelFlag|| adc_pressure > 100){break;}
		moveMotorSpindleToLocBlock(M2POS7VAL);
		while((HAL_GetTick() - tickstart) <= 1000 );
		tickstart = HAL_GetTick();
		if(CleaningCancelFlag|| adc_pressure > 100){break;}

	}
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	while((HAL_GetTick() - tickstart) <= 1000 );
	tickstart = HAL_GetTick();
	CleaningCancelFlag = TRUE;
	cleanLongPressed = FALSE;
	delLongPressed = FALSE;
	UserCleaningCycleNumber = 1;
	CleaningOperationFlag = FALSE;
}

static void servoCleaning(void)
{
	uint32_t get_timeTick = HAL_GetTick();
	moveMotor(VALVEMOTOR, ON, INT_INF); //Valve ON
	Servo_SetPercent(90);
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 2000);
	Servo_SetPercent(40);
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 1000);

	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	Servo_SetPercent(90);
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 2000);

	Servo_SetPercent(40);
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 1000);

	get_timeTick =  HAL_GetTick();
	Servo_SetPercent(90);
	while(HAL_GetTick() - get_timeTick <= 2000);

	Servo_SetPercent(40);
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 1000);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	Servo_SetPercent(90);
	ManualOperation = FALSE;

}


static void capCleaning_Servo(void)
{
	LoadingFunc(BLINK, LOADING); //Power LED (off, on, blink), loading screen (LOADING, NOT_LOADING)
	moveMotor(VALVEMOTOR, ON, INT_INF); //Valve OFF
	HAL_Delay(100);
	moveMotorSpindleToLocBlock(M2POS1VAL);
	uint64_t get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 1000);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 3000);
	moveMotor(VALVEMOTOR, ON, INT_INF); //Servo?
	Servo_SetPercent(90);
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 200);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 500);
	moveMotor(VALVEMOTOR, ON, INT_INF); //Servo?
	get_timeTick =  HAL_GetTick();
	Servo_SetPercent(40);
	while(HAL_GetTick() - get_timeTick <= 1000);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 500);
	moveMotor(VALVEMOTOR, ON, INT_INF); //Servo?
	get_timeTick =  HAL_GetTick();
	Servo_SetPercent(90);
	while(HAL_GetTick() - get_timeTick <= 1000);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 500);
	moveMotor(VALVEMOTOR, ON, INT_INF); //Servo?
	get_timeTick =  HAL_GetTick();
	Servo_SetPercent(40);
	while(HAL_GetTick() - get_timeTick <= 1000);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 500);
	moveMotor(VALVEMOTOR, ON, INT_INF); //Servo?
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 100);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Valve ON
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 3000);
	moveMotor(VALVEMOTOR, ON, INT_INF); //Servo?
	moveMotorSpindleToLocBlock(M2POS1VAL);
	get_timeTick =  HAL_GetTick();
	while(HAL_GetTick() - get_timeTick <= 1000);
	moveMotor(VALVEMOTOR, OFF, INT_INF); //Servo?
}
