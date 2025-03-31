/*
 * LoadCells.c
 *
 *  Created on: oct 9, 2024
 *      Author: Abhi
 */

// LoadCells.c
#include <main.h>

#define SYNCWORD 0xAAAAAAAA    // Sync word at the beginning of the message
#define  AVERAGE  1  //change 2 to 1 to deactivate average filter
#define RDNUM 5
#define FILTER_B 1
long AllReadings[RDNUM]; // Array for storing readings

extern UART_HandleTypeDef huart3;
extern uint8_t  rxBuffer[8];// UART handle (assume USART1 is used)

uint8_t dst[8];

/**
  * @brief  Function is future filter for Weight so that peeks are avoided.
  * @Param  Current Weight  or total weight
  * @retval None
  */
void futureFilter(int current_A ) {

	// Shift readings in the array for new entry

	if (FILTER_B == 1) {
		for (int i = 0; i < RDNUM - 1; i++) {
			AllReadings[i] = AllReadings[i + 1];
		}
		AllReadings[RDNUM - 1] = current_A;
		// Check and apply specific filtering criteria
		if (AllReadings[1] < AllReadings[RDNUM / 2]
				&& AllReadings[RDNUM / 2] > AllReadings[RDNUM - 1]) {
			for (int i = 1; i < RDNUM - 1; i++) {
				AllReadings[i] = AllReadings[0];
			}
		} else if (AllReadings[1] > AllReadings[RDNUM / 2] && AllReadings[RDNUM / 2] < AllReadings[RDNUM - 1]) {
			for (int i = 1; i < RDNUM - 1; i++) {
				AllReadings[i] = AllReadings[0];
			}
		} else if (AllReadings[1] < AllReadings[RDNUM / 3] && AllReadings[RDNUM / 3] < AllReadings[RDNUM / 2] && AllReadings[RDNUM / 2] < AllReadings[RDNUM - 1]) {
			for (int i = 0; i < RDNUM - 1; i++) {
				AllReadings[i] = AllReadings[RDNUM - 1];
			}
		} else if (AllReadings[1] > AllReadings[RDNUM / 3] && AllReadings[RDNUM / 3] > AllReadings[RDNUM / 2] && AllReadings[RDNUM / 2] > AllReadings[RDNUM - 1]) {
			for (int i = 0; i < RDNUM - 1; i++) {
				AllReadings[i] = AllReadings[RDNUM - 1];
			}
		} else if (AllReadings[RDNUM - 1] < 100) {
			for (int i = 0; i < RDNUM - 1; i++) {
				AllReadings[i] = AllReadings[RDNUM - 1];
			}
		}
		Total_Weight = AllReadings[0];
	}
	else{
		Total_Weight = current_A;
	}
}
/**
  * @brief  Function to send the Weight after Sending TARE command.
  * @Param   TareWeight which is the total weight or the weight when not zero
  * @retval None
  */
static void SendTareWeight(int Tareweight) {
    char buffer[7]= {0};// Allocate enough space for the number (max 10 digits)
    sprintf(buffer, "%d", Tareweight);  // Convert integer to string

    // Transmit data using DMA
    if (HAL_UART_Transmit_DMA(&huart3, (uint8_t*)buffer,7) != HAL_OK) {
        Error_Handler();  // Handle errors if transmission fails
    }
}

/**
  * @brief  Function to send the "TARE" command using interrupt-based transmission.
  * @retval None
  */
void TareAll(void)
{
	 HAL_Delay(2000);
    // Start interrupt-based transmission of the "TARE" string
	   uint8_t tare[4] = "TARE";

	   //sprintf(char_buffer, "%s%d", Data,Total_Weight);
	   if(HAL_UART_Transmit_DMA(&huart3, tare, sizeof(tare))!= HAL_OK)
	   {
			  Error_Handler();
	   }
	   HAL_Delay(10);
	   SendTareWeight(tareSend);
	   SerialPrintln("Taring the system");
	   HAL_Delay(1000);
}

/**
  * @brief  Processes the received 8-byte message and displays the weight if valid.
  * @param  message: Pointer to the received 8-byte message buffer.
  * @retval None
  */
void ReadLoadCellAVG()
{
	int current_A = 0;
	//startUARTReception();
    uint32_t syncWord = (rxBuffer[0] << 24) | (rxBuffer[1] << 16) | (rxBuffer[2] << 8) | rxBuffer[3];
    if (syncWord != SYNCWORD)
    {
        return;
    }
    //Extract the weight values
    int16_t value1 = (rxBuffer[4] << 8) | rxBuffer[5];  // First weight value
    int16_t value2 = (rxBuffer[6] << 8) | rxBuffer[7];  // Repeated weight value
    if (value1 != value2)
    {
        return;
    }
   // value1 = value1 * CORRECTION_FACTOR;
    for(int i = 0; i < AVERAGE; i++){
       	current_A = current_A + value1;
    }
    current_A = current_A / AVERAGE;
    //futureFilter(current_A);
    Total_Weight = current_A;
    // Re-enable UART reception for the next message
    //startUARTReception();
}

