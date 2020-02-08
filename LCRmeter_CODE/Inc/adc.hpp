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

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include <unordered_map>
#include <map>

//TODO return values, dma,dma interrupts.

//note ADC clock must be configured before using anything from this file.

namespace Adc
{
  ///size of buffer for LCR and Oscilloscope mode
  ///
  const uint32_t size_of_adc_buffer = 2000;

  extern volatile uint32_t adc_value[size_of_adc_buffer];
  extern volatile uint32_t volt_temp[2];///array containing value of internal vref and temp.
  extern uint32_t vref;///battery voltage

  enum SamplingTimeClocks
    : uint32_t
      {
	ADCCLK_1CYCLES5 = ADC_SAMPLETIME_1CYCLE_5,
    ADCCLK_7CYCLES5 = ADC_SAMPLETIME_7CYCLES_5,
    ADCCLK_13CYCLES5 = ADC_SAMPLETIME_13CYCLES_5,
    ADCCLK_28CYCLES5 = ADC_SAMPLETIME_28CYCLES_5,
    ADCCLK_41CYCLES5 = ADC_SAMPLETIME_41CYCLES_5,
    ADCCLK_55CYCLES5 = ADC_SAMPLETIME_55CYCLES_5,
    ADCCLK_71CYCLES5 = ADC_SAMPLETIME_71CYCLES_5,
    ADCCLK_239CYCLES5 = ADC_SAMPLETIME_239CYCLES_5
  };

  static std::unordered_map<SamplingTimeClocks, uint32_t> SampleTime =
    {
      { ADCCLK_1CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_1CYCLE5 },
      { ADCCLK_7CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_7CYCLES5 },
      { ADCCLK_13CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_13CYCLES5 },
      { ADCCLK_28CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_28CYCLES5 },
      { ADCCLK_41CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_41CYCLES5 },
      { ADCCLK_55CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_55CYCLES5 },
      { ADCCLK_71CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_71CYCLES5 },
      { ADCCLK_239CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_239CYCLES5 } };

  /**@brief Sets ADC1 for osc. input. deinit adc2
   * @retval HAL error code
   */
  uint32_t Set_Oscilloscope (void);

  /**@brief Sets ADC1 and ADC2.
   * @retval HAL error code
   */
  uint32_t Set_LCR (void);

  /**@brief Sets ADC1 for osc. input. deinit adc2
   * @param Sample time can be value of #SamplingTimeClocks
   * @retval Duration of one sample point in microseconds
   * @note return value is valid for ADC clock=12MHz
   */
  double Set_Sampling_time (SamplingTimeClocks sampling_time);

  /**@brief Sets ADC1 for reading temperature and 1,2V internal reference. It is used
   * to calculate battery voltage.
   * @retval HAL error code
   */
  uint32_t Set_Voltage_Temperature (void);

  /**@brief Gets temperature.
   * @note #Set_Voltage_Temperature shall be executed prior to this function.
   * @retval temperature in celsius degrees
   */
  int Get_Temperature (void);

  /**@brief Gets battery voltage (vref for adc).
   * @note #Set_Voltage_Temperature shall be executed prior to this function.
   * @retval voltage in milivolts.
   */
  uint32_t Update_Vref (void);

  /**@brief Converts ADC readings to milivolts
   * @param 12bit value
   * @retval voltage in milivolts.
   */
  uint32_t Adc_To_Milivolts (uint32_t adc);
}

#endif /* ADC_HPP_ */
