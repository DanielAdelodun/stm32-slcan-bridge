/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "slcan.h"
#include "led.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEST
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
void strcopy(uint8_t *, uint8_t *);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART3_UART_Init();
  MX_CAN1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  // CAN Headers, Data Buffers, and Holders for the Command String & it's Length
  CAN_TxHeaderTypeDef TxHeader;
  CAN_RxHeaderTypeDef RxHeader;
  uint8_t aData[8];
  uint32_t Len;

  HAL_Delay(3000);
#ifdef TEST
  // BEGIN SLCAN TEST
  // Using SLCAN_CommandStringRx for tests so that no weird conflicts happen with SLCAN_CommandStringTx in USB receive callback
  // Test LEDs
  LED_GreenOn();
  LED_BlueOn();
  LED_RedOn();
  HAL_Delay(250);
  LED_Process();
  uint8_t TestResult = SLCAN_OK;
  uint32_t Tx_Mailbox;
  // Test Setting Up Filter (0's) --> M
  // Try ExtId Filter
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"M0000000\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2 + SLCAN_EXT_ID_LEN, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Try StdId Filter
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"M000\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2 + SLCAN_STD_ID_LEN, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Setting up a Mask (0's) --> m
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"m000\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2 + SLCAN_STD_ID_LEN, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Filter+Mask Request
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"MM\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 3, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Speed Change --> 500kbit/s
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"s6\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 3, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Mode Switch (Loopback + Silent(Listen) Mode)
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"L3\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 3, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Opening Connection
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"O\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Transmit
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"T1122334481122334455667788EA5F\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, SLCAN_MTU, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  else if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, aData, &Tx_Mailbox) != HAL_OK) {
  					  TestResult = SLCAN_ERROR;
  }

  HAL_Delay(500);
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"t12381122334455667788EA5F\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, SLCAN_MTU, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  else if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, aData, &Tx_Mailbox) != HAL_OK) {
  					  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Receive (No Timestamps) <Should be messages available from Test Mode + Transmit Tests>
  while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0) {
	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, aData) == HAL_OK) {
		if (SLCAN_Parse_Frame(&RxHeader, aData, SLCAN_CommandStringRx, &Len) == SLCAN_OK) {
			if (CDC_Transmit_FS(SLCAN_CommandStringRx, Len) == HAL_OK) {
				;
			}
			else {
				TestResult = SLCAN_ERROR;
			}
		}
		else {
			TestResult = SLCAN_ERROR; 	// This means the HAL_CAN_GetRxMessage call failed - which would be weird
		}
	}
  }
  // Test Timestamps
  HAL_Delay(500);
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"Z\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"Z0\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 3, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"Z1\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 3, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Transmit (Again, to test Timestamps)
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"T1122334481122334455667788EA5F\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, SLCAN_MTU, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  else if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, aData, &Tx_Mailbox) != HAL_OK) {
  					  TestResult = SLCAN_ERROR;
  }

  HAL_Delay(500);
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"t12381122334455667788EA5F\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, SLCAN_MTU, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  else if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, aData, &Tx_Mailbox) != HAL_OK) {
  					  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Receive (Timestamps)
  while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0) {
	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, aData) == HAL_OK) {
		if (SLCAN_Parse_Frame(&RxHeader, aData, SLCAN_CommandStringRx, &Len) == SLCAN_OK) {
			if (CDC_Transmit_FS(SLCAN_CommandStringRx, Len) == HAL_OK) {
				;
			}
			else {
				TestResult = SLCAN_ERROR;
			}
		}
		else {
			TestResult = SLCAN_ERROR;
		}
	}
  }
  HAL_Delay(500);
  // Test Closing Connection
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"C\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(500);
  // Test Read Status Flags
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"F\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  // END SLCAN TEST
  HAL_Delay(200);
  ClearSLCAN();
  strcopy(SLCAN_CommandStringRx, (uint8_t *)"Z\r");
  if (SLCAN_Parse_Str(SLCAN_CommandStringRx, 2, &TxHeader, aData) != SLCAN_OK) {
	  TestResult = SLCAN_ERROR;
  }
  HAL_Delay(200);
  LED_Process();
  if (TestResult != SLCAN_OK) {
	  LED_RedForceOn();
	  HAL_Delay(5000);
  }
  else {
	  LED_GreenOn();
  }
  HAL_Delay(500);
#endif
#ifndef TEST
  CAN_SetFilter(hcan1, 0, 0);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	// Turn off LEDs that have been on long enough
	LED_Process();

	// Parse and send (to USB) any received CAN frames
	if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0) {
		if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, aData) == HAL_OK) {
			if (SLCAN_Parse_Frame(&RxHeader, aData, SLCAN_CommandStringRx, &Len) == SLCAN_OK) {
				if (CDC_Transmit_FS(SLCAN_CommandStringRx, Len) == HAL_OK) {
					LED_GreenOn();
				}
				else {
					; // Add Handler for failed transmission
				}
			}
			else {
				; // Add Handler for failed SLCAN parse
			}
		}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLQ;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 24;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// Copy string1 <-- string2
void strcopy(uint8_t *string1, uint8_t *string2){
	int i = 0;
	for (i = 0; string2[i] != '\0'; ++i) {
		string1[i] = string2[i];
	}
 	string1[i] = '\0';
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
