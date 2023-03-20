/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define BUTTON_OK_Pin GPIO_PIN_2
#define BUTTON_OK_GPIO_Port GPIOA
#define BUTTON_OK_EXTI_IRQn EXTI2_IRQn
#define BUTTON_BACK_Pin GPIO_PIN_3
#define BUTTON_BACK_GPIO_Port GPIOA
#define BUTTON_BACK_EXTI_IRQn EXTI3_IRQn
#define BUTTON_OKA4_Pin GPIO_PIN_4
#define BUTTON_OKA4_GPIO_Port GPIOA
#define BUTTON_OKA4_EXTI_IRQn EXTI4_IRQn
#define BUTTON_UP_Pin GPIO_PIN_5
#define BUTTON_UP_GPIO_Port GPIOA
#define BUTTON_UP_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
