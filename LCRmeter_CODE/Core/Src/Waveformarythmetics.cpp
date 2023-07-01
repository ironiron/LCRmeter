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

#include "Waveformarythmetics.hpp"
#include <stdlib.h>

uint16_t Waveform_arythmetics::filtered_buffer[2][max_buffer_size] =
{ 0 };
uint32_t Waveform_arythmetics::buffer_size = 0;
uint32_t Waveform_arythmetics::user_point_time = 0;
uint32_t Waveform_arythmetics::point_time = 0;
uint32_t Waveform_arythmetics::peak1 = 0;
uint32_t Waveform_arythmetics::peak2 = 0;
int32_t Waveform_arythmetics::alfa = 0;
uint32_t Waveform_arythmetics::frequency = 0;
uint32_t Waveform_arythmetics::minor_peak1 = 0;
uint32_t Waveform_arythmetics::minor_peak2 = 0;
uint32_t Waveform_arythmetics::mid_voltage = 0; /////////////////////////TODO make const, or add documenttation
uint32_t Waveform_arythmetics::amplitude1 = 0;
uint32_t Waveform_arythmetics::amplitude2 = 0;

void Waveform_arythmetics::Calc_Moving_Average(uint32_t* buffer, uint32_t size,
		uint32_t step)
{
	if (step == 0)
	{
		return;
	}

	point_time = user_point_time * step;
	uint16_t temp1 = 0;
	uint16_t temp2 = 0;
	for (uint32_t i = 0, j = 0; i + step - 1 < size; i += step, j++)
	{
		for (uint32_t k = 0; k < step; k++)
		{
			temp1 = temp1 + (buffer[k + i] & 0xffff); //ADC1 input
			temp2 = temp2 + ((buffer[k + i] >> 16) & 0xffff); //ADC2 input
		}

		filtered_buffer[0][j] = temp1 / step;
		filtered_buffer[1][j] = temp2 / step;
		temp1 = 0;
		temp2 = 0;
	}
	buffer_size = size / step; //update end of buffer
}

//TODO what if peaks are not on the same side of sine?(are moved 360degree?)
void Waveform_arythmetics::Find_Peaks(void)
{
	uint32_t index = 0;
	uint32_t i = 0;
//move to the lower value first
	while (1)
	{
		if (filtered_buffer[0][i] < mid_voltage)
		{
			break;
		}
		if (i >= buffer_size)
		{
			break;//TODO so like what will happen??? this is possible error and out of bound
		}
		i++;
	}
	index = i;
	//rising edge detect
	while (1)
	{
		if (filtered_buffer[0][i] > mid_voltage)
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

		if (filtered_buffer[0][i] > filtered_buffer[0][index])
		{
			index = i;
		}
		if (filtered_buffer[0][i] < mid_voltage)
		{
			peak1 = index;
			index = i;
			break;
		}
	}
	//correction for flat peak in sinewave
	i=peak1;
	for(;filtered_buffer[0][peak1]==filtered_buffer[0][i];i--)
	{
	}
	unsigned int c1,c2;
	c1=i;
	i++;
	for(;filtered_buffer[0][peak1]==filtered_buffer[0][i];i++)
	{
	}
	c2=i;
	peak1=(c2-c1)/2+c1;
	i=index;

//find next rising edge
	while (1)
	{
		if (filtered_buffer[0][i] > mid_voltage)
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

		if (filtered_buffer[0][i] > filtered_buffer[0][index])
		{
			index = i;
		}
		if (filtered_buffer[0][i] < mid_voltage)
		{
			minor_peak1 = index;
			break;
		}
	}
	if(i>=buffer_size)
	{
		minor_peak1=0;
	}
	if (point_time != 0 && minor_peak1 != peak1) //TODO put this in separate func.
	{
		frequency = 1000000 / point_time / (minor_peak1 - peak1);
	}
	else
	{
		frequency = 0;
	}
////////////////////////////////////////
	// 2nd waveform
	//TODO refractor this (its the same as 1st wave
	i = 0;
	index = 0;

	while (1)
	{
		if (filtered_buffer[1][i] < mid_voltage)
		{
			break;
		}
		if (i >= buffer_size)
		{
			break;
		}
		i++;
	}
	index = i;
	while (1)
	{
		if (filtered_buffer[1][i] > mid_voltage)
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

		if (filtered_buffer[1][i] > filtered_buffer[1][index])
		{
			index = i;
		}
		if (filtered_buffer[1][i] < mid_voltage)
		{
			peak2 = index;
			index = i;
			break;
		}
	}
	//correction for flat peak in sinewave
	i=peak2;
	for(;filtered_buffer[1][peak2]==filtered_buffer[1][i];i--)
	{
	}
	c1=i;
	i++;
	for(;filtered_buffer[1][peak2]==filtered_buffer[1][i];i++)
	{
	}
	c2=i;
	peak2=(c2-c1)/2+c1;
	i=index;
/////

	while (1)
	{
		if (filtered_buffer[1][i] > mid_voltage)
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

		if (filtered_buffer[1][i] > filtered_buffer[0][index])
		{
			index = i;
		}
		if (filtered_buffer[1][i] < mid_voltage)
		{
			minor_peak2 = index;
			break;
		}
	}
	if(i>=buffer_size)
	{
		minor_peak2=0;
	}
}

