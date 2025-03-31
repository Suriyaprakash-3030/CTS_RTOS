/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define Servo
#include <stdio.h>
#include <string.h>

#include <AnalogRead.h>
#include <CustomerApp.h>
#include <DemoApp.h>
#include <DigitalRead.h>
#include <DigitalWrite.h>
//#include <FX29K.h>
#include <Global.h>
#include <HardwareLevel.h>
#include <Initialize.h>
#include <ISR.h>
#include <Keypad.h>
#include <LCD.h>
#include <LoadCells.h>
//#include <Millis.h>
#include <pwm.h>
#include <stdlib.h>
#include <SerialHandler.h>
#include <LPC_UART.h>
#include <ESP_UART.h>
#include <Servo.h>
#include <Test.h>
#include <fatfs_sd.h>
#include <Sd_Card.h>
#include <usart.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Sensor_BottlePosition_Input_Pin GPIO_PIN_2
#define Sensor_BottlePosition_Input_GPIO_Port GPIOE
#define Sensor_RES_Input_Pin GPIO_PIN_4
#define Sensor_RES_Input_GPIO_Port GPIOE
#define Clamp_M2_EncA_Interrupt_Pin GPIO_PIN_5
#define Clamp_M2_EncA_Interrupt_GPIO_Port GPIOE
#define Clamp_M2_EncA_Interrupt_EXTI_IRQn EXTI9_5_IRQn
#define Spindle_M3_EncA_Interrupt_Pin GPIO_PIN_6
#define Spindle_M3_EncA_Interrupt_GPIO_Port GPIOE
#define Spindle_M3_EncA_Interrupt_EXTI_IRQn EXTI9_5_IRQn
#define Spindle_M3_EncB_Interrupt_Pin GPIO_PIN_13
#define Spindle_M3_EncB_Interrupt_GPIO_Port GPIOC
#define Spindle_M3_EncB_Interrupt_EXTI_IRQn EXTI15_10_IRQn
#define EN_DCDC_5V_Output_Pin GPIO_PIN_14
#define EN_DCDC_5V_Output_GPIO_Port GPIOC
#define EN_DCDC_8V_Output_Pin GPIO_PIN_15
#define EN_DCDC_8V_Output_GPIO_Port GPIOC
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define Clamp_M2_Prox_Interrupt_Pin GPIO_PIN_0
#define Clamp_M2_Prox_Interrupt_GPIO_Port GPIOC
#define Clamp_M2_Prox_Interrupt_EXTI_IRQn EXTI0_IRQn
#define Clamp_M2_EncB__Interrupt_Pin GPIO_PIN_1
#define Clamp_M2_EncB__Interrupt_GPIO_Port GPIOC
#define Clamp_M2_EncB__Interrupt_EXTI_IRQn EXTI1_IRQn
#define Curr_Sensor_ServoDC_ADC_Input_Pin GPIO_PIN_2
#define Curr_Sensor_ServoDC_ADC_Input_GPIO_Port GPIOC
#define Curr_Sensor_Clamp_ADC_Input_Pin GPIO_PIN_3
#define Curr_Sensor_Clamp_ADC_Input_GPIO_Port GPIOC
#define Curr_Sensor_Spindle_ADC_Input_Pin GPIO_PIN_0
#define Curr_Sensor_Spindle_ADC_Input_GPIO_Port GPIOA
#define V12_ADC_Input_Pin GPIO_PIN_1
#define V12_ADC_Input_GPIO_Port GPIOA
#define Button_Clean_Interrupt_PullUp_Pin GPIO_PIN_2
#define Button_Clean_Interrupt_PullUp_GPIO_Port GPIOA
#define Button_Clean_Interrupt_PullUp_EXTI_IRQn EXTI2_IRQn
#define Power_LED_Output_Pin GPIO_PIN_3
#define Power_LED_Output_GPIO_Port GPIOA
#define Button_Power_Interrupt_PullUp_Pin GPIO_PIN_4
#define Button_Power_Interrupt_PullUp_GPIO_Port GPIOA
#define Button_Power_Interrupt_PullUp_EXTI_IRQn EXTI4_IRQn
#define Button_ManAuto_Input_PullUp_Pin GPIO_PIN_5
#define Button_ManAuto_Input_PullUp_GPIO_Port GPIOA
#define Button_PlayStop_Input_PullUp_Pin GPIO_PIN_6
#define Button_PlayStop_Input_PullUp_GPIO_Port GPIOA
#define Button_DelBack_Input_PullUp_Pin GPIO_PIN_7
#define Button_DelBack_Input_PullUp_GPIO_Port GPIOA
#define Sensor_Pressure_ADC_Input_Pin GPIO_PIN_4
#define Sensor_Pressure_ADC_Input_GPIO_Port GPIOC
#define Button_1_Input_PullUp_Pin GPIO_PIN_5
#define Button_1_Input_PullUp_GPIO_Port GPIOC
#define Button_2_Input_PullUp_Pin GPIO_PIN_0
#define Button_2_Input_PullUp_GPIO_Port GPIOB
#define Button_3_Input_PullUp_Pin GPIO_PIN_1
#define Button_3_Input_PullUp_GPIO_Port GPIOB
#define Button_4_Input_PullUp_Pin GPIO_PIN_2
#define Button_4_Input_PullUp_GPIO_Port GPIOB
#define Button_5_Input_PullUp_Pin GPIO_PIN_7
#define Button_5_Input_PullUp_GPIO_Port GPIOE
#define Button_6_Input_PullUp_Pin GPIO_PIN_8
#define Button_6_Input_PullUp_GPIO_Port GPIOE
#define Button_7_Input_PullUp_Pin GPIO_PIN_9
#define Button_7_Input_PullUp_GPIO_Port GPIOE
#define Button_8_Input_PullUp_Pin GPIO_PIN_10
#define Button_8_Input_PullUp_GPIO_Port GPIOE
#define Button_9_Input_PullUp_Pin GPIO_PIN_11
#define Button_9_Input_PullUp_GPIO_Port GPIOE
#define Button_0_Input_PullUp_Pin GPIO_PIN_12
#define Button_0_Input_PullUp_GPIO_Port GPIOE
#define Button_Enter_Input_PullUp_Pin GPIO_PIN_13
#define Button_Enter_Input_PullUp_GPIO_Port GPIOE
#define Button_Comma_Input_PullUp_Pin GPIO_PIN_14
#define Button_Comma_Input_PullUp_GPIO_Port GPIOE
#define UART3TX_FC1RX_Pin GPIO_PIN_10
#define UART3TX_FC1RX_GPIO_Port GPIOB
#define UART3RX_FC1TX_Pin GPIO_PIN_11
#define UART3RX_FC1TX_GPIO_Port GPIOB
#define USBFlag_Interrupt_PullUp_Pin GPIO_PIN_12
#define USBFlag_Interrupt_PullUp_GPIO_Port GPIOB
#define USBFlag_Interrupt_PullUp_EXTI_IRQn EXTI15_10_IRQn
#define ServoDC_M1_Dir_Output_Pin GPIO_PIN_15
#define ServoDC_M1_Dir_Output_GPIO_Port GPIOB
#define nRESET_LPC_Output_Pin GPIO_PIN_8
#define nRESET_LPC_Output_GPIO_Port GPIOD
#define Relay_Output_Pin GPIO_PIN_9
#define Relay_Output_GPIO_Port GPIOD
#define Spindle_M3_Dir_Output_Pin GPIO_PIN_10
#define Spindle_M3_Dir_Output_GPIO_Port GPIOD
#define Clamp_M2_PWM_Pin GPIO_PIN_12
#define Clamp_M2_PWM_GPIO_Port GPIOD
#define Spindle_M3_PWM_Pin GPIO_PIN_13
#define Spindle_M3_PWM_GPIO_Port GPIOD
#define Clamp_M2_Dir_Output_Pin GPIO_PIN_14
#define Clamp_M2_Dir_Output_GPIO_Port GPIOD
#define ServoDC_M1_EncB_Interrupt_Pin GPIO_PIN_15
#define ServoDC_M1_EncB_Interrupt_GPIO_Port GPIOD
#define ServoDC_M1_EncB_Interrupt_EXTI_IRQn EXTI15_10_IRQn
#define LCD_Contrast_PWM_Pin GPIO_PIN_6
#define LCD_Contrast_PWM_GPIO_Port GPIOC
#define LCD_LED_PWM_Pin GPIO_PIN_7
#define LCD_LED_PWM_GPIO_Port GPIOC
#define ServoDC_M1_PWM_Pin GPIO_PIN_8
#define ServoDC_M1_PWM_GPIO_Port GPIOC
#define ServoDC_M1_Prox_Interrupt_Pin GPIO_PIN_8
#define ServoDC_M1_Prox_Interrupt_GPIO_Port GPIOA
#define ServoDC_M1_Prox_Interrupt_EXTI_IRQn EXTI9_5_IRQn
#define Spindle_M3_Prox_Interrupt_Pin GPIO_PIN_10
#define Spindle_M3_Prox_Interrupt_GPIO_Port GPIOA
#define Spindle_M3_Prox_Interrupt_EXTI_IRQn EXTI15_10_IRQn
#define SPI_nSS_Pin GPIO_PIN_15
#define SPI_nSS_GPIO_Port GPIOA
#define STMUART4_TX_ESPUART1RX_Pin GPIO_PIN_10
#define STMUART4_TX_ESPUART1RX_GPIO_Port GPIOC
#define STMUART4_RX_ESPUART1TX_Pin GPIO_PIN_11
#define STMUART4_RX_ESPUART1TX_GPIO_Port GPIOC
#define LCD_RS_Output_Pin GPIO_PIN_1
#define LCD_RS_Output_GPIO_Port GPIOD
#define LCD_EN_Output_Pin GPIO_PIN_2
#define LCD_EN_Output_GPIO_Port GPIOD
#define LCD_D4_Output_Pin GPIO_PIN_3
#define LCD_D4_Output_GPIO_Port GPIOD
#define LCD_D5_Output_Pin GPIO_PIN_4
#define LCD_D5_Output_GPIO_Port GPIOD
#define LCD_D6_Output_Pin GPIO_PIN_6
#define LCD_D6_Output_GPIO_Port GPIOD
#define LCD_D7_Output_Pin GPIO_PIN_7
#define LCD_D7_Output_GPIO_Port GPIOD
#define SPI_SCK_Pin GPIO_PIN_3
#define SPI_SCK_GPIO_Port GPIOB
#define SPI_MISO_Pin GPIO_PIN_4
#define SPI_MISO_GPIO_Port GPIOB
#define SPI_MOSI_Pin GPIO_PIN_5
#define SPI_MOSI_GPIO_Port GPIOB
#define UART1_TX_STLINK_RX_Pin GPIO_PIN_6
#define UART1_TX_STLINK_RX_GPIO_Port GPIOB
#define UART1_RX_STLINK_TX_Pin GPIO_PIN_7
#define UART1_RX_STLINK_TX_GPIO_Port GPIOB
#define USB_EN_Output_Pin GPIO_PIN_8
#define USB_EN_Output_GPIO_Port GPIOB
#define Servo_PWM_OR_DC_M1_EncA_Interrupt_Pin GPIO_PIN_9
#define Servo_PWM_OR_DC_M1_EncA_Interrupt_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

