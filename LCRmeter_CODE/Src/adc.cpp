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

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef dma_init;
int retval=0;
uint32_t adc_value[SIZE_OF_ADC_BUFFER]={0};
enum CurrentState {NOT_INITIALISED,LCR,OSCILLOSCOPE}state=NOT_INITIALISED;

void Clean_Buffer(void)
{
  for(uint32_t i=0;i<SIZE_OF_ADC_BUFFER;i++)
    {
      adc_value[i]=0;
    }
}

uint32_t ADC_Set_Oscilloscope (void)
{
  if (state==CurrentState::LCR)
    {
  HAL_ADCEx_MultiModeStop_DMA(&hadc1);
  HAL_ADC_DeInit (&hadc1);
  HAL_ADC_DeInit (&hadc2);
  Clean_Buffer();
    }
  state=OSCILLOSCOPE;
  hadc1.Instance = ADC1;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  HAL_ADC_Init (&hadc1);

  HAL_ADCEx_Calibration_Start (&hadc1);

  ADC_ChannelConfTypeDef adc_ch;
  adc_ch.Channel = ADC_CHANNEL_7;
  adc_ch.Rank = ADC_REGULAR_RANK_1;
  adc_ch.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
  HAL_ADC_ConfigChannel (&hadc1, &adc_ch);

  HAL_ADCEx_Calibration_Start (&hadc1);

  dma_init.Instance = DMA1_Channel1;
  dma_init.Init.Direction = DMA_PERIPH_TO_MEMORY;
  dma_init.Init.PeriphInc = DMA_PINC_DISABLE;
  dma_init.Init.MemInc = DMA_MINC_ENABLE;
  dma_init.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  dma_init.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  dma_init.Init.Mode = DMA_CIRCULAR;
  dma_init.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init (&dma_init);
  __HAL_LINKDMA (&hadc1, DMA_Handle, dma_init);

  HAL_ADC_Start_DMA (&hadc1, (uint32_t*) adc_value, SIZE_OF_ADC_BUFFER);
  return 0;

}

uint32_t ADC_Set_LCR (void)
{
//
  if (state==CurrentState::OSCILLOSCOPE)
    {
      HAL_ADC_Stop_DMA(&hadc1);
      HAL_ADC_DeInit (&hadc1);
      Clean_Buffer();
    }
state=LCR;
  ADC_MultiModeTypeDef multimode =
    { 0 };
  ADC_ChannelConfTypeDef sConfig =
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
  /**Configure Regular Channel
   */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel (&hadc1, &sConfig) != HAL_OK)
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
  /**Configure Regular Channel
   */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel (&hadc2, &sConfig) != HAL_OK)
    {
      asm("bkpt 255");
    }

  retval = HAL_ADC_Start (&hadc1);
  if (retval != 0)
    {
      asm("bkpt 255");
    }
  retval = HAL_ADC_Start (&hadc2);
  if (retval != 0)
    {
      asm("bkpt 255");
    }
  retval = HAL_ADCEx_MultiModeStart_DMA (&hadc1, (uint32_t*) adc_value, SIZE_OF_ADC_BUFFER);
  if (retval != 0)
    {
      asm("bkpt 255");
    }
  return 0;
}
