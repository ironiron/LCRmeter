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
#include "adc.hpp"

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


///@note here sample time = sampling time + ADC conversion time (12.5 clock cycles)
static std::unordered_map<SamplingTimeClocks, uint32_t> SampleTime =
{
{ ADCCLK_2CYCLES5, 15 },
{ ADCCLK_6CYCLES5, 19 },
{ ADCCLK_12CYCLES5, 25 },
{ ADCCLK_24CYCLES5, 37 },
{ ADCCLK_47CYCLES5, 60 },
{ ADCCLK_92CYCLES5, 105 },
{ ADCCLK_247CYCLES5, 260 },
{ ADCCLK_640CYCLES5, 653 } };

double Set_Sampling_time(SamplingTimeClocks sampling_time)
{
    HAL_ADC_Stop_DMA(&hadc3);
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_12;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = sampling_time;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
    {
      return -1.0;
    }
    Start_Oscilloscope();
}

float Get_sample_Time(SamplingTimeClocks sampling_time)
{
    return static_cast<float>(SampleTime.at(sampling_time))/42.5;
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
