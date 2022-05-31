/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "tim.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "storage.h"
#include "sample-lib.h"
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DS18B20_PORT GPIOA
#define DS18B20_PIN GPIO_PIN_1
#define MAX_SIZE_RECEIVE 30

// p1s4 defines //
#define MAX_INPUT_COMMAND 100
#define MAX_INPUT_LED 10
#define MAX_INPUT_STATUS 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// p1s2 variables //
  
// transmiter messages //
uint8_t TxMessage[] = "\n\r ping\n\r";

// receiver messages //
uint8_t RxMessage[MAX_SIZE_RECEIVE] = {'\0'};
int txrx ;

// p1s4 variables //

uint8_t uartreadmessage[100]; //  uart receive(config.c) the whole message //
char command[MAX_INPUT_COMMAND] = {'\0'}; // uart receive(config.c) user command //
char led[MAX_INPUT_LED]; // uart receive(config.c) led option //
char status[MAX_INPUT_STATUS]; // uart receive(config.c) selected status of led //
uint8_t uartresponseled[30] = "operation done!\n"; // after calling of command send a message back to congig //
int i; // uartreadmessage index //
int j;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
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
   txrx = 1; // transmitter  1, receiver 0//
  // txrx = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_LPUART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /********************** DHT11 *********************/
  
  while (1)
  {
   // ============================= P1S4 ====================== //

    // printf("hello world\n\r");
    // strcpy(uartreadmessage, "hello_world");
    // HAL_UART_Transmit(&huart1, uartreadmessage, sizeof(uartreadmessage), 100);
    for (i = 0 ; i < 100 ; i++)
      {
        uartreadmessage[i] = '\0';
      }

    for (i = 0 ; i < MAX_INPUT_LED ; i++)
      {
        led[i] = '\0';
      }
    for (i = 0 ; i < MAX_INPUT_STATUS ; i++)
      {
        status[i] = '\0';
      }

    // uartreadmessage[0] = '\0';
    // wait for message from config //
    HAL_UART_Receive(&huart1, uartreadmessage, sizeof(uartreadmessage), 1000);
    i = 0;
    
    printf("receive command: %s\n\r", uartreadmessage); 
    // if nothing received, repeat the receive proceedure //
    if (uartreadmessage[0] == '\0')
      {
        continue;
      }
    i = 0;

    // delimiter //
    // printf("strlen uartreadmessage %d\n\r", strlen((char* )uartreadmessage));
    for (i = 0 ; i < strlen(uartreadmessage) ; i++)
      {
        if (uartreadmessage[i] != '*')
          {
            command[i] = uartreadmessage[i];
          }
        else break ;
      }
    // printf("command = %s\n\r", command);
    

    if (strcmp(command, "set_led\n") == 0)
      { 
        printf("set_led command\n\r");
        // message format command*led*status* //
        // delimiter //
       
        // delimiter //
        // printf("led %s i  %d\n\r", led, i);
        j = 0;
        for (i = i + 1 ; i < strlen(uartreadmessage) ; i++)
          {
            if (uartreadmessage[i] != '*')
              {
                led[j] = uartreadmessage[i];
                j++;
              }
            else break ;
          }
        j = 0;
        printf("led %s", led);
        for (i = i + 1 ; i < strlen(uartreadmessage) ; i++)
          {
            if (uartreadmessage[i] != '*')
              {
                status[j] = uartreadmessage[i];
                j++;
              }
            else break ;
          }
         // call the function //
        printf("status %s\n", status);
        printf("strcmp  %d\n", strcmp(led, "red\n"));
        
         if (strcmp(led, "red\n") == 0)
          {
            printf("eftasa red\n");
            if (strcmp(status, "0") == 0)
              {
                set_led(LED_RED, LED_OFF); 
              }
            else 
              {
                set_led(LED_RED, LED_ON); 
              }
          }
        else if (strcmp(led, "blue\n") == 0)
          {
            printf("eftasa blue\n");
            if (strcmp(status, "0") == 0)
              {
                set_led(LED_BLUE, LED_OFF); 
              }
            else 
              {
                set_led(LED_BLUE, LED_ON); 
              }
          }
        else if (strcmp(led, "green\n") == 0)
          {
            printf("eftasa green\n");
            if (strcmp(status, "0") == 0)
              {
                set_led(LED_GREEN, LED_OFF); 
              }
            else 
              {
                set_led(LED_GREEN, LED_ON); 
              }
          }
      }
    //     // send response to config to inform that command is submitted successfully //
	      //  HAL_UART_Transmit(&huart1, (uint8_t *)uartresponseled, sizeof(uartresponseled), 0xFFFF);
    //   }
       /*
      if (strcmp(command, "storage_write"))
          { 
            // message format command*address*data* //
            // delimiter //
            while(uartreadmessage[i] != '*')
              {
                idx[i] = uartreadmessage[i];
                i++;
              }
            i++; 
            // delimiter //
            while(uartreadmessage[i] != '*')
              {
                data[i] = uartreadmessage[i];
                i++;
              }
            // call the function //
            // idx : startaddress string  //
            // 16 : hexadecimal           //
            storage_write(strtol(idx, NULL, 16), (uint8_t*) data, strlen(data);
            // send response to config to inform that command is submitted successfully //
            HAL_UART_Transmit(&huart1, (uint8_t *)uartresponsestorage, sizeof(uartresponseled), 0xFFFF);
          }
        if (strcmp(command, "storage_read"))
          {
            // message format command*address*len* //
            // delimiter //
            while(uartreadmessage[i] != '*')
              {
                idx[i] = uartreadmessage[i];
                i++;
              }
            i++; 
            // delimiter //
            while(uartreadmessage[i] != '*')
              {
                len[i] = uartreadmessage[i];
                i++;
              }
            // call the function //
            // idx : startaddress string  //
            storage_read(strtol(idx, NULL, 16), (uint8_t*) data, (uint32_t) atoi(len)); 
            // send response to config to inform that command is submitted successfully //
            HAL_UART_Transmit(&huart1, (uint8_t *)uartresponsestorage, sizeof(uartresponseled), 0xFFFF); 
          }
    */
    // ============================= P1S4 ====================== //

    // ============================= P1S1 ====================== //

    //hello_world();

    // ============================= P1S1 ====================== //


	  

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

