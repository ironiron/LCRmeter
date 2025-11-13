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
#include "LCRmath.hpp"


//FIXME add string for GUI for handling frequency change in the settings!

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

static  inline const std::unordered_map<sine_samples_t,const uint16_t*> sine_option = {
        { sine_samples_t::SINE_5000_SAMPELS, sine_table_5000samples_12bit.data() },
        { sine_samples_t::SINE_400_SAMPELS, sine_table_400s_12bit.data() },
        { sine_samples_t::SINE_200_SAMPELS, sine_table_200samples_12bit.data()  },
        { sine_samples_t::SINE_80_SAMPELS, sine_table_80samples_12bit.data()  },
        { sine_samples_t::SINE_40_SAMPELS, sine_table_40samples_12bit.data()  }
};

static inline const std::unordered_map<sine_samples_t,const int> sine_option_length = {
        { sine_samples_t::SINE_5000_SAMPELS, sine_table_5000samples_12bit.size() },
        { sine_samples_t::SINE_400_SAMPELS, sine_table_400s_12bit.size() },
        { sine_samples_t::SINE_200_SAMPELS, sine_table_200samples_12bit.size()  },
        { sine_samples_t::SINE_80_SAMPELS, sine_table_80samples_12bit.size()  },
        { sine_samples_t::SINE_40_SAMPELS, sine_table_40samples_12bit.size()  }
};

static inline const std::unordered_map<Rseries_t, float> r_series_option = {
        {Rseries_t::R_100, 100 },
        {Rseries_t::R_820, 820 },
        {Rseries_t::R_6, 6 }
};

static sine_samples_t current_sine = sine_samples_t::SINE_400_SAMPELS;
static Rseries_t current_rseries = Rseries_t::R_100;

static uint8_t edge_level=50;
static Adc::SamplingTimeClocks period = Adc::ADCCLK_92CYCLES5;

void Set_DAC_Frequency(sine_samples_t freq)
{
    htim6.Instance->CR1 &=~ TIM_CR1_CEN;//stop trigger

    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);// close previous configuration
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)sine_option.at(freq), sine_option_length.at(freq), DAC_ALIGN_12B_R);

    htim6.Instance->CR1|=TIM_CR1_CEN;
}

