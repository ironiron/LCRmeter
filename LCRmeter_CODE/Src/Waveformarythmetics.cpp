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

uint16_t Waveform_arythmetics::filtered_buffer[max_buffer_size][2] =
  { 0 };
uint32_t Waveform_arythmetics::buffer_size = 0;
uint32_t Waveform_arythmetics::peak1 = 0;
uint32_t Waveform_arythmetics::peak2 = 0;
int32_t Waveform_arythmetics::alfa = 0;
uint32_t Waveform_arythmetics::frequency = 0;
uint32_t Waveform_arythmetics::point_time = 0;
uint32_t Waveform_arythmetics::minor_peak1 = 0;
uint32_t Waveform_arythmetics::minor_peak2 = 0;
uint32_t Waveform_arythmetics::mid_voltage = 0;
uint32_t Waveform_arythmetics::amplitude1 = 0;
uint32_t Waveform_arythmetics::amplitude2 = 0;

void Waveform_arythmetics::Calc_Moving_Average (uint32_t* buffer, uint32_t size,
						uint32_t step)
{
  if (step == 0)
    {
      return;
    }
  uint16_t temp1 = 0;
  uint16_t temp2 = 0;
  for (uint32_t i = 0, j = 0; i + step - 1 < size; i += step, j++)
    {
      for (uint32_t k = 0; k < step; k++)
	{
	  temp1 = temp1 + (buffer[k + i] & 0xffff); //ADC1 input
	  temp2 = temp2 + ((buffer[k + i] >> 16) & 0xffff); //ADC2 input
	}

      filtered_buffer[j][0] = temp1 / step;
      filtered_buffer[j][1] = temp2 / step;
      temp1 = 0;
      temp2 = 0;
    }
  buffer_size = size / step; //update end of buffer
}

//TODO what if peaks are not on the same side of sine?(are moved 360degree?)
void Waveform_arythmetics::Find_Peaks (void)
{
  uint32_t index = 0;
  uint32_t i = 0;

  while (1)
    {
      if (filtered_buffer[i][0] > mid_voltage)
	{
	  break;
	}
      if (i >= buffer_size)
	{
	  break;
	}
      i++;
    }

  for (; i < buffer_size; i++)
    {

      if (filtered_buffer[i][0] > filtered_buffer[index][0])
	{
	  index = i;
	}
      if (filtered_buffer[i][0] < mid_voltage)
	{
	  peak1 = index;
	  break;
	}
    }
  index = 0;
  while (1)
    {
      if (filtered_buffer[i][0] > mid_voltage)
	{
	  break;
	}
      if (i >= buffer_size)
	{
	  break;
	}
      i++;
    }

  for (; i < buffer_size; i++)
    {

      if (filtered_buffer[i][0] > filtered_buffer[index][0])
	{
	  index = i;
	}
      if (filtered_buffer[i][0] < mid_voltage)
	{
	  minor_peak1 = index;
	  break;
	}
    }

  if (point_time != 0 && minor_peak1 != peak1)
    {
      frequency = 1000000 / point_time / (minor_peak1 - peak1);
    }
  else
    {
      frequency = 0;
    }
////////////////////////////////////////
  // 2nd waveform
  i = 0;
  index = 0;
  while (1)
    {
      if (filtered_buffer[i][1] > mid_voltage)
	{
	  break;
	}
      if (i >= buffer_size)
	{
	  break;
	}
      i++;
    }

  for (; i < buffer_size; i++)
    {

      if (filtered_buffer[i][1] > filtered_buffer[index][1])
	{
	  index = i;
	}
      if (filtered_buffer[i][1] < mid_voltage)
	{
	  peak2 = index;
	  break;
	}
    }
  index = 0;
  while (1)
    {
      if (filtered_buffer[i][1] > mid_voltage)
	{
	  break;
	}
      if (i >= buffer_size)
	{
	  break;
	}
      i++;
    }

  for (; i < buffer_size; i++)
    {

      if (filtered_buffer[i][1] > filtered_buffer[index][0])
	{
	  index = i;
	}
      if (filtered_buffer[i][1] < mid_voltage)
	{
	  minor_peak2 = index;
	  break;
	}
    }
}

void Waveform_arythmetics::Calc_Alfa (void)
{
  alfa = int32_t (
      (int32_t (peak2) - int32_t (peak1)) * int32_t (point_time)
	  * int32_t (frequency) * 360 / 1000);
}

void Waveform_arythmetics::Calc_Amplitude (void)
{
  uint32_t index = peak1;
  for (uint32_t i = peak1; i < minor_peak1; i++)
    {
      if (filtered_buffer[i][0] < filtered_buffer[index][0])
	{
	  index = i;
	}
    }
  amplitude1 = (filtered_buffer[peak1][0] - filtered_buffer[index][0]) / 2;

  index = peak2;
  for (uint32_t i = peak2; i < minor_peak2; i++)
    {
      if (filtered_buffer[i][1] < filtered_buffer[index][1])
	{
	  index = i;
	}
    }
  amplitude2 = (filtered_buffer[peak2][1] - filtered_buffer[index][1]) / 2;
}

void Waveform_arythmetics::Process_Signal (uint32_t *buffer, uint32_t size,
					   uint32_t point_time_lenght)
{
  point_time=point_time_lenght;
  Calc_Moving_Average(buffer,size,5);
  Find_Peaks();
  Calc_Alfa();
  Calc_Amplitude();
}
