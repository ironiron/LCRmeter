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
#include <algorithm>

uint16_t Waveform_arythmetics::filtered_buffer[2][max_buffer_size] =
{ 0 };
uint32_t Waveform_arythmetics::buffer_size = 0;
uint32_t Waveform_arythmetics::user_point_time = 0;
uint32_t Waveform_arythmetics::point_time = 0;
uint32_t Waveform_arythmetics::peak1 = 0;
uint32_t Waveform_arythmetics::peaks[nbr_of_signals][max_peaks];
uint32_t Waveform_arythmetics::minimas[nbr_of_signals][max_peaks];
uint32_t Waveform_arythmetics::nbr_of_peaks[nbr_of_signals];
uint32_t Waveform_arythmetics::nbr_of_minimas[nbr_of_signals];
uint32_t Waveform_arythmetics::peak2 = 0;
int32_t Waveform_arythmetics::alfa = 0;
uint32_t Waveform_arythmetics::frequency = 0;
uint32_t Waveform_arythmetics::minor_peak1 = 0;
uint32_t Waveform_arythmetics::minor_peak2 = 0;
uint32_t Waveform_arythmetics::mid_voltage = 0; /////////////////////////TODO make const, or add documenttation
uint32_t Waveform_arythmetics::amplitude1 = 0;
uint32_t Waveform_arythmetics::amplitude2 = 0;

uint_fast16_t Waveform_arythmetics::hysteresis_samples = 0;

void Waveform_arythmetics::Calc_Moving_Average(uint32_t *buffer, uint32_t size,
        uint8_t step)
{
    if (step == 0)
    {
        return;
    }

    point_time = user_point_time * step;
    uint32_t temp1 = 0;
    uint32_t temp2 = 0;
    for (uint32_t i = 0, j = 0; i + step - 1 < size; i += step, j++)
    {
        for (uint32_t k = 0; k < step; k++)
        {
            temp1 = temp1 + (buffer[k + i] & 0xffff); //ADC1 input
            temp2 = temp2 + ((buffer[k + i] >> 16) & 0xffff); //ADC2 input
        }

        filtered_buffer[0][j] = (uint32_t) temp1 / step;
        filtered_buffer[1][j] = (uint32_t) temp2 / step;
        temp1 = 0;
        temp2 = 0;
    }
    buffer_size = size / step; //update end of buffer
}