//* PreProcessor Directives (MACROS)

//* Oscillator Pins
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH

//* System Outputs
#define Power_LED_Output_Pin GPIO_PIN_3
#define Power_LED_Output_GPIO_Port GPIOA
#define EN_DCDC_5V_Output_Pin GPIO_PIN_14
#define EN_DCDC_5V_Output_GPIO_Port GPIOC
#define EN_DCDC_8V_Output_Pin GPIO_PIN_15
#define EN_DCDC_8V_Output_GPIO_Port GPIOC
#define Relay_Output_Pin GPIO_PIN_9
#define Relay_Output_GPIO_Port GPIOD

//* Motors PWM Pins
#define Clamp_M2_PWM_Pin GPIO_PIN_12
#define Clamp_M2_PWM_GPIO_Port GPIOD
#define Spindle_M3_PWM_Pin GPIO_PIN_13
#define Spindle_M3_PWM_GPIO_Port GPIOD
#define ServoDC_M1_PWM_Pin GPIO_PIN_8
#define ServoDC_M1_PWM_GPIO_Port GPIOC

//* Motor Directions Output
#define Spindle_M3_Dir_Output_Pin GPIO_PIN_10
#define Spindle_M3_Dir_Output_GPIO_Port GPIOD
#define Clamp_M2_Dir_Output_Pin GPIO_PIN_14
#define Clamp_M2_Dir_Output_GPIO_Port GPIOD
#define ServoDC_M1_Dir_Output_Pin GPIO_PIN_15
#define ServoDC_M1_Dir_Output_GPIO_Port GPIOB

