/**
 *******************************************************************************
 * @file    Global.h
 * @brief   This file provides the Macros that will be used globally throughout the program.
 ******************************************************************************
 *  Created on: Aug 30, 2024
 *      Author: mhmaq
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <main.h>
/*######################################## PROCESS MACROS BEGIN #####################################*/
#define BLACK_TEST_MACHINE

#define ON 1
/*! @def ON
    @brief A macro that returns the 1 for Turning ON the Motor & Setting Direction */

#define OFF 0
/*! @def OFF
    @brief A macro that returns the 0 for Turning OFF the Motor & Setting Direction */

#define HIGH 1
/*! @def HIGH
    @brief A macro that returns the 1 for Turning ON the GPIO */

#define LOW 0
/*! @def LOW
    @brief A macro that returns the 0 for Turning OFF the GPIO */

#define TRUE 1
/*! @def TRUE
    @brief A macro that returns the 1 for state check */

#define FALSE 0
/*! @def FALSE
    @brief A macro that returns the 0 for state check */

#define MAXCLEANCYCLESALLOWED  3 //Maximum allowed numbers for clean cycles
/*! @def MAXCLEANCYCLESALLOWED
    @brief A macro that permits the maximum number of cleaning cycle after a single drain */

#define INT_INF 9999999 //Large number to be considered as infitiy for an integer
/*! @def INT_INF
    @brief A macro that declares the infinite cycles of a process */

#define CUSTOMERAPP 1
#define DEMOAPP 0
#define TESTAPP 2

// DEAD VOULME of the CTS
#define DEAD_WEIGHT  110  //175   //Dead weight in machine, in grams 80 //250

// Assumed EMPTY _BOTTLE WIGHT in Actual is Different
#define EMPTY_BOTTLE_WEIGHT 75

// Assumed NO _BOTTLE WIGHT in Actual is Different
#define NO_BOTTLE_WEIGHT 100   // new flowchart  weight check line - 29

// Assumed WIGHT is less than 1litre
#define MIN_LIQUID_CHECK_FOR_CLEANING   1000

#if defined(BLACK_TEST_MACHINE)
  #define LOAD_CELL_CAL 1
#elif defined(WHITE_TEST_MACHINE)
  #define LOAD_CELL_CAL 1
#endif

//Debug and serial defines
#define DEBUG 1

//MOTOR defines
#define DCMOTOR 1
#define SERVOMOTOR 2
#define ISSERVOINTERTED	1

#if defined(BLACK_TEST_MACHINE)
  #define M1POS1VAL 0   //Unclamped pos
  #define M1POS2VAL  740 //Clamped pos - original: 740

#elif defined(WHITE_TEST_MACHINE)
  #define M1POS1VAL 0   //Unclamped pos
  #define M1POS2VAL 740 //Clamped pos
  #define ValveClose 100 //Valve closed - servo 0%
  #define ValveOpen 0 //Valve open - servo 100%
#endif

#define CLAMPMOTOR 1
#define MOVEM1SLOWPULSES 500 //How many pulses should we be close to target to start moving motor in slow
#define M1ACCEPTEDERROR 100 //How many motor pulses to accept as an error rate
//#define M1_MAX_CURRENT 800.0 //Max allowed motor current in mA
#define M1_I_RMS sqrt(MOTOR1_SPEED/255.0) * M1_MAX_CURRENT //SQRT(duty) * required Max Current value
#define M1_CURRENT_LIMIT 255 //255//M1_I_RMS * 455 * 820 * 256 / 5 / 1000000

#if defined(BLACK_TEST_MACHINE)
  #define M2POS1VAL 0
  #define M2POS2VAL   3250   //70mm  changed for field test //2040 //1300 // 1300 //2050 (52mm) is the minimum opening for maximum cross-section and 600 is the min opening for flow to start is 825 (20mm)
  #define M2POS2VAL_1Liter 1600// This for the Protech 1 liter CAN due to diameter change in the CAN nozzle (55mm)
  #define M2POS2_1VAL 4000 //Rinsing position 3200
  #define M2POS3VAL 4500 //4500
  #define M2POS4VAL 7100 //5500

  #define M2POS5VAL 6850
  #define M2POSCAPVAL 40 // 6mm to 5 mm due to pressure
//cleaning Cycle Pulses
  #define M2POS7VAL 5892 //6450
  #define M2POS8VAL 6500 //6600
  #define M2POS9VAL 7135 //6700

#elif defined(WHITE_TEST_MACHINE)
  #define M2POS1VAL 0
  #define M2POS2VAL 7750
  #define M2POS3VAL 17500
  #define M2POS4VAL 21500
#endif

#define MOTOR2_SPEED 98 //255 //PWM value of M1, out of 255
#define MOTOR2_SPEED_SLOW 90 //128 //PWM value of M1, out of 255, when in slow
#define CAPMOTOR 2
#define MOVEM2SLOWPULSES 1000 //How many pulses should we be close to target to start moving motor in slow
#define M2ACCEPTEDERROR 100 //How many motor pulses to accept as an error rate
#define M2_MAX_CURRENT 800.0 //Max allowed motor current in mA
#define M2_I_RMS sqrt(MOTOR2_SPEED/255.0) * M2_MAX_CURRENT //SQRT(duty) * required Max Current value
#define M2_CURRENT_LIMIT 255//255//M2_I_RMS * 455 * 820 * 256 / 5 / 1000000

#define CW 1 //1
#define CCW 2 //2

#define VALVEMOTOR 3

#define ValveClose 0 				//Valve closed - servo 0%
#define ValveOpen 100 				//Valve open - servo 100%

/*######################################## PROCESS MACROS END #####################################*/


/*######################################## Variables BEGIN #####################################*/

extern char stringBuffer[21];
// Machine state and operation variables
extern uint8_t isMachineOn;     // = FALSE; // Machine state, 0 for off, 1 for on

// Variables used for Weight
extern long Total_Weight; 		// Current total weight
extern int Weight_Initial; 		// Initial weight
extern int Tareweight;
extern long Target_Weight;
extern long Unclamped_Weight;   //This to store the Unclamped weight

extern float LitersVar;    		// Liters entered by user
extern float DensityVar; 		// Density entered by user
extern long Grams_Required; 	// Grams required to be dispensed
extern long GramsToDispense;    //User Input to dispense

extern uint8_t isValveOpen;		// = FALSE; // Valve state


extern uint8_t ProgramToRun; 	//Holds which program to run
extern uint8_t InitComp;		//Initialization Check

extern char ValveStatus[10];

/* Cleaning FLags used */
extern uint8_t CleaningFlag;
extern uint8_t CleaningCancelFlag;  // Cleaning Cancellation Flag
volatile extern uint8_t CleaningOperationFlag;
extern uint8_t CleaningExit;
extern uint8_t CleaningExitCounter;

extern volatile  uint8_t UserCleaningCycleNumber;
extern uint8_t FreshValveStatus;
extern uint8_t isCleaning;

/***** ANalog Raw Valves***/
extern uint16_t adc_pressure;
extern uint16_t adc_voltage;
extern int16_t adc_CurrentSpindle;
extern int16_t adc_CurrentServo;
extern int16_t adc_CurrentClamp;

extern float pressurechange;

extern volatile float Kp ;  // Proportional gain

extern volatile uint8_t ManualOperation;
extern volatile uint8_t completedCycles;

extern volatile uint8_t log_to_sd_flag;
extern volatile uint32_t seconds, minutes, hours;
extern float output;
extern long tareSend;

/*######################################## Variables END #######################################*/
void DWT_Init(void);


#endif /* GLOBAL_H */
