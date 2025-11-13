/*
 * LLutils.h
 *
 *  Created on: Jun 21, 2025
 *      Author: xDDD
 */

#ifndef LLUTILS_H_
#define LLUTILS_H_

#include <map>
#include <unordered_map>
#include <stdint.h>
#include "sine.hpp"
#include "adc.hpp"

enum class sine_samples_t : unsigned int
{
    SINE_5000_SAMPELS=0,
    SINE_400_SAMPELS,
    SINE_200_SAMPELS,
    SINE_80_SAMPELS,
    SINE_40_SAMPELS,
};

enum class Rseries_t : unsigned int
{
    R_6 =0, // in ohms
    R_100,
    R_820,
};

/**
 * @brief Sets DAC frequncy at one of the predefined values
 * Will stop and restart DAC transmission internally
 * @note will not reset samples, etc so it might corrupt measurment
 * @param freq to set
 */
void Set_DAC_Frequency(sine_samples_t freq);

/**
 * @brief Sets resistance value by switching GPIO
 * Will update resistance value in \ref LCRmath module as well.
 * @param R resistance to set
 */
void Set_Rseries(Rseries_t R);

/**
 * @brief Increase frequency by one enum value
 * If max is achieved does nothing.
 * @retval current frequency
 */
sine_samples_t Increase_DAC_frequncy(void);

/**
 * @brief Decrease frequency by one enum value
 * If min is achieved does nothing.
 * @retval current frequency
 */
sine_samples_t Decrease_DAC_frequncy(void);

/**
 * @brief Gets current frequency from internal state machine
 * @retval current frequency
 */
sine_samples_t Get_DAC_Frequency(void);

/**
 * @brief Increase R series by one enum value
 * If max is achieved does nothing.
 * @retval current R series
 */
Rseries_t Increase_Rseries(void);

/**
 * @brief Decrease R series by one enum value
 * If min is achieved does nothing.
 * @retval current R series
 */
Rseries_t Decrease_Rseries(void);

/**
 * @brief Gets current R series from internal state machine
 * @retval current R series
 */
Rseries_t Get_Rseries(void);


uint8_t Decrease_Edge_Level(void);

uint8_t Increase_Edge_Level(void);
uint8_t Get_Edge_Level(void);

Adc::SamplingTimeClocks Decrease_period(void);

Adc::SamplingTimeClocks Increase_period(void);
float Get_period(void);

#endif /* LLUTILS_H_ */
