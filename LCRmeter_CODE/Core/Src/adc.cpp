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

#include <adc.hpp>

//ADC_HandleTypeDef hadc1 =
//{ 0 };
//ADC_HandleTypeDef hadc2 =
//{ 0 };
//DMA_HandleTypeDef hdma_adc1 =
//{ 0 };
//
//ADC_ChannelConfTypeDef adc_ch =
//{ 0 };

namespace Adc
{
int retval = 0;
volatile uint32_t adc_buffer[size_of_adc_buffer] =
{ 0 };
volatile uint32_t volt_temp[2] =
{ 0 };

uint32_t vref = 3300;
double adc_freq = 12.0;
enum CurrentState
{
	NOT_INITIALISED, LCR, OSCILLOSCOPE, VOLT_TEMP
} state = NOT_INITIALISED;

void Clean_Buffer(void)
{
	for (uint32_t i = 0; i < size_of_adc_buffer; i++)
	{
		adc_buffer[i] = 0;
	}
}

void Deinitialize(void)
{
//	if (state == CurrentState::LCR)
//	{
//		HAL_ADCEx_MultiModeStop_DMA(&hadc1);
//		HAL_ADC_DeInit(&hadc1);
//		HAL_ADC_DeInit(&hadc2);
//		HAL_DMA_DeInit(&hdma_adc1);
//		Clean_Buffer();
//	}
//
//	if (state == CurrentState::OSCILLOSCOPE)
//	{
//		HAL_ADC_Stop_DMA(&hadc1);
//		HAL_ADC_DeInit(&hadc1);
//		HAL_DMA_DeInit(&hdma_adc1);
//		Clean_Buffer();
//	}
//	if (state == CurrentState::VOLT_TEMP)
//	{
//		HAL_ADC_Stop_DMA(&hadc1);
//		HAL_ADC_DeInit(&hadc1);
//		HAL_DMA_DeInit(&hdma_adc1);
//	}
}

void Resume_DMA(void)
{
//	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
//	DMA1_Channel1->CNDTR = Adc::size_of_adc_buffer;
//	DMA1_Channel1->CCR |= DMA_CCR_EN;
}

uint32_t Set_Oscilloscope(void)
{
//	uint32_t retval = 0;
//
//	Deinitialize();
//
//	state = OSCILLOSCOPE;
//
//	hadc1.Instance = ADC1;
//	hadc1.Init.ContinuousConvMode = ENABLE;
//	hadc1.Init.DiscontinuousConvMode = DISABLE;
//	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
//	hadc1.Init.NbrOfConversion = 1;
//	retval = HAL_ADC_Init(&hadc1);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	retval = HAL_ADCEx_Calibration_Start(&hadc1);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	Set_Sampling_time(SamplingTimeClocks::ADCCLK_71CYCLES5);
//
//	hdma_adc1.Instance = DMA1_Channel1;
//	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
//	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
//	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//	hdma_adc1.Init.Mode = DMA_NORMAL;
//	hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
//	retval = HAL_DMA_Init(&hdma_adc1);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
//
//	retval = HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buffer,
//			size_of_adc_buffer);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	return 0;
}

uint32_t Set_LCR(void)
{
//	uint32_t retval = 0;
//	ADC_MultiModeTypeDef multimode =
//	{ 0 };
//
//	Deinitialize();
//	state = LCR;
//
//	hadc1.Instance = ADC1;
//	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
//	hadc1.Init.ContinuousConvMode = ENABLE;
//	hadc1.Init.DiscontinuousConvMode = DISABLE;
//	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	hadc1.Init.NbrOfConversion = 1;
//	retval = HAL_ADC_Init(&hadc1);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	multimode.Mode = ADC_DUALMODE_REGSIMULT;
//	retval = HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	//////////////////////////////////////////////////////////
//	//ADC 2
//	////////////////////////////////////////////////////
//
//	hadc2.Instance = ADC2;
//	hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
//	hadc2.Init.ContinuousConvMode = ENABLE;
//	hadc2.Init.DiscontinuousConvMode = DISABLE;
//	hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	hadc2.Init.NbrOfConversion = 1;
//	retval = HAL_ADC_Init(&hadc2);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	hdma_adc1.Instance = DMA1_Channel1;
//	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
//	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
//	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//	hdma_adc1.Init.Mode = DMA_NORMAL;
//	hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
//	retval = HAL_DMA_Init(&hdma_adc1);
//	if (retval != 0)
//	{
//		return retval;
//	}
//	Set_Sampling_time(SamplingTimeClocks::ADCCLK_239CYCLES5);
//
//	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
//
//	retval = HAL_ADC_Start(&hadc2);
//	if (retval != 0)
//	{
//		return retval;
//	}
//	retval = HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*) adc_buffer,
//			size_of_adc_buffer);
//	if (retval != 0)
//	{
//		return retval;
//	}
	return 0;
}

//TODO add to Enum etc...
uint32_t Set_Voltage_Temperature(void)
{
//	uint32_t retval = 0;
//	ADC_ChannelConfTypeDef sConfig =
//	{ 0 };
//
//	Deinitialize();
//	state = VOLT_TEMP;
//
//	hadc1.Instance = ADC1;
//	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
//	hadc1.Init.ContinuousConvMode = ENABLE;
//	hadc1.Init.DiscontinuousConvMode = DISABLE;
//	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	hadc1.Init.NbrOfConversion = 2;
//	retval = HAL_ADC_Init(&hadc1);
//	if (retval != 0)
//	{
//		return retval;
//	}
//	/** Configure Regular Channel
//	 */
//	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
//	sConfig.Rank = ADC_REGULAR_RANK_1;
//	sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5; //datasheet says best is 71.5 sampling time
//	retval = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//	if (retval != 0)
//	{
//		return retval;
//	}
//	/** Configure Regular Channel
//	 */
//	sConfig.Channel = ADC_CHANNEL_VREFINT;
//	sConfig.Rank = ADC_REGULAR_RANK_2;
//	sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
//	retval = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	/* ADC1 DMA Init */
//	/* ADC1 Init */
//	hdma_adc1.Instance = DMA1_Channel1;
//	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
//	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
//	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//	hdma_adc1.Init.Mode = DMA_CIRCULAR;
//	hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
//	retval = HAL_DMA_Init(&hdma_adc1);
//	if (retval != 0)
//	{
//		return retval;
//	}
//
//	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
//
//	retval = HAL_ADC_Start_DMA(&hadc1, (uint32_t*) volt_temp, 2);
//	if (retval != 0)
//	{
//		return retval;
//	}
//	return 0;
}

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

int Get_Temperature(void)
{
//	int temp = (14300 - (volt_temp[0] * vref * 10 / 4095)) / 43 + 25; //from datasheet
//	return temp;
}

uint32_t Update_Vref(void)
{
//	uint32_t temp = 1200 * 4095 / volt_temp[1];
//	vref = temp;
//	return temp;
}

uint32_t Adc_To_Milivolts(uint32_t adc)
{
//	return adc * vref / 4095;
}

} //namespace
