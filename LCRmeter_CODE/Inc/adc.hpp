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
//TODO return values, dma, variable frequency,dma interrupts.
//TODO
//ADC conversion time: 12.5cycles + specified conversion time cycles

namespace Adc {



#define SIZE_OF_ADC_BUFFER 1024

extern volatile uint32_t adc_value[SIZE_OF_ADC_BUFFER];

uint32_t Set_Oscilloscope(void);
uint32_t Set_LCR(void);
uint32_t Set_Sampling_time(uint32_t sampling_time);

}


#endif /* ADC_HPP_ */