//* Motor Interrupts
//Clamp Motor
#define Clamp_M2_Prox_Interrupt_Pin GPIO_PIN_0
#define Clamp_M2_Prox_Interrupt_GPIO_Port GPIOC
#define Clamp_M2_Prox_Interrupt_EXTI_IRQn EXTI0_IRQn
#define Clamp_M2_EncB__Interrupt_Pin GPIO_PIN_1
#define Clamp_M2_EncB__Interrupt_GPIO_Port GPIOC
#define Clamp_M2_EncB__Interrupt_EXTI_IRQn EXTI1_IRQn
#define Clamp_M2_EncA_Interrupt_Pin GPIO_PIN_5
#define Clamp_M2_EncA_Interrupt_GPIO_Port GPIOE
#define Clamp_M2_EncA_Interrupt_EXTI_IRQn EXTI9_5_IRQn
//Spindle Motor
#define Spindle_M3_Prox_Interrupt_Pin GPIO_PIN_10
#define Spindle_M3_Prox_Interrupt_GPIO_Port GPIOA
#define Spindle_M3_Prox_Interrupt_EXTI_IRQn EXTI15_10_IRQn
#define Spindle_M3_EncA_Interrupt_Pin GPIO_PIN_6
#define Spindle_M3_EncA_Interrupt_GPIO_Port GPIOE
#define Spindle_M3_EncA_Interrupt_EXTI_IRQn EXTI9_5_IRQn
#define Spindle_M3_EncB_Interrupt_Pin GPIO_PIN_13
#define Spindle_M3_EncB_Interrupt_GPIO_Port GPIOC
#define Spindle_M3_EncB_Interrupt_EXTI_IRQn EXTI15_10_IRQn
//Servo/DC Motor
#define ServoDC_M1_Prox_Interrupt_Pin GPIO_PIN_8
#define ServoDC_M1_Prox_Interrupt_GPIO_Port GPIOA
#define ServoDC_M1_Prox_Interrupt_EXTI_IRQn EXTI9_5_IRQn
#define ServoDC_M1_EncB_Interrupt_Pin GPIO_PIN_15
#define ServoDC_M1_EncB_Interrupt_GPIO_Port GPIOD
#define ServoDC_M1_EncB_Interrupt_EXTI_IRQn EXTI15_10_IRQn
#define Servo_PWM_OR_DC_M1_EncA_Interrupt_Pin GPIO_PIN_9
#define Servo_PWM_OR_DC_M1_EncA_Interrupt_GPIO_Port GPIOB
#ifndef Servo
#define Servo_PWM_OR_DC_M1_EncA_Interrupt_EXTI_IRQn EXTI9_5_IRQn
#endif


