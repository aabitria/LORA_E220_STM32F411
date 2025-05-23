/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
	uint8_t conf[3] = {0xC1, 0x00, 0x09};
	uint8_t resp[16] = {0};
	uint8_t wconf[16] = {0};
	char payload[20] = "Hello World!";
	char message[32] = {0};
	uint8_t count = 0;

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
//  HAL_GPIO_WritePin(E220_M0_GPIO_Port, E220_M0_Pin, GPIO_PIN_SET);
//  HAL_GPIO_WritePin(E220_M1_GPIO_Port, E220_M1_Pin, GPIO_PIN_SET);
//
//  HAL_Delay(500);
//  HAL_GPIO_WritePin(E220_M0_GPIO_Port, E220_M0_Pin, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(E220_M1_GPIO_Port, E220_M1_Pin, GPIO_PIN_RESET);
//
//  conf[0] = 0xC1;
//  conf[1] = 0x00;
//  conf[2] = 0x09;
//
//  HAL_Delay(500);
//  HAL_GPIO_WritePin(E220_M0_GPIO_Port, E220_M0_Pin, GPIO_PIN_SET);
//  HAL_GPIO_WritePin(E220_M1_GPIO_Port, E220_M1_Pin, GPIO_PIN_SET);

  HAL_UART_Transmit(&huart1, conf, 3, 1000);

  if (HAL_GPIO_ReadPin(E220_AUX_GPIO_Port, E220_AUX_Pin) == GPIO_PIN_RESET)
  {
	  HAL_UART_Receive(&huart1, resp, (3 + conf[2]), 1000);
  }
  else
  {
	  while(1);
  }

  HAL_Delay(20);

  wconf[0] = 0xC0;
  wconf[1] = 0;
  wconf[2] = 0x08;

  memcpy(&wconf[3], &resp[3], conf[2]);

  wconf[2 + 5] = 0x41;
  wconf[2 + 3] = 0x62;
  wconf[2 + 1] = 0x00;
  wconf[2 + 2] = 0x03;
  wconf[2 + 6] |= (1 << 6);

  memset(resp, 0, sizeof(resp));

  HAL_Delay(5);

  HAL_UART_Transmit(&huart1, wconf, (3 + wconf[2]), 1000);

  if (HAL_GPIO_ReadPin(E220_AUX_GPIO_Port, E220_AUX_Pin) == GPIO_PIN_RESET)
  {
	  HAL_UART_Receive(&huart1, resp, (3 + wconf[2]), 1000);
  }
  else
  {
	  while(1);
  }

  HAL_GPIO_WritePin(E220_M0_GPIO_Port, E220_M0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(E220_M1_GPIO_Port, E220_M1_Pin, GPIO_PIN_RESET);
  HAL_Delay(15);

//  while (HAL_GPIO_ReadPin(E220_AUX_GPIO_Port, E220_AUX_Pin) == GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  payload[12] = 0x30 + (count % 10);
	  message[0] = 0x00;	// ADDRH
	  message[1] = 0x01;	// ADDRL
	  message[2] = 0x41;	// CHAN
	  memcpy(&message[3], payload, strlen(payload));
	  HAL_UART_Transmit(&huart1, (uint8_t*)message, (3+strlen(payload)), 1000);
      count++;
	  HAL_Delay(500);
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
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
