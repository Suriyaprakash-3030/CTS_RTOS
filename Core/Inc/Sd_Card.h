/**
 *******************************************************************************
 * @file    Sd_card.h
 * @brief   This file provides the Macros that will be used globally throughout the program.
 ******************************************************************************
 *  Created on: Nov 22, 2024
 *      Author: mhmaq
 */
void sd_Card_Check(void);

void UpdateTimeToSDCard(uint32_t hours, uint32_t minutes, uint32_t seconds);

void LoadTimeFromSDCard(uint32_t *hours, uint32_t *minutes, uint32_t *seconds);
