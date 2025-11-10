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
#include "cmsis_os.h"
#include <stdio.h>
#include "stdint.h"

UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);

int uart2_write(int ch);
int __io_putchar(int ch);

void SenderTask1(void *pvParameters);
void SenderTask2(void *pvParameters);
void ReceiverTask(void *pvParameters);

TaskHandle_t SendTask, ReceiveTask;

QueueHandle_t yearQueue;

int main(void)
{

  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialise all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* commenting delay in the tasks make the data rate faster*/
  /* Reason we need greater heap size*/
  yearQueue = xQueueCreate(20, sizeof(int32_t));

  xTaskCreate(SenderTask1,
		      "Sender Task",
			  100,
			  NULL,
			  1,
			  NULL);
  xTaskCreate(SenderTask2,
 		      "Sender Task",
 			  100,
 			  NULL,
 			  1,
 			  NULL);

  xTaskCreate(ReceiverTask,
		      "Receiver Task",
			  100,
			  NULL,
			  2,
			  NULL);


  vTaskStartScheduler();

  while (1)
  {
	 printf("Bismillah \n\r");
  }
}


void SenderTask1(void *pvParameters)
{
	int32_t value_to_Send = 2050;
	BaseType_t qStatus;

	while(1)
	{
		qStatus = xQueueSend(yearQueue,&value_to_Send,0);

		if (qStatus != pdPASS)
		{
			printf("Queue Send Error has occurred! \n\r");
		}
		//vTaskDelay(pdMS_TO_TICKS(100));

	}
}

void SenderTask2(void *pvParameters)
{
	int32_t value_to_Send = 5500;
	BaseType_t qStatus;

	while(1)
	{
		qStatus = xQueueSend(yearQueue,&value_to_Send,0);

		if (qStatus != pdPASS)
		{
			printf("Queue Send Error has occurred! \n\r");
		}
		//vTaskDelay(pdMS_TO_TICKS(100));

	}
}
void ReceiverTask(void *pvParameters)
{
	int32_t value_Received;
	BaseType_t qStatus;
	const TickType_t waiting_time = pdMS_TO_TICKS(100);

	while(1)
	{
		qStatus = xQueueReceive(yearQueue,&value_Received,waiting_time);
		if (qStatus  == pdPASS)
		{
			printf("The value Received is %ld!...... \n\r", value_Received);
		}
		else
		{
			printf("Error: could not receive...\n\r");
		}
	}
}

int uart2_write(int ch)
{
	while(!(USART2->SR & 0x0080)){}
	USART2->DR = (ch & 0xFF);
	return ch;
}

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initialises the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initialises the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}



/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
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
