/**
 ******************************************************************************
 * @file    Waveformarythmetics.hpp
 * @author  Rafał Mazurkiewicz
 * @date    09.10.2019
 * @brief   Signal conditioning for LCRmeter's ADC purposes
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */
#ifndef WAVEFORMARYTHMETICS_HPP_
#define WAVEFORMARYTHMETICS_HPP_

#include <stdint.h>

///\note this file is made solely for LCRmeter project!

class Waveform_arythmetics
{
public:

  Waveform_arythmetics ()
  {
    // TODO Auto-generated constructor stub

  }
  ///Two dim. array that contain filtered ADC1 readings in 1st row and ADC2 in 2nd,
  static uint16_t filtered_buffer[1024][2];
  ///size of filtered buffer ( after averaging it is smaller than original one
  static uint32_t buffer_size;

  /**@brief filtering data by moving mean
   * @param buffer pointer to array made of readings of ADC Data register
   * @param size of array
   * @param step of moving mean
   */
  static void Calc_Moving_Average(uint32_t *buffer,uint32_t size, uint32_t step);


};

//

#endif /* WAVEFORMARYTHMETICS_HPP_ */