//* Sensors & ADCs
#define Curr_Sensor_ServoDC_ADC_Input_Pin GPIO_PIN_2
#define Curr_Sensor_ServoDC_ADC_Input_GPIO_Port GPIOC
#define Curr_Sensor_Clamp_ADC_Input_Pin GPIO_PIN_3
#define Curr_Sensor_Clamp_ADC_Input_GPIO_Port GPIOC
#define Curr_Sensor_Spindle_ADC_Input_Pin GPIO_PIN_0
#define Sensor_BottlePosition_Input_Pin GPIO_PIN_2
#define Sensor_BottlePosition_Input_GPIO_Port GPIOE
#define Sensor_RES_Input_Pin GPIO_PIN_4
#define Sensor_RES_Input_GPIO_Port GPIOE
#define Curr_Sensor_Spindle_ADC_Input_GPIO_Port GPIOA
#define V12_ADC_Input_Pin GPIO_PIN_1
#define V12_ADC_Input_GPIO_Port GPIOA
#define Sensor_Pressure_ADC_Input_Pin GPIO_PIN_4
#define Sensor_Pressure_ADC_Input_GPIO_Port GPIOC

//* Buttons Input & Interrupts
#define Button_Clean_Interrupt_PullUp_Pin GPIO_PIN_2
#define Button_Clean_Interrupt_PullUp_GPIO_Port GPIOA
#define Button_Clean_Interrupt_PullUp_EXTI_IRQn EXTI2_IRQn
#define Button_Power_Interrupt_PullUp_Pin GPIO_PIN_4
#define Button_Power_Interrupt_PullUp_GPIO_Port GPIOA
#define Button_Power_Interrupt_PullUp_EXTI_IRQn EXTI4_IRQn
#define Button_ManAuto_Input_PullUp_Pin GPIO_PIN_5
#define Button_ManAuto_Input_PullUp_GPIO_Port GPIOA
#define Button_PlayStop_Input_PullUp_Pin GPIO_PIN_6
#define Button_PlayStop_Input_PullUp_GPIO_Port GPIOA
#define Button_DelBack_Input_PullUp_Pin GPIO_PIN_7
#define Button_DelBack_Input_PullUp_GPIO_Port GPIOA
#define Button_1_Input_PullUp_Pin GPIO_PIN_5
#define Button_1_Input_PullUp_GPIO_Port GPIOC
#define Button_2_Input_PullUp_Pin GPIO_PIN_0
#define Button_2_Input_PullUp_GPIO_Port GPIOB
#define Button_3_Input_PullUp_Pin GPIO_PIN_1
#define Button_3_Input_PullUp_GPIO_Port GPIOB
#define Button_4_Input_PullUp_Pin GPIO_PIN_2
#define Button_4_Input_PullUp_GPIO_Port GPIOB
#define Button_5_Input_PullUp_Pin GPIO_PIN_7
#define Button_5_Input_PullUp_GPIO_Port GPIOE
#define Button_6_Input_PullUp_Pin GPIO_PIN_8
#define Button_6_Input_PullUp_GPIO_Port GPIOE
#define Button_7_Input_PullUp_Pin GPIO_PIN_9
#define Button_7_Input_PullUp_GPIO_Port GPIOE
#define Button_8_Input_PullUp_Pin GPIO_PIN_10
#define Button_8_Input_PullUp_GPIO_Port GPIOE
#define Button_9_Input_PullUp_Pin GPIO_PIN_11
#define Button_9_Input_PullUp_GPIO_Port GPIOE
#define Button_0_Input_PullUp_Pin GPIO_PIN_12
#define Button_0_Input_PullUp_GPIO_Port GPIOE
#define Button_Enter_Input_PullUp_Pin GPIO_PIN_13
#define Button_Enter_Input_PullUp_GPIO_Port GPIOE
#define Button_Comma_Input_PullUp_Pin GPIO_PIN_14
#define Button_Comma_Input_PullUp_GPIO_Port GPIOE

