/**
  ******************************************************************************
  * @file    adc.hpp
  * @author  Rafał Mazurkiewicz
  * @date    08.10.2019
  * @brief   Class header file for DAC based on MCP4725 chip
  ******************************************************************************
  * @attention
  * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
  ******************************************************************************
  */

#ifndef ADC_HPP_
#define ADC_HPP_

#include "stdint.h"
#include "stm32f1xx_hal.h"

#include <map>
#include <unordered_map>
//TODO return values, dma,dma interrupts.

namespace Adc {

#define SIZE_OF_ADC_BUFFER 1024

extern volatile uint32_t adc_value[SIZE_OF_ADC_BUFFER];
extern volatile uint32_t volt_temp[2];

  enum SamplingTimeClocks: uint32_t
  {
    ADCCLK_1CYCLES5 =ADC_SAMPLETIME_1CYCLE_5,
    ADCCLK_7CYCLES5 =ADC_SAMPLETIME_7CYCLES_5,
    ADCCLK_13CYCLES5 = ADC_SAMPLETIME_13CYCLES_5,
    ADCCLK_28CYCLES5 = ADC_SAMPLETIME_28CYCLES_5,
    ADCCLK_41CYCLES5 = ADC_SAMPLETIME_41CYCLES_5,
    ADCCLK_55CYCLES5 = ADC_SAMPLETIME_55CYCLES_5,
    ADCCLK_71CYCLES5 = ADC_SAMPLETIME_71CYCLES_5,
    ADCCLK_239CYCLES5 =ADC_SAMPLETIME_239CYCLES_5
  };
   static std::unordered_map<SamplingTimeClocks,uint32_t>SampleTime=
      {
	  {ADCCLK_1CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_1CYCLE5},
	  {ADCCLK_7CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_7CYCLES5},
	  {ADCCLK_13CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_13CYCLES5},
	  {ADCCLK_28CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_28CYCLES5},
	  {ADCCLK_41CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_41CYCLES5},
	  {ADCCLK_55CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_55CYCLES5},
	  {ADCCLK_71CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_71CYCLES5},
	  {ADCCLK_239CYCLES5,ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_239CYCLES5}
      };

uint32_t Set_Oscilloscope(void);
uint32_t Set_LCR(void);
double Set_Sampling_time(SamplingTimeClocks sampling_time);
uint32_t Set_Voltage_temperature(void);
}

#endif /* ADC_HPP_ */
