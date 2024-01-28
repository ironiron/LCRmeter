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
#include "stm32g4xx_hal.h"

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
#define ADC_2_Pin GPIO_PIN_1
#define ADC_2_GPIO_Port GPIOA
#define ADC_1_Pin GPIO_PIN_2
#define ADC_1_GPIO_Port GPIOA
#define OSCILLOSCOPE_IN_Pin GPIO_PIN_0
#define OSCILLOSCOPE_IN_GPIO_Port GPIOB
#define BUTTON_DOWN_Pin GPIO_PIN_1
#define BUTTON_DOWN_GPIO_Port GPIOB
#define BUTTON_DOWN_EXTI_IRQn EXTI1_IRQn
#define BUTTON_UP_Pin GPIO_PIN_2
#define BUTTON_UP_GPIO_Port GPIOB
#define BUTTON_UP_EXTI_IRQn EXTI2_IRQn
#define V_USB_IN_Pin GPIO_PIN_11
#define V_USB_IN_GPIO_Port GPIOB
#define V_USB_IN_EXTI_IRQn EXTI15_10_IRQn
#define ADC_2b_Pin GPIO_PIN_12
#define ADC_2b_GPIO_Port GPIOB
#define ADC_1b_Pin GPIO_PIN_13
#define ADC_1b_GPIO_Port GPIOB
#define EN_PATH_1_Pin GPIO_PIN_14
#define EN_PATH_1_GPIO_Port GPIOB
#define RES_CALIBRATION_Pin GPIO_PIN_15
#define RES_CALIBRATION_GPIO_Port GPIOB
#define VDD_DIV_2_Pin GPIO_PIN_8
#define VDD_DIV_2_GPIO_Port GPIOA
#define USB_EN_Pin GPIO_PIN_9
#define USB_EN_GPIO_Port GPIOA
#define PWM_OUT_Pin GPIO_PIN_10
#define PWM_OUT_GPIO_Port GPIOA
#define BUTTON_OK_Pin GPIO_PIN_4
#define BUTTON_OK_GPIO_Port GPIOB
#define BUTTON_OK_EXTI_IRQn EXTI4_IRQn
#define BUTTON_BACK_Pin GPIO_PIN_5
#define BUTTON_BACK_GPIO_Port GPIOB
#define BUTTON_BACK_EXTI_IRQn EXTI9_5_IRQn
#define EN_PATH_2_Pin GPIO_PIN_6
#define EN_PATH_2_GPIO_Port GPIOB
#define EN_PATH_3_Pin GPIO_PIN_9
#define EN_PATH_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define BUTTON_OKA4_Pin GPIO_PIN_4
#define BUTTON_OKA4_GPIO_Port GPIOA
#define BUTTON_OKA4_EXTI_IRQn EXTI4_IRQn


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
