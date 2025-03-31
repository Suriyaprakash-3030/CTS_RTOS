/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
//#include "timers.h"
#include "queue.h"
#include "semphr.h"
//#include "event_groups.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define  MAJOR_VERSION  6

#define  MINOR_VERSION  0
#define  PATCH_VERSION  0
// 6 new Work flow chart implementation
// 5 water valve open for 2s in auto operation, previously 0.5s.
// 4 average filter added but not used, future filter added with RDNUM value 5.
// 3 removed auto Taring from developer Menu
                        // 1  Added Valve open and close before auto dispensing removed auto Taring from developer Menu
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//void MX_FREERTOS_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;
//uint8_t txBuffer[5] = "TARE";
uint8_t rxBuffer[8];
extern uint8_t dst[8];
uint8_t rxBuffer1[11];
uint8_t dst1[11];

char VERSION_NO[10];  // Allocate a buffer for the version string



//Mutex

SemaphoreHandle_t Mutex_Instance;
SemaphoreHandle_t LCDMutex;
SemaphoreHandle_t SDCardMutex;
SemaphoreHandle_t ButtonMutex;


TaskHandle_t xCustomerAppTaskEndHandle;






/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM11_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_HCD_Init();
  MX_TIM2_Init();
  MX_TIM7_Init();
  MX_TIM10_Init();
  MX_FATFS_Init();
  MX_TIM6_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(3000);
	// Start Timer 7 in interrupt mode
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim2);
  /** ENABLING LPC  AND THE LOAD CELLS WILL GET ACTIVATED **/
  HAL_GPIO_WritePin(GPIOC, EN_DCDC_5V_Output_Pin|EN_DCDC_8V_Output_Pin, GPIO_PIN_SET);
  //HAL_GPIO_WritePin(GPIOD, nRESET_LPC_Output_Pin, GPIO_PIN_SET);
  /********************************************************/
  HAL_Delay(1000);
  moveMotor(VALVEMOTOR, OFF, INT_INF);
  Servo_Init();
  digitalWrite(Spindle_M3_Dir_Output, LOW);
  digitalWrite(Clamp_M2_Dir_Output, LOW);
  pwm(Spindle_M3_PWM, 0);
  pwm(Clamp_M2_PWM, 0);
  sd_Card_Check();

  if (isMachineOn == FALSE) {
		goToSleep(); // Ensure goToSleep is adapted for STM32
	}

  //HAL_GPIO_WritePin(GPIOC, EN_DCDC_5V_Output_Pin|EN_DCDC_8V_Output_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOD, nRESET_LPC_Output_Pin, GPIO_PIN_SET);
  //HAL_Delay(1000);
  HAL_TIM_Base_Start_IT(&htim6);
  /* testing in the go sleep for faster response
	  MX_TIM8_Init();
	  LCD_Init();
	  LCDRGBControl(WHITE);
	  controlLCDContrast(100);
  */
  /* THIS iS DEVELOPER AND TESTING PURPOSE ONLE */
  printLCD(0, "SOFTWARE VER ", NO_SYMBOL);
  snprintf(VERSION_NO, sizeof(VERSION_NO), "%d.%d.%d ", MAJOR_VERSION,MINOR_VERSION,PATCH_VERSION);
  printLCD(1, VERSION_NO, NO_SYMBOL);
  /************************************************/

  CTS_Home_position();
  HAL_Delay(100);
  HAL_UART_Receive_DMA(&huart3, rxBuffer, 8);
  HAL_UART_Receive_DMA(&huart1, rxBuffer1, 11);
  HAL_TIM_Base_Start_IT(&htim10);
  LoadTimeFromSDCard(0,0,0);
  UpdateTimeToSDCard(hours,  minutes,  seconds);




  LCDMutex = xSemaphoreCreateMutex();
  SDCardMutex = xSemaphoreCreateMutex();
  ButtonMutex = xSemaphoreCreateMutex();


  //Mutex Test
 /* if(xSemaphoreCreateMutex() != NULL)
  {
	  SerialPrint("Mutex_Working\n");
  }*/

//Task


//  	 xTaskCreate(vTaskLCDHandler, "LCD_Handler", 256, NULL, 2, NULL);
//     xTaskCreate(vTaskButtonHandler, "Button_Handler", 256, NULL, 3, NULL);
//     xTaskCreate(vTaskSDCardLogger, "SDCard_Logger", 256, NULL, 2, NULL);
//     xTaskCreate(vTaskMotorControl, "Motor_Control", 256, NULL, 4, NULL);
     xTaskCreate(CustomerFrontEnd, "Customer_App", 256, NULL, 2, &xCustomerAppTaskEndHandle);


vTaskStartScheduler();
  /* USER CODE END 2 */


  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
 {
	  if (ProgramToRun == CUSTOMERAPP) {
		  xTaskNotifyGive(xCustomerAppTaskEndHandle);
		}
	  else if (ProgramToRun == TESTAPP) {
			MotorTest();
	  }
	  else {
			DemoApp();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(TIM7_IRQn);
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* EXTI9_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  /* EXTI4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  /* EXTI2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  /* EXTI1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
  /* EXTI0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  /* PVD_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PVD_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(PVD_IRQn);
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   if (huart->Instance == USART3)
   {
	   memcpy(dst,rxBuffer,sizeof(rxBuffer));
	   ReadLoadCellAVG();
       HAL_UART_Receive_DMA(huart, rxBuffer, 8);
   }

}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
