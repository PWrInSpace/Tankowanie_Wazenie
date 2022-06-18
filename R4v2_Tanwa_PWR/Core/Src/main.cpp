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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <MAX14870.hh>
#include<vector>
#include<tuple>
#include<queue.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct TxStruct{
    bool tick;
    uint8_t lastDoneCommandNum;
    uint8_t MotorState[5];
    //int16_t adcValue[8];
};

struct RxStruct{
    uint8_t CommandNum;
    uint16_t CommandArgument;
};

TxStruct txStruct = {0,0,{0,0,0,0,0}};
RxStruct rxStruct = {0,0};
const uint16_t rxBufferLenght = 10;
auto rxQueue = xQueueCreate(rxBufferLenght, sizeof(rxStruct));
uint16_t ventingTime = 500; //tmp
std::vector<std::tuple<Motor*,volatile ValveState>> MotorList;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c2;
DMA_HandleTypeDef hdma_i2c2_tx;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* Definitions for COM */
osThreadId_t COMHandle;
const osThreadAttr_t COM_attributes = {
  .name = "COM",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Valves */
osThreadId_t ValvesHandle;
const osThreadAttr_t Valves_attributes = {
  .name = "Valves",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Measure */
osThreadId_t MeasureHandle;
const osThreadAttr_t Measure_attributes = {
  .name = "Measure",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM4_Init(void);
void TaskCOM(void *argument);
void TaskValves(void *argument);
void TaskMeasure(void *argument);

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(50);
	MotorList.push_back({new Motor(M1Dir_GPIO_Port, M1Dir_Pin, &htim4, TIM_CHANNEL_4), ValveStateIDK});
	MotorList.push_back({new Motor(M2Dir_GPIO_Port, M2Dir_Pin, &htim4, TIM_CHANNEL_3), ValveStateIDK});
	MotorList.push_back({new Motor(M3Dir_GPIO_Port, M3Dir_Pin, &htim1, TIM_CHANNEL_2), ValveStateIDK});
	MotorList.push_back({new Motor(M4Dir_GPIO_Port, M4Dir_Pin, &htim1, TIM_CHANNEL_1), ValveStateIDK});
	MotorList.push_back({new Motor(M5Dir_GPIO_Port, M5Dir_Pin, &htim3, TIM_CHANNEL_3), ValveStateIDK});
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of COM */
  COMHandle = osThreadNew(TaskCOM, NULL, &COM_attributes);

  /* creation of Valves */
  ValvesHandle = osThreadNew(TaskValves, NULL, &Valves_attributes);

  /* creation of Measure */
  MeasureHandle = osThreadNew(TaskMeasure, NULL, &Measure_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (true){
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

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 8;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 52;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 47;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 47;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 47;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, M5Dir_Pin|MotorEnable_Pin|M4Dir_Pin|M2Dir_Pin
                          |M1Dir_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(M3Dir_GPIO_Port, M3Dir_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : M5CloseLimitSwitchEXT_Pin M5OpenLimitSwitchEXT_Pin */
  GPIO_InitStruct.Pin = M5CloseLimitSwitchEXT_Pin|M5OpenLimitSwitchEXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : M4CloseLimitSwitch_Pin */
  GPIO_InitStruct.Pin = M4CloseLimitSwitch_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(M4CloseLimitSwitch_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : M4OpenLimitSwitchEXT_Pin M3CloseLimitSwitchEXT_Pin M3OpenLimitSwitchEXT_Pin M2CloseLimitSwitchEXT_Pin */
  GPIO_InitStruct.Pin = M4OpenLimitSwitchEXT_Pin|M3CloseLimitSwitchEXT_Pin|M3OpenLimitSwitchEXT_Pin|M2CloseLimitSwitchEXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : M5Dir_Pin MotorEnable_Pin M4Dir_Pin M2Dir_Pin
                           M1Dir_Pin */
  GPIO_InitStruct.Pin = M5Dir_Pin|MotorEnable_Pin|M4Dir_Pin|M2Dir_Pin
                          |M1Dir_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MotorFault_Pin */
  GPIO_InitStruct.Pin = MotorFault_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MotorFault_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : M3Dir_Pin */
  GPIO_InitStruct.Pin = M3Dir_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(M3Dir_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : M2OpenLimitSwitchEXT_Pin M1OpenLimitSwitchEXT_Pin M1CloseLimitSwitchEXT_Pin */
  GPIO_InitStruct.Pin = M2OpenLimitSwitchEXT_Pin|M1OpenLimitSwitchEXT_Pin|M1CloseLimitSwitchEXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void TryReachExpectedState(std::tuple<Motor*,volatile ValveState>& Valve, uint16_t parameter = 500){
	Motor* motor = std::get<0>(Valve);
	ValveState expectedState = std::get<1>(Valve);
	if(motor->GetState() == expectedState){
		motor->Stop();
		return;
	}
	else if(expectedState == ValveStateOpen){
		motor->State = ValveStateAttemptToOpen;
		motor->Open();
	}
	else if(expectedState == ValveStateClose){
		motor->State = ValveStateAttemptToClose;
		motor->Close();
	}
	else if(expectedState == ValveStateVent){
		motor->State = ValveStateAttemptToOpen;
		motor->Open();
		osDelay(parameter);
		motor->State = ValveStateAttemptToClose;
		motor->Close();
	}
}

void setNewExpectedStateOfValveOnVector(std::vector<std::tuple<Motor*,volatile ValveState>>& Valves, const uint8_t& ValveNumber, const ValveState& newExpectedValveState){
	std::get<0>(Valves[ValveNumber])->SetState(ValveStateIDK);
	Valves[ValveNumber] = std::tuple<Motor*,volatile ValveState>(std::get<0>(Valves[ValveNumber]), newExpectedValveState);
}

void handleRxStruct(RxStruct rxStruct){
	txStruct.lastDoneCommandNum = rxStruct.CommandNum;
	uint8_t CommandNumValve = (rxStruct.CommandNum / 10) % 10;
	uint8_t CommandNumState = rxStruct.CommandNum % 10;
	ventingTime = rxStruct.CommandArgument; //new venting time
	if(rxStruct.CommandNum == 99){
			HAL_NVIC_SystemReset();
	}
	else if(CommandNumValve > 0 && CommandNumValve < 6){ //Motor1 - Motor5
		if(CommandNumState == 0 || CommandNumState == 1 || CommandNumState == 3){
			setNewExpectedStateOfValveOnVector(MotorList, CommandNumValve - 1, (ValveState)CommandNumState);
		}
	}

}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	HAL_I2C_DisableListen_IT(hi2c);
	if(!TransferDirection) {
		HAL_I2C_Slave_Transmit(hi2c, (uint8_t*)&txStruct, sizeof(TxStruct), 5);
	}
	else{
		HAL_I2C_Slave_Receive(hi2c, (uint8_t*)&rxStruct, sizeof(RxStruct), 50);
		xQueueSendFromISR(rxQueue, &rxStruct, NULL);
	}
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){ //ToDo change to loop
	HAL_Delay(50);
	//ToDO add real de-bounce // https://www.instructables.com/STM32CubeMX-Button-Debounce-With-Interrupt/
	if(std::get<1>(MotorList[0]) == ValveStateOpen && GPIO_Pin == M1OpenLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOB, M1OpenLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[0])->SetState(ValveStateOpen);
	}
	else if(std::get<1>(MotorList[0]) == ValveStateClose && GPIO_Pin == M1CloseLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOB, M1CloseLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[0])->SetState(ValveStateClose);
	}
	else if(std::get<1>(MotorList[1]) == ValveStateOpen && GPIO_Pin == M2OpenLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOB, M2OpenLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[1])->SetState(ValveStateOpen);
	}
	else if(std::get<1>(MotorList[1]) == ValveStateClose && GPIO_Pin == M2CloseLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOA, M2CloseLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[1])->SetState(ValveStateClose);
	}
	else if(std::get<1>(MotorList[2]) == ValveStateOpen && GPIO_Pin == M3OpenLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOA, M3OpenLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[2])->SetState(ValveStateOpen);
	}
	else if(std::get<1>(MotorList[2]) == ValveStateClose && GPIO_Pin == M3CloseLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOA, M3CloseLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[2])->SetState(ValveStateClose);
	}
	else if(std::get<1>(MotorList[3]) == ValveStateOpen && GPIO_Pin == M4OpenLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOC, M4OpenLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[3])->SetState(ValveStateOpen);
	}
	else if(std::get<1>(MotorList[4]) == ValveStateOpen && GPIO_Pin == M5OpenLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOC, M5OpenLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[4])->SetState(ValveStateOpen);
	}
	else if(std::get<1>(MotorList[4]) == ValveStateClose && GPIO_Pin == M5CloseLimitSwitchEXT_Pin){
		if (HAL_GPIO_ReadPin(GPIOC, M5CloseLimitSwitchEXT_Pin) == 0)
			std::get<0>(MotorList[4])->SetState(ValveStateClose);
	}
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_TaskCOM */
/**
  * @brief  Function implementing the COM thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_TaskCOM */