//* LCD Pins Output
#define LCD_Contrast_PWM_Pin GPIO_PIN_6
#define LCD_Contrast_PWM_GPIO_Port GPIOC
#define LCD_LED_PWM_Pin GPIO_PIN_7
#define LCD_LED_PWM_GPIO_Port GPIOC
#define LCD_RS_Output_Pin GPIO_PIN_1
#define LCD_RS_Output_GPIO_Port GPIOD
#define LCD_EN_Output_Pin GPIO_PIN_2
#define LCD_EN_Output_GPIO_Port GPIOD
#define LCD_D4_Output_Pin GPIO_PIN_3
#define LCD_D4_Output_GPIO_Port GPIOD
#define LCD_D5_Output_Pin GPIO_PIN_4
#define LCD_D5_Output_GPIO_Port GPIOD
#define LCD_D6_Output_Pin GPIO_PIN_6
#define LCD_D6_Output_GPIO_Port GPIOD
#define LCD_D7_Output_Pin GPIO_PIN_7
#define LCD_D7_Output_GPIO_Port GPIOD

//* Communication Defines

//* UART4 For ESP
#define STMUART4_TX_ESPUART1RX_Pin GPIO_PIN_10
#define STMUART4_TX_ESPUART1RX_GPIO_Port GPIOC
#define STMUART4_RX_ESPUART1TX_Pin GPIO_PIN_11
#define STMUART4_RX_ESPUART1TX_GPIO_Port GPIOC

