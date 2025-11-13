/**
 ******************************************************************************
 * @file    adc.hpp
 * @author  Rafał Mazurkiewicz
 * @date    1.02.2020
 * @brief   header file for ADC. Made primary for LCR meter project.
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#ifndef ADC_HPP_
#define ADC_HPP_

#include <stdint.h>
#include <unordered_map>
#include "stm32g4xx_hal.h"

///@note ADC clock must be configured before using anything from this file.
///@note for current MCU (stm32f103c8) DMA channel 1 is used and interrupts need to be implemented
namespace Adc
{
///size of buffer for LCR and Oscilloscope mode
///
constexpr uint32_t size_of_adc_buffer = 1000;

extern uint32_t adc_buffer[size_of_adc_buffer];
extern uint32_t volt_temp[2]; ///array containing value of internal vref and temp.
extern uint32_t vref;  ///battery voltage

/*
 * @brief For \ref volt_temp array as a helper
 */
enum VOLT_TEMP_ENUM
{
    VOLT_INDEX=0,
    TEMP_INDEX=1,
};

enum SamplingTimeClocks
	: uint32_t
	{
		ADCCLK_2CYCLES5 = ADC_SAMPLETIME_2CYCLES_5,
	ADCCLK_6CYCLES5 = ADC_SAMPLETIME_6CYCLES_5,
	ADCCLK_12CYCLES5 = ADC_SAMPLETIME_12CYCLES_5,
	ADCCLK_24CYCLES5 = ADC_SAMPLETIME_24CYCLES_5,
	ADCCLK_47CYCLES5 = ADC_SAMPLETIME_47CYCLES_5,
	ADCCLK_92CYCLES5 = ADC_SAMPLETIME_92CYCLES_5,
	ADCCLK_247CYCLES5 = ADC_SAMPLETIME_247CYCLES_5,
	ADCCLK_640CYCLES5 = ADC_SAMPLETIME_640CYCLES_5
};



/**@brief
 * @param Sample time can be value of #SamplingTimeClocks
 * @retval negative -> Error code; positive -> Duration of one sample point in microseconds
 * @note return value is valid for ADC clock=12MHz
 */
double Set_Sampling_time(SamplingTimeClocks sampling_time);



/**@brief Sets ADC1 for reading temperature and 1,2V internal reference. It is used
 * to check battery voltage.
 * @retval HAL error code
 */
bool Start_LCR(void);
bool Start_Oscilloscope(void);

/**@brief Gets temperature.
 * @note #Set_Voltage_Temperature shall be executed prior to this function.
 * @retval temperature in celsius degrees
 */
int Get_Temperature(void);

/**@brief Gets battery voltage (vref for adc).
 * @note #Set_Voltage_Temperature shall be executed prior to this function.
 * @retval voltage in milivolts.
 */
uint32_t Get_Vref(void);

/**@brief Updates voltage reference with value given
 * @param vref reference voltage in milivolts
 */
void Update_Vref(uint32_t vref);

/**@brief Converts ADC readings to milivolts
 * @param 12bit value
 * @retval voltage in milivolts.
 */
uint32_t Adc_To_Milivolts(uint32_t adc);

float Get_sample_Time(SamplingTimeClocks sampling_time);

}  //namespace

#endif /* ADC_HPP_ */
