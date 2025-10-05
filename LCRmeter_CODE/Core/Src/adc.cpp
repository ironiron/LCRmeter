/**
 ******************************************************************************
 * @file    adc.cpp
 * @author  Rafał Mazurkiewicz
 * @date    1.02.2020
 * @brief   source file for ADC. Made primary for LCR meter project.
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#include <stdio.h>
#include "adc.hpp""

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

namespace Adc
{
int retval = 0;
uint32_t adc_buffer[size_of_adc_buffer] =
{ 0 };
uint32_t volt_temp[2] =
{ 0 };

uint32_t vref = 3300;
constexpr double adc_freq = 170/4;

void Clean_Buffer(void)
{
	for (uint32_t i = 0; i < size_of_adc_buffer; i++)
	{
		adc_buffer[i] = 0;
	}
}

#define TS_CAL1 (*(volatile uint16_t *)0x1FFF75A8)
#define TS_CAL2 (*(volatile uint16_t *)0x1FFF75CA)
#define VREF_CAL (*(volatile uint16_t *)0x1FFF75AA)
#define TS_CAL1_TEMP 30
#define TS_CAL2_TEMP 130
#define TS_VREF 3000 // voltage at which calibration data has been obtained in millivolts


double Set_Sampling_time(SamplingTimeClocks sampling_time)
{
//	HAL_StatusTypeDef retval;
//	if (state == CurrentState::OSCILLOSCOPE)
//	{
//		adc_ch.Channel = ADC_CHANNEL_7;
//		adc_ch.Rank = ADC_REGULAR_RANK_1;
//		adc_ch.SamplingTime = sampling_time;
//		retval = HAL_ADC_ConfigChannel(&hadc1, &adc_ch);
//		if (retval != HAL_OK)
//		{
//			return (-retval);
//		}
//	}
//	if (state == CurrentState::LCR)
//	{
//		adc_ch.Channel = ADC_CHANNEL_8;
//		adc_ch.Rank = ADC_REGULAR_RANK_1;
//		adc_ch.SamplingTime = sampling_time;
//		retval = HAL_ADC_ConfigChannel(&hadc1, &adc_ch);
//		if (retval != HAL_OK)
//		{
//			return (-retval);
//		}
//
//		adc_ch.Channel = ADC_CHANNEL_9;
//		adc_ch.Rank = ADC_REGULAR_RANK_1;
//		adc_ch.SamplingTime = sampling_time;
//		retval = HAL_ADC_ConfigChannel(&hadc2, &adc_ch);
//		if (retval != HAL_OK)
//		{
//			return (-retval);
//		}
//	}
//	return 1 / adc_freq * SampleTime[sampling_time]; //microseconds, assuming ADC clock=12MHz
}

bool Start_LCR(void)
{
    auto retval = HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*) Adc::adc_buffer,
           Adc::size_of_adc_buffer);
   if (retval != 0)
   {
                printf("Start_LCR = %d\n",retval);
                return true;
   }
   return false;
}

bool Start_Oscilloscope(void)
{
    auto ret = HAL_ADC_Start_DMA(&hadc3, Adc::adc_buffer, Adc::size_of_adc_buffer);
   if (ret != 0)
   {
       printf("hadc3 ret = %d\n",ret);
                return true;
   }
   return false;
}

int Get_Temperature(void)
{
    //equation from datasheet
	int temp = (TS_CAL2_TEMP - TS_CAL1_TEMP)/(TS_CAL2-TS_CAL1)*(volt_temp[TEMP_INDEX]*vref/TS_VREF - TS_CAL1) + TS_CAL1_TEMP;
	return temp;
}

uint32_t Get_Vref(void)
{
    //not divide by zero!
    if (volt_temp[VOLT_INDEX] ==0)
    {
        return 0;
    }
    uint32_t v =  3000 * VREF_CAL / volt_temp[VOLT_INDEX];// magic number from DT
	return v;
}

void Update_Vref(uint32_t vref)
{
    Adc::vref=vref;
}

uint32_t Adc_To_Milivolts(uint32_t adc)
{
	return adc * vref / 4095;
}

} //namespace