//* SPI
#define SPI_nSS_Pin GPIO_PIN_15
#define SPI_nSS_GPIO_Port GPIOA
#define SPI_SCK_Pin GPIO_PIN_3
#define SPI_SCK_GPIO_Port GPIOB
#define SPI_MISO_Pin GPIO_PIN_4
#define SPI_MISO_GPIO_Port GPIOB
#define SPI_MOSI_Pin GPIO_PIN_5
#define SPI_MOSI_GPIO_Port GPIOB

//USB Flag Check
#define USBFlag_Interrupt_PullUp_Pin GPIO_PIN_12
#define USBFlag_Interrupt_PullUp_GPIO_Port GPIOB
#define USBFlag_Interrupt_PullUp_EXTI_IRQn EXTI15_10_IRQn
#define USB_EN_Output_Pin GPIO_PIN_8
#define USB_EN_Output_GPIO_Port GPIOB

//*USART1 For Debugging & STlink
#define UART1_TX_STLINK_RX_Pin GPIO_PIN_6
#define UART1_TX_STLINK_RX_GPIO_Port GPIOB
#define UART1_RX_STLINK_TX_Pin GPIO_PIN_7
#define UART1_RX_STLINK_TX_GPIO_Port GPIOB

//*USART3 For LPC Controller
#define UART3TX_FC1RX_Pin GPIO_PIN_10
#define UART3TX_FC1RX_GPIO_Port GPIOB
#define UART3RX_FC1TX_Pin GPIO_PIN_11
#define UART3RX_FC1TX_GPIO_Port GPIOB
#define UART3CTS_FC1RTS_Pin GPIO_PIN_13
#define UART3CTS_FC1RTS_GPIO_Port GPIOB
#define UART3RTS_FC1CTS_Pin GPIO_PIN_14
#define UART3RTS_FC1CTS_GPIO_Port GPIOB
#define nRESET_LPC_Output_Pin GPIO_PIN_8
#define nRESET_LPC_Output_GPIO_Port GPIOD
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
