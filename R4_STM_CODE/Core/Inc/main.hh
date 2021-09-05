/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUILD_IN_LED_Pin GPIO_PIN_13
#define BUILD_IN_LED_GPIO_Port GPIOC
#define IGNITER_CONNECTION_TEST_Pin GPIO_PIN_15
#define IGNITER_CONNECTION_TEST_GPIO_Port GPIOC
#define FIRE_Pin GPIO_PIN_0
#define FIRE_GPIO_Port GPIOA
#define VOLTAGE_MEASURE_Pin GPIO_PIN_1
#define VOLTAGE_MEASURE_GPIO_Port GPIOA
#define HX2_SDA_Pin GPIO_PIN_4
#define HX2_SDA_GPIO_Port GPIOA
#define HX2_SCL_Pin GPIO_PIN_5
#define HX2_SCL_GPIO_Port GPIOA
#define PQD_OPEN_Pin GPIO_PIN_6
#define PQD_OPEN_GPIO_Port GPIOA
#define PQD_CLOSE_Pin GPIO_PIN_7
#define PQD_CLOSE_GPIO_Port GPIOA
#define QD_EN_Pin GPIO_PIN_0
#define QD_EN_GPIO_Port GPIOB
#define PQD_EN_Pin GPIO_PIN_1
#define PQD_EN_GPIO_Port GPIOB
#define HX1_SDA_Pin GPIO_PIN_12
#define HX1_SDA_GPIO_Port GPIOB
#define HX1_SCL_Pin GPIO_PIN_13
#define HX1_SCL_GPIO_Port GPIOB
#define QD_CLOSE_Pin GPIO_PIN_14
#define QD_CLOSE_GPIO_Port GPIOB
#define QD_OPEN_Pin GPIO_PIN_15
#define QD_OPEN_GPIO_Port GPIOB
#define QD_CLOSE_LIMIT_SW_Pin GPIO_PIN_8
#define QD_CLOSE_LIMIT_SW_GPIO_Port GPIOA
#define QD_OPEN_LIMIT_SW_Pin GPIO_PIN_9
#define QD_OPEN_LIMIT_SW_GPIO_Port GPIOA
#define DEPR_CLOSE_LIMIT_SW_Pin GPIO_PIN_11
#define DEPR_CLOSE_LIMIT_SW_GPIO_Port GPIOA
#define DEPR_OPEN_LIMIT_SW_Pin GPIO_PIN_12
#define DEPR_OPEN_LIMIT_SW_GPIO_Port GPIOA
#define FILL_CLOSE_LIMIT_SW_Pin GPIO_PIN_15
#define FILL_CLOSE_LIMIT_SW_GPIO_Port GPIOA
#define FILL_OPEN_LIMIT_SW_Pin GPIO_PIN_3
#define FILL_OPEN_LIMIT_SW_GPIO_Port GPIOB
#define DEPR_CLOSE_Pin GPIO_PIN_4
#define DEPR_CLOSE_GPIO_Port GPIOB
#define DEPR_EN_Pin GPIO_PIN_5
#define DEPR_EN_GPIO_Port GPIOB
#define DEPR_OPEN_Pin GPIO_PIN_6
#define DEPR_OPEN_GPIO_Port GPIOB
#define FILL_CLOSE_Pin GPIO_PIN_7
#define FILL_CLOSE_GPIO_Port GPIOB
#define FILL_EN_Pin GPIO_PIN_8
#define FILL_EN_GPIO_Port GPIOB
#define FILL_OPEN_Pin GPIO_PIN_9
#define FILL_OPEN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
