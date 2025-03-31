/*
 * HardwareLevel.h
 *
 *  Created on: Sep 6, 2024
 *      Author: mhmaq
 */

#ifndef HARDWARELEVEL_H
#define HARDWARELEVEL_H

#include <main.h>


void moveMotorSpindleToLocBlock(long posM2);
void moveMotorClampToLocBlock(long posM1);
uint8_t moveMotorSpindleToLocNoneBlock(long posM2);
uint8_t moveMotorClampToLocNoneBlock(long posM1);
void moveMotor(int motorNum, int Dir, int targetPoint);
void checkFaultsMotors(uint8_t x);
void GetADC(void);

#endif /* HARDWARELEVEL_H_*/