void Waveform_arythmetics::Calc_Alfa(void)
{
	if (abs(int32_t(peak2 - peak1)) < abs(int32_t(peak2 - minor_peak1)))
	{
		alfa = int32_t(
				(int32_t(peak2) - int32_t(peak1)) * int32_t(point_time)
						* int32_t(frequency) * 360 / 1000);
	}
	else
	{
		alfa = int32_t(
				(int32_t(peak2) - int32_t(minor_peak1)) * int32_t(point_time)
						* int32_t(frequency) * 360 / 1000);
	}
}

void Waveform_arythmetics::Calc_Amplitude(void)
{
	uint32_t index = peak1;
	for (uint32_t i = peak1; i < minor_peak1; i++)
	{
		// check for local minimum
		if (filtered_buffer[0][i] < filtered_buffer[0][index])
		{
			index = i;
		}
	}
	amplitude1 = (filtered_buffer[0][peak1] - filtered_buffer[0][index])/2;

	index = peak2;
	for (uint32_t i = peak2; i < minor_peak2; i++)
	{
		if (filtered_buffer[1][i] < filtered_buffer[1][index])
		{
			index = i;
		}
	}
	amplitude2 = (filtered_buffer[1][peak2] - filtered_buffer[1][index])/2;
}

void Waveform_arythmetics::Process_Signal(uint32_t *buffer, uint32_t size,
		uint32_t point_time_lenght)
{
	user_point_time = point_time_lenght;
	Calc_Moving_Average(buffer, size, 5);
	Find_Peaks();
	Calc_Alfa();
	Calc_Amplitude();
}

uint32_t Waveform_arythmetics::Get_Edge_index(uint32_t level, bool is_rising)//TODO fix naming of is_rising
{
	if (buffer_size < 1)
	{
		return 0;
	}
	uint32_t i = 0;

	if (is_rising == true)
	{
		for (i = 0; i < buffer_size; i++)
		{
			if (filtered_buffer[0][i] < level)
			{
				break;
			}
		}
		for (i++; i < buffer_size; i++)
		{

			if (filtered_buffer[0][i] > level)
			{
				if (filtered_buffer[0][i - 1] < filtered_buffer[0][i])
				{
					return i;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < buffer_size; i++)
		{
			if (filtered_buffer[0][i] > level)
			{
				break;
			}
		}
		for (i++; i < buffer_size; i++)
		{

			if (filtered_buffer[0][i] < level)
			{
				if (filtered_buffer[0][i - 1] > filtered_buffer[0][i])
				{
					return i;
				}
			}
		}
	}

	return 0;
}
