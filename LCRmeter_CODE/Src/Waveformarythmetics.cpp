/**
 ******************************************************************************
 * @file    Waveformarythmetics.cpp
 * @author  Rafał Mazurkiewicz
 * @date    09.10.2019
 * @brief   Signal conditioning for LCRmeter's ADC purposes
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#include <Waveformarythmetics.hpp>



uint16_t Waveform_arythmetics::filtered_buffer[1024][2] ={ 0 };
uint32_t Waveform_arythmetics::buffer_size=0;

void Waveform_arythmetics::Calc_Moving_Average (uint32_t* buffer, uint32_t size,
						uint32_t step)
{
  uint16_t temp1 = 0;
  uint16_t temp2 = 0;
  for (uint32_t i = 0, j = 0; i+step < size; i += step, j++)
    {
      for (uint32_t k = 0; k < step; k++)
	{
	  temp1 = temp1 + (buffer[k + i] & 0xffff);//ADC1 input
	  temp2 = temp2 + ((buffer[k + i]>>16) & 0xffff);//ADC2 input
	}

      filtered_buffer[j][0] = temp1/step;
      filtered_buffer[j][1] = temp2/step;
      temp1 = 0;
      temp2 = 0;
    }
  buffer_size=size/step;//update end of buffer
}
