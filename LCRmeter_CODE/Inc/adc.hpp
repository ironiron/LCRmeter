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

//TODO return values, dma, variable frequency,dma interrupts.
//TODO
//ADC conversion time: 12.5cycles + specified conversion time cycles

#define SIZE_OF_ADC_BUFFER 1024

extern uint32_t adc_value[SIZE_OF_ADC_BUFFER];

uint32_t ADC_Set_Oscilloscope(void);
uint32_t ADC_Set_LCR(void);




#endif /* ADC_HPP_ */
