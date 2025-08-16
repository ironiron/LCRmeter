/*
 * LLutils.cpp
 *
 *  Created on: Jun 21, 2025
 *      Author: xDDD
 */

#include <stdio.h>
#include "LLutils.h"
#include "stm32g4xx_hal.h"
#include "sine.hpp"
#include "common_defines.hpp"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern ADC_HandleTypeDef hadc5;
extern DMA_HandleTypeDef hdma_adc1;

extern CORDIC_HandleTypeDef hcordic;

extern DAC_HandleTypeDef hdac1;
extern DAC_HandleTypeDef hdac2;
extern DMA_HandleTypeDef hdma_dac1_ch1;

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;

static inline const std::unordered_map<sine_samples_t,const int> sine_option_length = {
        { sine_samples_t::SINE_400_SAMPELS, sine_table_400s_12bit.size() },
        { sine_samples_t::SINE_200_SAMPELS, sine_table_200samples_12bit.size()  },
        { sine_samples_t::SINE_80_SAMPELS, sine_table_80samples_12bit.size()  },
        { sine_samples_t::SINE_40_SAMPELS, sine_table_400s_12bit.size()  }
};

void Set_DAC_Frequency(sine_samples_t freq)
{
    htim6.Instance->CR1 &=~ TIM_CR1_CEN;//stop trigger

    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);// close previous configuration
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)sine_option.at(freq), sine_option_length.at(freq), DAC_ALIGN_12B_R);

    htim6.Instance->CR1|=TIM_CR1_CEN;
}

void Set_Rseries(Rseries_t R)
{
    HAL_GPIO_WritePin(EN_PATH_1_GPIO_Port, EN_PATH_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EN_PATH_2_GPIO_Port, EN_PATH_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EN_PATH_3_GPIO_Port, EN_PATH_3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RES_CALIBRATION_GPIO_Port, RES_CALIBRATION_Pin, GPIO_PIN_RESET);

    switch(R)
    {
        case Rseries_t::R_100:
            HAL_GPIO_WritePin(EN_PATH_1_GPIO_Port, EN_PATH_1_Pin, GPIO_PIN_SET);
            break;
        case Rseries_t::R_820:
            HAL_GPIO_WritePin(EN_PATH_2_GPIO_Port, EN_PATH_2_Pin, GPIO_PIN_SET);
            break;
        case Rseries_t::R_6:
            HAL_GPIO_WritePin(EN_PATH_3_GPIO_Port, EN_PATH_3_Pin, GPIO_PIN_SET);
            break;
    }
}
