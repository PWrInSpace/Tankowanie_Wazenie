/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M5CloseLimitSwitchEXT_Pin GPIO_PIN_13
#define M5CloseLimitSwitchEXT_GPIO_Port GPIOC
#define M5CloseLimitSwitchEXT_EXTI_IRQn EXTI15_10_IRQn
#define M5OpenLimitSwitchEXT_Pin GPIO_PIN_14
#define M5OpenLimitSwitchEXT_GPIO_Port GPIOC
#define M5OpenLimitSwitchEXT_EXTI_IRQn EXTI15_10_IRQn
#define M4CloseLimitSwitch_Pin GPIO_PIN_15
#define M4CloseLimitSwitch_GPIO_Port GPIOC
#define M4OpenLimitSwitchEXT_Pin GPIO_PIN_0
#define M4OpenLimitSwitchEXT_GPIO_Port GPIOA
#define M1Analog_Pin GPIO_PIN_1
#define M1Analog_GPIO_Port GPIOA
#define M2Analog_Pin GPIO_PIN_2
#define M2Analog_GPIO_Port GPIOA
#define M3Analog_Pin GPIO_PIN_3
#define M3Analog_GPIO_Port GPIOA
#define BatteryVoltage_Pin GPIO_PIN_4
#define BatteryVoltage_GPIO_Port GPIOA
#define Pressure1Measure_Pin GPIO_PIN_5
#define Pressure1Measure_GPIO_Port GPIOA
#define Pressure1MeasureA6_Pin GPIO_PIN_6
#define Pressure1MeasureA6_GPIO_Port GPIOA
#define M5Analog_Pin GPIO_PIN_7
#define M5Analog_GPIO_Port GPIOA
#define M5PWM_Pin GPIO_PIN_0
#define M5PWM_GPIO_Port GPIOB
#define M4Analog_Pin GPIO_PIN_1
#define M4Analog_GPIO_Port GPIOB
#define M5Dir_Pin GPIO_PIN_12
#define M5Dir_GPIO_Port GPIOB
#define MotorFault_Pin GPIO_PIN_13
#define MotorFault_GPIO_Port GPIOB
#define MotorEnable_Pin GPIO_PIN_14
#define MotorEnable_GPIO_Port GPIOB
#define M4Dir_Pin GPIO_PIN_15
#define M4Dir_GPIO_Port GPIOB
#define M4PWM_Pin GPIO_PIN_8
#define M4PWM_GPIO_Port GPIOA
#define M3PWM_Pin GPIO_PIN_9
#define M3PWM_GPIO_Port GPIOA
#define M3CloseLimitSwitchEXT_Pin GPIO_PIN_10
#define M3CloseLimitSwitchEXT_GPIO_Port GPIOA
#define M3CloseLimitSwitchEXT_EXTI_IRQn EXTI15_10_IRQn
#define M3OpenLimitSwitchEXT_Pin GPIO_PIN_11
#define M3OpenLimitSwitchEXT_GPIO_Port GPIOA
#define M3OpenLimitSwitchEXT_EXTI_IRQn EXTI15_10_IRQn
#define M3Dir_Pin GPIO_PIN_12
#define M3Dir_GPIO_Port GPIOA
#define M2CloseLimitSwitchEXT_Pin GPIO_PIN_15
#define M2CloseLimitSwitchEXT_GPIO_Port GPIOA
#define M2CloseLimitSwitchEXT_EXTI_IRQn EXTI15_10_IRQn
#define M2OpenLimitSwitchEXT_Pin GPIO_PIN_3
#define M2OpenLimitSwitchEXT_GPIO_Port GPIOB
#define M2OpenLimitSwitchEXT_EXTI_IRQn EXTI3_IRQn
#define M2Dir_Pin GPIO_PIN_4
#define M2Dir_GPIO_Port GPIOB
#define M1OpenLimitSwitchEXT_Pin GPIO_PIN_5
#define M1OpenLimitSwitchEXT_GPIO_Port GPIOB
#define M1OpenLimitSwitchEXT_EXTI_IRQn EXTI9_5_IRQn
#define M1CloseLimitSwitchEXT_Pin GPIO_PIN_6
#define M1CloseLimitSwitchEXT_GPIO_Port GPIOB
#define M1CloseLimitSwitchEXT_EXTI_IRQn EXTI9_5_IRQn
#define M1Dir_Pin GPIO_PIN_7
#define M1Dir_GPIO_Port GPIOB
#define M2PWM_Pin GPIO_PIN_8
#define M2PWM_GPIO_Port GPIOB
#define M1PWM_Pin GPIO_PIN_9
#define M1PWM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