void Waveform_arythmetics::Find_Peaks(void)
{
    int8_t hysteresis_counter = 0;

    //TODO refractor those GIANT ifs !!!!!!!!!!!!
    for (unsigned int signal = 0; signal < nbr_of_signals; signal++)
    {
        nbr_of_peaks[signal] = 0; //Starting from zero. Actual value will be valid after f. execution
        nbr_of_minimas[signal] = 0;
        bool is_first_peak = true;
        bool is_first_minimum = true;

        for (unsigned int i = 0; i < max_peaks; ++i)
        {
            peaks[signal][i] = 0;
            minimas[signal][i] = 0;
        }
         enum
        {
            ABOVE, BELOW, ND
        } level = ND;
        uint32_t max_value = 0;
        uint32_t min_value = 0xffff;

        for (uint32_t index = 0; index < buffer_size; index++)
        {
            if (filtered_buffer[signal][index] > mid_voltage)
            {
                if (hysteresis_counter < 0)
                {
                    hysteresis_counter = 0;
                }
                if (level == ABOVE)
                {
                    hysteresis_counter = 0;
                    if (filtered_buffer[signal][index] > max_value)
                    {
                        peaks[signal][nbr_of_peaks[signal]] = index;
                        max_value = filtered_buffer[signal][index];
                    }
                }
                else if (level == BELOW)
                {
                    hysteresis_counter++;
                    if (hysteresis_counter >= hysteresis_samples)
                    {
                        hysteresis_counter = 0;
                        level = ABOVE;
                        if (nbr_of_minimas[signal] < max_peaks)
                        {
                            if (is_first_minimum == true)
                            {
                                uint16_t min = 0xffff;
                                for (unsigned int i = 0; i < hysteresis_samples;
                                        i++)
                                {
                                    min = std::min(filtered_buffer[signal][i],
                                            min);
                                }
                                if (min > min_value)
                                {
                                    nbr_of_minimas[signal]++;
                                }
                                is_first_minimum = false;
                            }
                            else
                            {
                                nbr_of_minimas[signal]++;
                            }
                            min_value = 0xffff;
                        }
                    }
                }
                else if (level == ND)
                {
                    hysteresis_counter++;
                    if (hysteresis_counter >= hysteresis_samples)
                    {
                        hysteresis_counter = 0;
                        level = ABOVE;
                    }
                }
            }
            else
            {
                if (hysteresis_counter > 0)
                {
                    hysteresis_counter = 0;
                }
                if (level == ABOVE)
                {
                    hysteresis_counter--;
                    if (hysteresis_counter <= 0 - hysteresis_samples)
                    {
                        hysteresis_counter = 0;
                        level = BELOW;
                        if (nbr_of_peaks[signal] < max_peaks)
                        {
                            if (is_first_peak == true)
                            {
                                uint16_t max = 0;
                                for (unsigned int i = 0; i < hysteresis_samples;
                                        i++)
                                {
                                    max = std::max(filtered_buffer[signal][i], max);
                                }
                                if (max < max_value)
                                {
                                    nbr_of_peaks[signal]++;
                                }
                                is_first_peak = false;
                            }
                            else
                            {
                                nbr_of_peaks[signal]++;
                            }
                            max_value = 0;
                        }
                    }
                }
                else if (level == BELOW)
                {
                    hysteresis_counter = 0;
                        if (filtered_buffer[signal][index] < min_value)
                        {
                            minimas[signal][nbr_of_minimas[signal]] = index;
                            min_value = filtered_buffer[signal][index];
                        }
                }
                else if (level == ND)
                {
                    hysteresis_counter--;
                    if (hysteresis_counter <= 0 - hysteresis_samples)
                    {
                        hysteresis_counter = 0;
                        level = BELOW;
                    }
                }
            }
        }
        //////////////////////////////////////////////////////////////////////
        // handling last peak which happens without mid-range crossing.
        //////////////////////////////////////////////////////////////////////
        if (peaks[signal][nbr_of_peaks[signal]] != 0)
        {
            uint16_t max = 0;
            uint32_t mean = 0;
            for (unsigned int i = peaks[signal][nbr_of_peaks[signal]];
                    i < buffer_size; i++)
            {

                if (filtered_buffer[signal][i] > max)
                {
                    max = filtered_buffer[signal][i];
                }
            }
            for (unsigned int i = buffer_size - hysteresis_samples;
                    i < buffer_size; i++)
            {
                mean += filtered_buffer[signal][i];
            }
            mean = mean / (hysteresis_samples);
            if (max
                    <= filtered_buffer[signal][peaks[signal][nbr_of_peaks[signal]]]
                    && mean
                            != filtered_buffer[signal][peaks[signal][nbr_of_peaks[signal]]])
            {
                nbr_of_peaks[signal]++;
            }
        }

        if (minimas[signal][nbr_of_minimas[signal]] != 0)
        {
            uint16_t min = 0xffff;
            uint32_t mean = 0;
            for (unsigned int i = minimas[signal][nbr_of_minimas[signal]];
                    i < buffer_size; i++)
            {
                if (filtered_buffer[signal][i] < min)
                {
                    min = filtered_buffer[signal][i];
                }
            }
            for (unsigned int i = buffer_size - hysteresis_samples;
                    i < buffer_size; i++)
            {
                mean += filtered_buffer[signal][i];
            }
            mean = mean / (hysteresis_samples);
            if (min
                    >= filtered_buffer[signal][minimas[signal][nbr_of_minimas[signal]]]
                    && mean
                            != filtered_buffer[signal][minimas[signal][nbr_of_minimas[signal]]])
            {
                nbr_of_minimas[signal]++;
            }

        }
        //////////////////////////////////////////////////////////////////////
        // find center of peak if signal is flat around it.
        //////////////////////////////////////////////////////////////////////
        for (unsigned int i = 0; i < nbr_of_peaks[signal]; i++)
        {
            uint32_t i1 = peaks[signal][i];        //beginning index
            uint32_t i2 = peaks[signal][i];        //end index after loop
            uint32_t temp_mid = i1;
            for (; i2 <= temp_mid + hysteresis_samples; i2++)
            {
                if (filtered_buffer[signal][i2] == filtered_buffer[signal][i1])
                {
                    temp_mid = i2;
                }

            }
            i2 = temp_mid;
            peaks[signal][i] = ((i2 - i1) / 2) + i1;
        }

        for (unsigned int i = 0; i < nbr_of_minimas[signal]; i++)
        {
            uint32_t i1 = minimas[signal][i];        //beginning index
            uint32_t i2 = minimas[signal][i];        //end index after loop
            uint32_t temp_mid = i1;
            for (; i2 <= temp_mid + hysteresis_samples; i2++)
            {
                if (filtered_buffer[signal][i2] == filtered_buffer[signal][i1])
                {
                    temp_mid = i2;
                }

            }
            i2 = temp_mid;
            minimas[signal][i] = ((i2 - i1) / 2) + i1;
        }
        //////////////////////////////////////////////////////////////////////
        //Handling boundary conditions so that peaks and minimas are ignored
        //////////////////////////////////////////////////////////////////////
        // beginning of the buffer hysteresis
        if (peaks[signal][0] <= hysteresis_samples && nbr_of_peaks[signal] != 0)
        {
            for (unsigned int i = 0; i < nbr_of_peaks[signal] - 1; i++)
            {
                peaks[signal][i] = peaks[signal][i + 1];
            }
            nbr_of_peaks[signal]--;
        }
        // end of buffer hysteresis
        if (nbr_of_peaks[signal] != 0
                && peaks[signal][nbr_of_peaks[signal] - 1]
                        >= buffer_size - hysteresis_samples)
        {
            nbr_of_peaks[signal]--;
        }

        // same as above but with minimas
        if (minimas[signal][0] <= hysteresis_samples
                && nbr_of_minimas[signal] != 0)
        {
            for (unsigned int i = 0; i < nbr_of_minimas[signal] - 1; i++)
            {
                minimas[signal][i] = minimas[signal][i + 1];
            }
            nbr_of_minimas[signal]--;
        }
        if (nbr_of_minimas[signal] != 0
                && minimas[signal][nbr_of_minimas[signal] - 1]
                        >= buffer_size - hysteresis_samples)
        {
            nbr_of_minimas[signal]--;
        }
    }
}

