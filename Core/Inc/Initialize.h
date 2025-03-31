/*
 * Initialize.h
 *
 *  Created on: Sep 6, 2024
 *      Author: mhmaq
 */

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <main.h>  // main will include all programs
#include "stm32f4xx_hal.h"
// Function prototypes for initialization
void BasicSetup(void);
void InitializeMachine(void);
void InitializeMachine2(void);
void SubInit(void);
void goToSleep(void);
void CTS_Home_position(void);


#endif // INITIALIZE_H
