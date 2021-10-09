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
#include <adc.h>
#include <dma.h>
#include <tim.h>
#include <usart.h>
#include <gpio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//STL:
#include <string>
#include <charconv>
//Projektowe:
#include "xbee.h"
#include "Igniter.hh"
#include "hx711.hh"
#include "L298.hh"
#include "Bluetooth.hh"
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
volatile int32_t buf1 = -1, buf2 = -1, buf3 = -1;
char dataIn[100];
char dataOut[100];
uint8_t BTbuf[20];
Xbee communication;

 std::shared_ptr<Rocket> R4;

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
int main(void) {
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
	xbee_init(&communication, 0x0013A20041C283D6, &huart2); //inicjalizacja modułu xbee

	///ADDED FOR BLUETOOTH///{
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
	HAL_UART_Receive_IT(&huart3, BTbuf, 15);
	// HAL_GPIO_WritePin(Bluetooth_reset_GPIO_Port, Bluetooth_reset_Pin, SET);//ADDITIONAL PIN PC14 FOR RESET //
	HAL_UART_Transmit(&huart3, (uint8_t*) "INIT\n", strlen("INIT\n"), 500);
	//}
	/* USER CODE END 2 */

	/* USER CODE BEGIN WHILE */

	//wyodrebnic do init function
	std::shared_ptr<Motor> FillMotor = std::make_shared<Motor>(
			FILL_OPEN_GPIO_Port, FILL_OPEN_Pin, FILL_CLOSE_GPIO_Port,
			FILL_CLOSE_Pin, &htim4, TIM_CHANNEL_3,
			FILL_OPEN_LIMIT_SW_GPIO_Port, FILL_OPEN_LIMIT_SW_Pin,
			FILL_CLOSE_LIMIT_SW_GPIO_Port, FILL_CLOSE_LIMIT_SW_Pin); //tankujący - dwie krancówki (1 i 2)
	std::shared_ptr<Motor> DepressurizationMotor = std::make_shared<Motor>(
			DEPR_OPEN_GPIO_Port, DEPR_OPEN_Pin, DEPR_CLOSE_GPIO_Port,
			DEPR_CLOSE_Pin, &htim3, TIM_CHANNEL_2,
			DEPR_OPEN_LIMIT_SW_GPIO_Port, DEPR_OPEN_LIMIT_SW_Pin,
			DEPR_CLOSE_LIMIT_SW_GPIO_Port, DEPR_CLOSE_LIMIT_SW_Pin);//odpowietrzajacy - dwie krancówki (3 i 4));
	std::shared_ptr<Motor> QDMotor = std::make_shared<Motor>(
			QD_OPEN_GPIO_Port, QD_OPEN_Pin, QD_CLOSE_GPIO_Port,
			QD_CLOSE_Pin, &htim3, TIM_CHANNEL_3,
			QD_OPEN_LIMIT_SW_GPIO_Port, QD_OPEN_LIMIT_SW_Pin,
			QD_CLOSE_LIMIT_SW_GPIO_Port, QD_CLOSE_LIMIT_SW_Pin); //upustowy) - dwie krancowki (5 i 6));
	std::shared_ptr<Motor> PlusQDMotor = std::make_shared<Motor>(
			PQD_OPEN_GPIO_Port, PQD_OPEN_Pin, PQD_CLOSE_GPIO_Port,
			PQD_CLOSE_Pin, &htim3, TIM_CHANNEL_4);
	Igniter igniter(FIRE_GPIO_Port, FIRE_Pin, IGNITER_CONNECTION_TEST_GPIO_Port,
			IGNITER_CONNECTION_TEST_Pin);
	std::shared_ptr<HX711> RocketWeight = std::make_shared<HX711>(
			HX1_SDA_GPIO_Port, HX1_SDA_Pin, HX1_SCL_GPIO_Port, HX1_SCL_Pin,
			-21663, 27.215); //hardcoded
	std::shared_ptr<HX711> TankWeight = std::make_shared<HX711>(
			HX2_SDA_GPIO_Port, HX2_SDA_Pin, HX2_SCL_GPIO_Port, HX2_SCL_Pin,
			-2414370, 37.11); //hardcoded
	Voltmeter Voltmeter(&hadc1, 1);

	R4 = std::make_shared<Rocket>(FillMotor, DepressurizationMotor, QDMotor, std::make_shared<Igniter>(igniter),
									RocketWeight, TankWeight, PlusQDMotor);

	R4->comandHandler(std::string("DWRT"));
	//R4->comandHandler(std::string("DWRC;9000"));

	while (1) {
		HAL_GPIO_TogglePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin);

		sprintf(dataOut, "R4TN;%.1f;%s\n", Voltmeter.GetBatteryVoltageInVolts(), R4->getInfo().c_str());
		xbee_transmit_char(communication, dataOut);
    
		HAL_UART_Transmit(&huart3, (uint8_t*)dataOut, (uint16_t)strlen(dataOut), 500);//BT
		HAL_Delay(50);

		switch (R4->getCurrState()) {
			case RocketStateInit: 		//0
				R4->setCurrState(RocketStateIdle);
				HAL_Delay(500);
				break;
			case RocketStateIdle:{		//1
				HAL_Delay(500);
				break;
			}
			case RocketStateFueling:{	//2
				HAL_Delay(250);
				break;
			}
			case RocketStateCountdown:{ //3
				HAL_Delay(1000);
				break;
			}
			case RocketStateFlight:{	//5:Flight aka FIRED
				HAL_Delay(15000);
				break;
			}
			case RocketStateAbort:{	//4:ABORT
				HAL_Delay(1000);
				break;
			}
			default:{
				R4->setCurrState(RocketStateIdle);
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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
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
			if (strncmp(xbee_rx.data_array, "TNWN", 4) == 0) {
				std::string_view comand(xbee_rx.data_array);
				comand = comand.substr(5, std::string::npos); //cut TNWN;

				if (comand[0] == 'D' || comand[0] == 'S') {
					R4->comandHandler(comand);
				}
			}
		}
		HAL_UART_Receive_DMA(&huart2, (uint8_t*) xbee_rx.mess_loaded,
				DATA_LENGTH);
	}
	else if (huart->Instance == USART3) {
		std::string_view command((char*) BTbuf);	//TODO: test (changed to string_view)
		if (command[0] == 'D' || command[0] == 'S') {
			R4->comandHandler(command);
		}
		HAL_UART_Transmit(&huart3, (uint8_t*) "RECEIVED\n", strlen("RECEIVED\n"), 500);
		memset(BTbuf, 0, sizeof(BTbuf));
		HAL_UART_Receive_IT(&huart3, BTbuf, 15);
	}
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
