/**
 ******************************************************************************
 * @file    adc.cpp
 * @author  Rafał Mazurkiewicz
 * @date    08.10.2019
 * @brief   Class header file for DAC based on MCP4725 chip
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#include <adc.hpp>
#include "stm32f1xx_hal.h"

//TODO add battery voltage

ADC_HandleTypeDef hadc1 =
  { 0 };
ADC_HandleTypeDef hadc2 =
  { 0 };
DMA_HandleTypeDef hdma_adc1 =
  { 0 };
DMA_HandleTypeDef dma_init =
  { 0 };

ADC_ChannelConfTypeDef adc_ch =
  { 0 };

namespace Adc
{
  int retval = 0;
  volatile uint32_t adc_value[SIZE_OF_ADC_BUFFER] =
    { 0 };

  enum CurrentState
  {
    NOT_INITIALISED, LCR, OSCILLOSCOPE
  } state = NOT_INITIALISED;

  void Clean_Buffer (void)
  {
    for (uint32_t i = 0; i < SIZE_OF_ADC_BUFFER; i++)
      {
	adc_value[i] = 0;
      }
  }

  uint32_t Set_Oscilloscope (void)
  {
    if (state == CurrentState::LCR)
      {
	HAL_ADCEx_MultiModeStop_DMA (&hadc1);
	HAL_ADC_DeInit (&hadc1);
	HAL_ADC_DeInit (&hadc2);
	Clean_Buffer ();
      }

    state = OSCILLOSCOPE;

    hadc1.Instance = ADC1;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.NbrOfConversion = 1;
    HAL_ADC_Init (&hadc1);

    HAL_ADCEx_Calibration_Start (&hadc1);

    Set_Sampling_time (SamplingTimeClocks::ADCCLK_71CYCLES5);

    dma_init.Instance = DMA1_Channel1;
    dma_init.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_init.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_init.Init.MemInc = DMA_MINC_ENABLE;
    dma_init.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    dma_init.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    dma_init.Init.Mode = DMA_NORMAL; //TODO make the same for dual adc
    dma_init.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init (&dma_init);
    __HAL_LINKDMA(&hadc1, DMA_Handle, dma_init);

    HAL_ADC_Start_DMA (&hadc1, (uint32_t*) adc_value, SIZE_OF_ADC_BUFFER);

    return 0;
  }

  uint32_t Set_LCR (void)
  {
    if (state==CurrentState::OSCILLOSCOPE)
      {
        HAL_ADC_Stop_DMA(&hadc1);
        HAL_ADC_DeInit (&hadc1);
        Clean_Buffer();
      }
  state=LCR;
    ADC_MultiModeTypeDef multimode =
      { 0 };


    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init (&hadc1) != HAL_OK)
      {
        asm("bkpt 255");
      }
    /**Configure the ADC multi-mode
     */
    multimode.Mode = ADC_DUALMODE_REGSIMULT;
    if (HAL_ADCEx_MultiModeConfigChannel (&hadc1, &multimode) != HAL_OK)
      {
        asm("bkpt 255");
      }

    //////////////////////////////////////////////////////////
    //ADC 2
    ////////////////////////////////////////////////////

    hadc2.Instance = ADC2;
    hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc2.Init.ContinuousConvMode = ENABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init (&hadc2) != HAL_OK)
      {
        asm("bkpt 255");
      }

    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
	asm("bkpt 255");
    }
//
    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

    retval = HAL_ADCEx_MultiModeStart_DMA (&hadc1, (uint32_t*) adc_value, SIZE_OF_ADC_BUFFER);
    if (retval != 0)
      {
        asm("bkpt 255");
      }

    Set_Sampling_time(SamplingTimeClocks::ADCCLK_239CYCLES5);
    return 0;
  }


//
//    dma_init.Instance = DMA1_Channel1;
//    dma_init.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    dma_init.Init.PeriphInc = DMA_PINC_DISABLE;
//    dma_init.Init.MemInc = DMA_MINC_ENABLE;
//    dma_init.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//    dma_init.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//    dma_init.Init.Mode = DMA_CIRCULAR;
//    dma_init.Init.Priority = DMA_PRIORITY_HIGH;
//        if (HAL_DMA_Init(&dma_init) != HAL_OK)
//        {
//    	asm("bkpt 255");
//        }
//
//    retval = HAL_ADCEx_MultiModeStart_DMA (&hadc1, (uint32_t*) adc_value,
//					   SIZE_OF_ADC_BUFFER);
//    if (retval != 0)
//      {
//	asm("bkpt 255");
//      }
//
//
//    return 0;


  uint32_t Set_Voltage_temperature(void)
  {
    //TODO finish
//
//
//    ADC_ChannelConfTypeDef sConfig = {0};
//
//    /* USER CODE BEGIN ADC1_Init 1 */
//
//    /* USER CODE END ADC1_Init 1 */
//    /** Common config
//    */
//    hadc1.Instance = ADC1;
//    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
//    hadc1.Init.ContinuousConvMode = ENABLE;
//    hadc1.Init.DiscontinuousConvMode = DISABLE;
//    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//    hadc1.Init.NbrOfConversion = 2;
//    if (HAL_ADC_Init(&hadc1) != HAL_OK)
//    {
//      Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
//    sConfig.Rank = ADC_REGULAR_RANK_1;
//    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//    {
//      Error_Handler();
//    }
//    /** Configure Regular Channel
//    */
//    sConfig.Channel = ADC_CHANNEL_VREFINT;
//    sConfig.Rank = ADC_REGULAR_RANK_2;
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_RCC_ADC1_CLK_ENABLE();
//
//    /* ADC1 DMA Init */
//    /* ADC1 Init */
//    hdma_adc1.Instance = DMA1_Channel1;
//    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_adc1.Init.Mode = DMA_NORMAL;
//    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
//    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);
  }

  double Set_Sampling_time (SamplingTimeClocks sampling_time)
  {
    if (state == CurrentState::OSCILLOSCOPE)
      {
	adc_ch.Channel = ADC_CHANNEL_7;
	adc_ch.Rank = ADC_REGULAR_RANK_1;
	adc_ch.SamplingTime = sampling_time;
	HAL_ADC_ConfigChannel (&hadc1, &adc_ch);
      }
    if (state == CurrentState::LCR)
      {
	adc_ch.Channel = ADC_CHANNEL_8;
	adc_ch.Rank = ADC_REGULAR_RANK_1;
	adc_ch.SamplingTime = sampling_time;
	HAL_ADC_ConfigChannel (&hadc1, &adc_ch);

	adc_ch.Channel = ADC_CHANNEL_9;
	adc_ch.Rank = ADC_REGULAR_RANK_1;
	adc_ch.SamplingTime = sampling_time;
	HAL_ADC_ConfigChannel (&hadc2, &adc_ch);
      }

    return 1/12.0*SampleTime[sampling_time];//microseconds, assuming ADC clock=12MHz
  }
}
