/**
 *******************************************************************************
 * @file    Global.c
 * @brief   This file provides the variables that will be used globally throughout the program.
 ******************************************************************************
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

// Global.c
#include <main.h>
#include "Global.h"

/*######################################## Variables BEGIN #####################################*/
// Machine state and operation variables
uint8_t isMachineOn = FALSE; // Machine state, 0 for off, 1 for on


long Total_Weight = 0; 					// Current total weight
int Weight_Initial = 0; 				// Initial weight
int Tareweight = 0;
long Target_Weight = 0;

long Grams_Required = 0; 				// Grams required to be dispensed
long GramsToDispense = 0;
float LitersVar = 0.0; 					// Liters entered by user
float DensityVar = 0.0; 				// Density entered by user
uint8_t isValveOpen = FALSE; 			// Valve state


uint8_t ProgramToRun=CUSTOMERAPP; 		 //Holds which program to run
long Unclamped_Weight = 0;
char ValveStatus[10]="";

uint8_t CleaningFlag=FALSE;
uint8_t CleaningCancelFlag = TRUE;  	// Cleaning Cancellation Flag
volatile uint8_t CleaningOperationFlag = FALSE;
uint8_t CleaningExit=FALSE;
uint8_t CleaningExitCounter=0;

volatile uint8_t UserCleaningCycleNumber = 1;
uint8_t FreshValveStatus = 0;
float output = 0;

long tareSend = 0;
uint8_t isCleaning = FALSE;

volatile float Kp = 0.001;  // Proportional gain  0.02

volatile uint8_t ManualOperation = FALSE;
volatile uint8_t completedCycles = 0;

volatile uint8_t log_to_sd_flag = 0;
volatile uint32_t seconds = 0, minutes = 0, hours = 0;

void DWT_Init(void) {
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
}

uint8_t InitComp = FALSE;


/*****Analog sensor Raw valves ****/
float pressurechange = 0;
uint16_t adc_pressure=0;
uint16_t adc_voltage=0;
int16_t adc_CurrentSpindle=0;
int16_t adc_CurrentServo=0;
int16_t adc_CurrentClamp=0;

/*######################################## Variables END #######################################*/