void TaskCOM(void *argument)
{
  /* USER CODE BEGIN 5 */
	RxStruct currentCommand = {0,0};
	HAL_I2C_EnableListen_IT(&hi2c2); // Start listening for I2C master call.

	{//test OPEN - CLOSE
	//	rxStruct = {11,0}; //open M1
	//	xQueueSend(rxQueue, &rxStruct, 1000);
	//	rxStruct = {10,0}; //close M1
	//	xQueueSend(rxQueue, &rxStruct, 1000);
	}
	/* Infinite loop */
	for(;;){
		if(xQueueReceive(rxQueue, &currentCommand, 100) == pdPASS){
			handleRxStruct(currentCommand);
		}
		osDelay(100);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_TaskValves */
/**
* @brief Function implementing the Valves thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TaskValves */
void TaskValves(void *argument)
{
  /* USER CODE BEGIN TaskValves */
	/* Infinite loop */
	while(true){
		for(auto motor : MotorList){
			TryReachExpectedState(motor, *(static_cast<uint16_t*>(argument)));
		}
		osDelay(50);
	}
  /* USER CODE END TaskValves */
}

/* USER CODE BEGIN Header_TaskMeasure */

/**
* @brief Function implementing the Measure thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TaskMeasure */
void TaskMeasure(void *argument)
{
  /* USER CODE BEGIN TaskMeasure */
	volatile uint16_t ADCData[8] = {1,1,1,1,1,1,1,1};
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCData, 8);
	float ADCDividerRatio[8] = {1.0 * (3.3 / 4096), 1.0 * (3.3 / 4096), 1.0 * (3.3 / 4096), 1.0 * (3.3 / 4096), 5.7 * (3.3 / 4096), 1.0 * (3.3 / 4096), 1.0 * (3.3 / 4096), 1.0 * (3.3 / 4096)};
	bool tick = false;
	/* Infinite loop */
	while(true){
		if(std::get<1>(MotorList[3]) == ValveStateClose && HAL_GPIO_ReadPin(M4CloseLimitSwitch_GPIO_Port, M4CloseLimitSwitch_Pin) == 0){ //not an EXT pin cant be handled by interupt
			std::get<0>(MotorList[3])->SetState(ValveStateClose);
		}
		tick = !tick;
		txStruct = {
			tick,
			txStruct.lastDoneCommandNum,
			{
				(uint8_t)std::get<0>(MotorList[0])->GetState(),
				(uint8_t)std::get<0>(MotorList[1])->GetState(),
				(uint8_t)std::get<0>(MotorList[2])->GetState(),
				(uint8_t)std::get<0>(MotorList[3])->GetState(),
				(uint8_t)std::get<0>(MotorList[4])->GetState()
			}
			/*,
			{
				ADCData[0]/ADCDividerRatio[0], //M1 Analog
				ADCData[1]/ADCDividerRatio[1], //M2 Analog
				ADCData[2]/ADCDividerRatio[2], //M3 Analog
				ADCData[3]/ADCDividerRatio[3], //M1 Analog
				(int16_t)(ADCData[4]/ADCDividerRatio[4]), //Battery Voltage
				ADCData[5]/ADCDividerRatio[5], //Pressure1
				ADCData[6]/ADCDividerRatio[6], //Pressure2
				ADCData[7]/ADCDividerRatio[7] //M4 Analog
			}*/
		};
		osDelay(500);
	}
  /* USER CODE END TaskMeasure */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
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
	HAL_NVIC_SystemReset();
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

