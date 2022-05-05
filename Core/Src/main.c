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

// p1s4 defines //
#define MAX_INPUT_COMMAND 100
#define MAX_INPUT_LED 10
#define MAX_INPUT_STATUS 2

// DHT11 DS18B20 //
#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

// p1s5 defines //
#define MAX_INPUT_ADDRESS 10
#define MAX_INPUT_DATA 30 
#define MAX_INPUT_LEN 3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// p1s2 variables //
  
// transmiter messages //


// receiver messages //
uint8_t Rxresponse[30];


uint8_t TxMessage[] = "\n\r ping\n\r";
uint8_t RxMessage[] = "\n\r pong\n\r";
int txrx ;

// p1s3 variables //
// DHT11 DS18B20 //
uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM, RH, TEMP;
float Temperature = 0;
float Humidity = 0;
uint8_t Presence = 0;


// p1s4 variables //

uint8_t uartreadmessage[100]; //  uart receive(config.c) the whole message //
char command[MAX_INPUT_COMMAND] = {'\0'}; // uart receive(config.c) user command //
char led[MAX_INPUT_LED]; // uart receive(config.c) led option //
char status[MAX_INPUT_STATUS]; // uart receive(config.c) selected status of led //
uint8_t uartresponseled[30] = "operation done!\n"; // after calling of command send a message back to congig //
int i; // uartreadmessage index //


// p1s5 variables //
//  storage write variables //
char idx[10]; // uart receive(config.c) startaddress // 
char data[MAX_INPUT_DATA];  // uart receive(config.c) user input data to be written at startaddress //
uint8_t uartresponsestorage[30] = "operation done!\n"; // after calling of command send a message back to congig //

// storage read variables //
char len[MAX_INPUT_LEN];  // uart receive(config.c) len of data to read //
 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// ==================== P1S3 ============== //

void delay (uint16_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim2))<time);
}
/*********************************** DHT11 FUNCTIONS p1s3 ********************************************/

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT11_Start (void)
{
	Set_Pin_Output (DHT11_PORT, DHT11_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
	delay (18000);   // wait for 18ms
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
	delay (20);   // wait for 20us
	Set_Pin_Input(DHT11_PORT, DHT11_PIN);    // set as input
}

uint8_t DHT11_Check_Response (void)
{
	uint8_t Response = 0;
	delay (40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay (80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
		else Response = -1; // 255
	}
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go low

	return Response;
}

uint8_t DHT11_Read (void)
{
	uint8_t i = 0;
	uint8_t j = 0;

	for (j = 0; j < 8; j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go high
		delay (40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
		{
			i &= ~(1 << (7 - j));   // write 0
		}
		else i |= (1 << (7 - j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));  // wait for the pin to go low
	}
	return i;
}

/*********************************** DS18B20 FUNCTIONS p1s3****************************************/
uint8_t DS18B20_Start (void)
{
	uint8_t Response = 0;
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);   // set the pin as output
	HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin low
	delay (480);   // delay according to datasheet

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);    // set the pin as input
	delay (80);    // delay according to datasheet

	if (!(HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = -1;

	delay (400); // 480 us delay totally.

	return Response;
}

void DS18B20_Write (uint8_t data)
{
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output

	for (int i = 0; i < 8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			delay (1);  // wait for 1 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // set as input
			delay (50);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, 0);  // pull the pin LOW
			delay (50);  // wait for 60 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
		}
	}
}

// ==================== P1S3 ============== //

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
   txrx = 1; // transmitter //
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
    /*
    // ============================= P1S5 ====================== //

    // ============================= P1S4 ====================== //
    
    // wait for message from config //
    HAL_UART_Receive(&huart1, uartreadmessage, sizeof(uartreadmessage), 100);

    i = 0;
    // delimiter //
    while(uartreadmessage[i] != '*')
      {
        command[i] = uartreadmessage[i];
        i++;
      }
    i++;
    if (strcmp(command, "set_led"))
      { 
        // message format command*led*status* //
        // delimiter //
        while(uartreadmessage[i] != '*')
          {
            led[i] = uartreadmessage[i];
            i++;
          }
        i++; 
        // delimiter //
        while(uartreadmessage[i] != '*')
          {
            status[i] = uartreadmessage[i];
            i++;
          }
         // call the function //
         if (strcmp(led, "red") == 0)
          {
              if (strcmp(status, "0") == 0)
              {
                set_led(LED_RED, LED_OFF); 
              }
              else 
                {
                  set_led(LED_RED, LED_ON); 
                }
          }
        else if (strcmp(led, "blue") == 0)
          {
            if (strcmp(status, "0") == 0)
              {
                set_led(LED_BLUE, LED_OFF); 
              }
            else 
              {
                set_led(LED_BLUE, LED_ON); 
              }
          }
        else if (strcmp(led, "green") == 0)
          {
            if (strcmp(status, "0") == 0)
              {
                set_led(LED_GREEN, LED_OFF); 
              }
            else 
              {
                set_led(LED_GREEN, LED_ON); 
              }
          }

        // send response to config to inform that command is submitted successfully //
	      HAL_UART_Transmit(&huart1, (uint8_t *)uartresponseled, sizeof(uartresponseled), 0xFFFF);
      }
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
    
    // ============================= P1S4 ====================== //

    // ============================= P1S3 ====================== //
    
    DHT11_Start();
    Presence = DHT11_Check_Response();
    Rh_byte1 = DHT11_Read ();
    Rh_byte2 = DHT11_Read ();
    Temp_byte1 = DHT11_Read ();
    Temp_byte2 = DHT11_Read ();
    SUM = DHT11_Read();

    TEMP = Temp_byte1;
    RH = Rh_byte1;

    Temperature = (float) TEMP;
    Humidity = (float) RH;
    
    
    // ============================= P1S3 ====================== //
    */
    // ============================= P1S2 ====================== //
    
    //   ***********************  TRANSMITTER *********************************  //
    // Wait for User push-button (SW1) press before starting the Communication   //
    // After user pushes the SW1, transmitter board sends a message to receiver  //
    // Blue led remains turned on for 2'' to indicate that the message has sent  //
  
    if (txrx)
      {
        printf("transmitter starts\n\r");
        // read the state of button 1 //
         if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == 1)
           {   
              printf("pushed button\n\r");

              HAL_Delay(2000);

              // send a message to receiver //
              HAL_UART_Transmit(&hlpuart1, TxMessage, strlen((char*)TxMessage), 0xFFFF);

              // led blue state becomes on to indicate that a message has just been sent //
              set_led(LED_BLUE, LED_ON);

              printf("transmitter ping\n\r");
              HAL_Delay(100);
              
              // led blue state becomes off to indicate that a message has just been received //
              set_led(LED_BLUE, LED_OFF); 
              txrx = 0;
              
          }
      }
    //   ***********************  RECEIVER ********************************* //
    // waits for 2'' to receive a message                                    //
    else 
      {
        printf("Receiver starts!\n\r");

        // receive a message //
        HAL_UART_Receive(&hlpuart1, RxMessage, strlen((char*)TxMessage), 1000);

        printf("received message: %s\n\r", RxMessage);

        // led blue state becomes on to indicate that a message has just been received //
        set_led(LED_BLUE, LED_ON); 

        // duration that blue led will be turned on //
        HAL_Delay(100);
        
        // turn off led blue //
        set_led(LED_BLUE, LED_OFF); 
        txrx = 1;
      }

    // ============================= P1S2 ====================== //
   
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

