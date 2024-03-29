
/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include <main.hh>
#include <Voltmeter.hh>
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "xbee.h"
//#include "Rocket.hh"
#include <string>
#include "Igniter.hh"
#include "hx711.hh"
#include "L298.hh"
#include "Bluetooth.h"
#include "Voltmeter.hh"
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
enum state {
	Init = 0,
	Idle = 1,
	Fueling = 2,
	Countdown = 3,
	Flight = 4,
	Abort = 5
};

volatile state currState = Init;
volatile int32_t buf = -1, buf2 = -1, buf3 = -1;
char dataIn[30];
char dataOut[30];
Xbee communication;
//Rocket R4(std::make_shared<Motor>(FillMotor), std::make_shared<Motor>(DeprMotor), std::make_shared<Motor>(QDMotor), std::make_shared<Igniter>(igniter));

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  HAL_Delay(1000);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); test timer
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart2, (uint8_t*) xbee_rx.mess_loaded, DATA_LENGTH);
	xbee_init(&communication, 0x0013A20041C283E5, &huart2); //inicjalizacja modułu xbee

	///ADDED FOR BLUETOOTH///
	//__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
	// HAL_GPIO_WritePin(Bluetooth_reset_GPIO_Port, Bluetooth_reset_Pin, SET);//ADDITIONAL PIN PC14 FOR RESET //

	//memset(buff ,0,sizeof(buff));

	 HAL_UART_Transmit(&huart3, (uint8_t*)"INIT\n", strlen("INIT\n"), 500);
	/* USER CODE END 2 */

	/* USER CODE BEGIN WHILE */
	Motor FillMotor(FILL_OPEN_GPIO_Port, FILL_OPEN_Pin,	FILL_CLOSE_GPIO_Port, FILL_CLOSE_Pin, &htim4, TIM_CHANNEL_3,
				FILL_OPEN_LIMIT_SW_GPIO_Port, FILL_OPEN_LIMIT_SW_Pin,	FILL_CLOSE_LIMIT_SW_GPIO_Port, FILL_CLOSE_LIMIT_SW_Pin);
	Motor DeprMotor(DEPR_OPEN_GPIO_Port, DEPR_OPEN_Pin,	DEPR_CLOSE_GPIO_Port, DEPR_CLOSE_Pin, &htim3, TIM_CHANNEL_2,
				DEPR_OPEN_LIMIT_SW_GPIO_Port, DEPR_OPEN_LIMIT_SW_Pin, DEPR_CLOSE_LIMIT_SW_GPIO_Port, DEPR_CLOSE_LIMIT_SW_Pin);
	Motor QDMotor(QD_OPEN_GPIO_Port, QD_OPEN_Pin,	QD_CLOSE_GPIO_Port, QD_CLOSE_Pin, &htim3, TIM_CHANNEL_3); //bez krańcówek
	Igniter igniter(FIRE_GPIO_Port, FIRE_Pin, IGNITER_CONNECTION_TEST_GPIO_Port, IGNITER_CONNECTION_TEST_Pin);
	Voltmeter VM(&hadc1, 1);
	HX711 RocketWeight(HX1_SDA_GPIO_Port, HX1_SDA_Pin, HX1_SCL_GPIO_Port, HX1_SCL_Pin);
	HX711 TankWeight(HX2_SDA_GPIO_Port, HX2_SDA_Pin, HX2_SCL_GPIO_Port, HX2_SCL_Pin);

	RocketWeight.initialCalibration(200);
	while(1){
		buf = RocketWeight.ReadValue();
		//buf2 = RocketWeight.AverageValue(5);
		buf3 = VM.GetBatteryVoltageInMilivolts();

		sprintf(dataOut, "DDAT;%i;%i;%i;%i;%i;%li;%li\n", currState, VM.GetBatteryVoltageInMilivolts(), igniter.isConnected(), FillMotor.getStatus(), DeprMotor.getStatus() , buf, buf2);
		//xbee_transmit_char(communication, dataOut);

		HAL_UART_Transmit(&huart3, (uint8_t*)dataOut, strlen(dataOut), 500);
		HAL_Delay(50);

		switch (currState){
			case Init: //test state		//1:INIT
				//place for random tests	//

				if (igniter.isConnected()){
					HAL_GPIO_TogglePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin);
				}
				// (end) place for random test //

				//currState = Idle;
				HAL_Delay(100);
				break;
			case Idle: {	//2
				HAL_Delay(500);
				break;
			}
			case Fueling: {	//3
				HAL_Delay(250);
				break;
			}
			case Countdown: {	//4
				HAL_Delay(500);
				break;
			}
			case Flight: {	//5:Flight aka FIRED
				HAL_Delay(5000);
				break;
			}
			case Abort: {	//6:ABORT
				HAL_Delay(2000);
				break;
			}
		}
	}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART2) {
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		HAL_UART_DMAStop(&huart2);

		xbee_receive(); //odebranie całej wiadomości
		if (xbee_rx.data_flag) { //jeżeli wiadomość była danymi to ta zmienna będzie miała wartość 1
			/*
			 TUTAJ WEDLE UZNANIA PRZECHWYTUJECIE DANE KTORE PRZYSZ�?Y
			 macie do dyspozycji tablice 'xbee_rx.data_array' o wielkości 'DATA_ARRAY' - 30, w której są wartości
			 jeżeli chcecie zatrzymać te dane musicie skopiować wartości tej tabilicy
			 pobranie adresu jest złym pomysłem bo przy każdym odebraniu tablica zmienia swoją zawartosć
			 */
			if(strncmp(xbee_rx.data_array, "TNWN", 4) == 0){
				std::string comand(xbee_rx.data_array);
				comand = comand.substr(5, std::string::npos); //cut WNWN;
				if (comand.substr(0, 4) == "STAT"){
					currState = (state) (((int) (comand[7])) - 48);
				}
				else if (comand[0] == 'D') {
					//R4.comandHandler(comand);
				}
			}
		}
		//tutaj zmienić tylko huart
		HAL_UART_Receive_DMA(&huart2, (uint8_t*) xbee_rx.mess_loaded, DATA_LENGTH);
	}
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
	__disable_irq();
	while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