void Set_Rseries(Rseries_t R)
{
    current_rseries = R;
    LCR_math::series_resistance = static_cast<double>(R);
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



sine_samples_t Increase_DAC_frequncy(void)
{
    switch(current_sine)
    {
        case sine_samples_t::SINE_5000_SAMPELS:
            current_sine = sine_samples_t::SINE_400_SAMPELS;
            break;
        case sine_samples_t::SINE_400_SAMPELS:
            current_sine = sine_samples_t::SINE_200_SAMPELS;
            break;
        case sine_samples_t::SINE_200_SAMPELS:
            current_sine = sine_samples_t::SINE_80_SAMPELS;
            break;
        case sine_samples_t::SINE_80_SAMPELS:
            current_sine = sine_samples_t::SINE_40_SAMPELS;
            break;
        case sine_samples_t::SINE_40_SAMPELS:
        default:
            current_sine = sine_samples_t::SINE_40_SAMPELS;
            break;
    }
    Set_DAC_Frequency(current_sine);
    return current_sine;
}

sine_samples_t Decrease_DAC_frequncy(void)
{
    switch(current_sine)
    {
        case sine_samples_t::SINE_40_SAMPELS:
            current_sine = sine_samples_t::SINE_80_SAMPELS;
            break;
        case sine_samples_t::SINE_80_SAMPELS:
            current_sine = sine_samples_t::SINE_200_SAMPELS;
            break;
        case sine_samples_t::SINE_200_SAMPELS:
            current_sine = sine_samples_t::SINE_400_SAMPELS;
            break;
        case sine_samples_t::SINE_400_SAMPELS:
            current_sine = sine_samples_t::SINE_5000_SAMPELS;
            break;
        case sine_samples_t::SINE_5000_SAMPELS:
        default:
            current_sine = sine_samples_t::SINE_5000_SAMPELS;
            break;
    }
    Set_DAC_Frequency(current_sine);
    return current_sine;
}

sine_samples_t Get_DAC_Frequency(void)
{
    return current_sine;
}


Rseries_t Increase_Rseries(void)
{
    switch(current_rseries)
    {
        case Rseries_t::R_6:
            current_rseries = Rseries_t::R_100;
            break;
        case Rseries_t::R_100:
            current_rseries = Rseries_t::R_820;
            break;
        case Rseries_t::R_820:
        default:
            current_rseries = Rseries_t::R_820;
            break;
    }
    Set_Rseries(current_rseries);
    return current_rseries;
}

Rseries_t Decrease_Rseries(void)
{
    switch(current_rseries)
    {
        case Rseries_t::R_820:
            current_rseries = Rseries_t::R_100;
            break;
        case Rseries_t::R_100:
            current_rseries = Rseries_t::R_6;
            break;
        case Rseries_t::R_6:
        default:
            current_rseries = Rseries_t::R_6;
            break;
    }
    Set_Rseries(current_rseries);
    return current_rseries;
}

Rseries_t Get_Rseries(void)
{
    return current_rseries;
}

uint8_t Decrease_Edge_Level(void)
{
    if(edge_level>=10)
        edge_level -= 10;
    return edge_level;
}

uint8_t Increase_Edge_Level(void)
{
    if(edge_level<=90)
        edge_level += 10;
    return edge_level;
}

uint8_t Get_Edge_Level(void)
{
    return edge_level;
}

Adc::SamplingTimeClocks Decrease_period(void)
{
    switch(period)
    {
        case Adc::ADCCLK_640CYCLES5:
            period = Adc::ADCCLK_247CYCLES5;
            break;
        case Adc::ADCCLK_247CYCLES5:
            period = Adc::ADCCLK_92CYCLES5;
            break;
        case Adc::ADCCLK_92CYCLES5:
            period = Adc::ADCCLK_47CYCLES5;
            break;
        case Adc::ADCCLK_47CYCLES5:
            period = Adc::ADCCLK_24CYCLES5;
            break;
        case Adc::ADCCLK_24CYCLES5:
            period = Adc::ADCCLK_12CYCLES5;
            break;
        case Adc::ADCCLK_12CYCLES5:
            period = Adc::ADCCLK_6CYCLES5;
            break;
        case Adc::ADCCLK_6CYCLES5:
            period = Adc::ADCCLK_2CYCLES5;
            break;
        case Adc::ADCCLK_2CYCLES5:
            period = Adc::ADCCLK_2CYCLES5;
            break;
        default:
            period = Adc::ADCCLK_2CYCLES5;
            break;
    }
    Adc::Set_Sampling_time(period);
    return period;
}

Adc::SamplingTimeClocks Increase_period(void)
{
    switch(period)
    {
        case Adc::ADCCLK_2CYCLES5:
            period = Adc::ADCCLK_6CYCLES5;
            break;
        case Adc::ADCCLK_6CYCLES5:
            period = Adc::ADCCLK_12CYCLES5;
            break;
        case Adc::ADCCLK_12CYCLES5:
            period = Adc::ADCCLK_24CYCLES5;
            break;
        case Adc::ADCCLK_24CYCLES5:
            period = Adc::ADCCLK_47CYCLES5;
            break;
        case Adc::ADCCLK_47CYCLES5:
            period = Adc::ADCCLK_92CYCLES5;
            break;
        case Adc::ADCCLK_92CYCLES5:
            period = Adc::ADCCLK_247CYCLES5;
            break;
        case Adc::ADCCLK_247CYCLES5:
            period = Adc::ADCCLK_640CYCLES5;
            break;
        case Adc::ADCCLK_640CYCLES5:
            period = Adc::ADCCLK_640CYCLES5;
            break;
        default:
            period = Adc::ADCCLK_640CYCLES5;
            break;
    }
    Adc::Set_Sampling_time(period);
    return period;
}

float Get_period(void)
{
    return Adc::Get_sample_Time(period);
}