bool Waveform_arythmetics::Get_Indexes(uint32_t *first, uint32_t *second)
{
    bool one_side=false;

    if(nbr_of_peaks[0]>1 )
    {
        *first=peaks[0][0];
        *second=peaks[0][1];
        one_side=true;
    }
    else if (nbr_of_minimas[0]>1)
    {
        *first=minimas[0][0];
        *second=minimas[0][1];
        one_side=true;
    }
    else if (nbr_of_minimas[0]>0 && nbr_of_peaks[0]>0)
    {
        one_side=false;
        if(peaks[0][0]>minimas[0][0])
        {
           *first=minimas[0][0];
            *second=peaks[0][0];
        }
        else
        {
            *first=peaks[0][0];
            *second=minimas[0][0];
        }
    }
    else
    {
        first=NULL;
        second=NULL;
    }
    return one_side;
}

void Waveform_arythmetics::Calc_Frequency(void)
{
    uint32_t i1=0;
    uint32_t i2=0;
    auto one_side=Get_Indexes(&i1,&i2);

    if(i1 != 0 && i2 !=0)
    {
        if(one_side==true)
        {
            frequency=1000000/(i2-i1)/point_time;
        }
        else
        {
            frequency=1000000/(i2-i1)/point_time/2;
        }
    }
    else
    {
        frequency=0;
    }
}

void Waveform_arythmetics::Calc_Alfa(void)
{
    uint32_t i1=0;
    uint32_t i2=0;
    Get_Indexes(&i1,&i2);

    bool above1,above2;

    if (nbr_of_peaks[0]>0)
    {
       i1=peaks[0][0];
       above1=true;
    }
    else if (nbr_of_minimas[0]>0)
    {
       i1=peaks[0][0];
       above1=false;
    }
    else
    {
        alfa=0xfffffff;
        return;
    }

    if (nbr_of_peaks[1]>0)
    {
       i2=peaks[1][0];
       above2=true;
    }
    else if (nbr_of_minimas[1]>0)
    {
       i2=peaks[1][0];
       above2=false;
    }
    else
    {
        alfa=0xfffffff;
        return;
    }

    int32_t diff = abs(int32_t(i1 - i2));
    if (above1 != above2) //one side no matter upper or lower TODO check name
    {
        diff *= 2; //we got only half of a way (one peak or one minima)
    }
    alfa = int32_t(diff) * int32_t(point_time) * int32_t(frequency) * 360 / 1000;
    if (alfa > 180000)
    {
        alfa = alfa - 360000;
    }
}

void Waveform_arythmetics::Calc_Amplitude(void)
{
    amplitude1 = uint32_t((filtered_buffer[0][peaks[0][0]] - filtered_buffer[0][minimas[0][0]]) / 2);
    amplitude2 = uint32_t((filtered_buffer[1][peaks[1][0]] - filtered_buffer[1][minimas[1][0]]) / 2);
}

void Waveform_arythmetics::Process_Signal(uint32_t *buffer, uint32_t size,
        uint32_t point_time_lenght)
{
    user_point_time = point_time_lenght;
    Calc_Moving_Average(buffer, size, 5);
    Find_Peaks();
    Calc_Frequency();
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
