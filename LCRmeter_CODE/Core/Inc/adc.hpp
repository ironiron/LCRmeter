/**
 ******************************************************************************
 * @file    adc.hpp
 * @author  Rafał Mazurkiewicz
 * @date    1.02.2020
 * @brief   header file for ADC. Made primary for LCR meter project.
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#ifndef ADC_HPP_
#define ADC_HPP_

#include <stdint.h>
#include "stm32g4xx_hal.h"
#include <unordered_map>
#include <map>

//TODO ADC clock can be modifiable?? I think? check on-the-fly change of frequency is required

///@note ADC clock must be configured before using anything from this file.
///@note for current MCU (stm32f103c8) DMA channel 1 is used and interrupts need to be implemented
namespace Adc
{
///size of buffer for LCR and Oscilloscope mode
///
const uint32_t size_of_adc_buffer = 1000;

extern volatile uint32_t adc_buffer[size_of_adc_buffer];
extern volatile uint32_t volt_temp[2]; ///array containing value of internal vref and temp.
extern uint32_t vref;  ///battery voltage

enum SamplingTimeClocks
	: uint32_t
	{
		ADCCLK_1CYCLES5 = ADC_SAMPLETIME_2CYCLES_5,
	ADCCLK_7CYCLES5 = ADC_SAMPLETIME_6CYCLES_5,
	ADCCLK_13CYCLES5 = ADC_SAMPLETIME_12CYCLES_5,
	ADCCLK_28CYCLES5 = ADC_SAMPLETIME_24CYCLES_5,
	ADCCLK_41CYCLES5 = ADC_SAMPLETIME_47CYCLES_5,
	ADCCLK_55CYCLES5 = ADC_SAMPLETIME_92CYCLES_5,
	ADCCLK_71CYCLES5 = ADC_SAMPLETIME_247CYCLES_5,
	ADCCLK_239CYCLES5 = ADC_SAMPLETIME_640CYCLES_5
};

///@note here sample time = sampling time + ADC conversion time (13 clock cycles)
static std::unordered_map<SamplingTimeClocks, uint32_t> SampleTime =
{
{ ADCCLK_1CYCLES5, 66 },
{ ADCCLK_7CYCLES5, 8 },
{ ADCCLK_13CYCLES5, 6 },
{ ADCCLK_28CYCLES5, 7 },
{ ADCCLK_41CYCLES5, 5 },
{ ADCCLK_55CYCLES5, 4 },
{ ADCCLK_71CYCLES5, 1 },
{ ADCCLK_239CYCLES5, 2 } };
//{ ADCCLK_1CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_2CYCLE5 },
//{ ADCCLK_7CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_6CYCLES5 },
//{ ADCCLK_13CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_13CYCLES5 },
//{ ADCCLK_28CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_28CYCLES5 },
//{ ADCCLK_41CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_41CYCLES5 },
//{ ADCCLK_55CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_55CYCLES5 },
//{ ADCCLK_71CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_71CYCLES5 },
//{ ADCCLK_239CYCLES5, ADC_CONVERSIONCLOCKCYCLES_SAMPLETIME_239CYCLES5 } };

/**@brief Deinitializes ADC
 */
void Deinitialize(void);

/**@brief In oscilloscope and LCR mode DMA works in single conversion (normal mode)
 * after /ref adc_buffer is full DMA is automatically stopped and this function resumes DMA.
 */
void Resume_DMA(void);

/**@brief Sets ADC1 for osc. input. deinit adc2
 * @retval HAL error code
 */
uint32_t Set_Oscilloscope(void);

/**@brief Sets ADC1 and ADC2.
 * @retval HAL error code
 */
uint32_t Set_LCR(void);
//TODO fix docs
/**@brief
 * @param Sample time can be value of #SamplingTimeClocks
 * @retval negative -> Error code; positive -> Duration of one sample point in microseconds
 * @note return value is valid for ADC clock=12MHz
 */
double Set_Sampling_time(SamplingTimeClocks sampling_time);

/**@brief Sets ADC1 for reading temperature and 1,2V internal reference. It is used
 * to check battery voltage.
 * @retval HAL error code
 */
uint32_t Set_Voltage_Temperature(void);

/**@brief Gets temperature.
 * @note #Set_Voltage_Temperature shall be executed prior to this function.
 * @retval temperature in celsius degrees
 */
int Get_Temperature(void);

/**@brief Gets battery voltage (vref for adc).
 * @note #Set_Voltage_Temperature shall be executed prior to this function.
 * @retval voltage in milivolts.
 */
uint32_t Update_Vref(void);

/**@brief Converts ADC readings to milivolts
 * @param 12bit value
 * @retval voltage in milivolts.
 */
uint32_t Adc_To_Milivolts(uint32_t adc);

}  //namespace

#endif /* ADC_HPP_ */
