/**
 ******************************************************************************
 * @file    Waveformarythmetics.hpp
 * @author  Rafał Mazurkiewicz
 * @date    09.10.2019
 * @brief   Signal conditioning for LCRmeter's ADC purposes
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */
#ifndef WAVEFORMARYTHMETICS_HPP_
#define WAVEFORMARYTHMETICS_HPP_

#include <stdint.h>
//TODO fix comments and docs
//TODO make this class more safer

/** \defgroup  Waveform Waveform
 * @brief Signal conditioning for LCRmeter's ADC purposes
 *  @{
 */

/**
 *
 *
 * This Class is made for conditioning signal from STM32F103 MCU ADC working in dual ADC
 * with 32bit data wide and data from ADCs stored in upper and lower 16bit.
 * It is developed primary for stm32f103 MCU only. All members and methods are static.
 *
 * Use this class with Process_Signal function or by using below sequence:
 * - set variable:
 * 	- #point_time
 * - call functions:
 * 	- #Calc_Moving_Average
 * 	- #Find_Peaks
 * 	- #Calc_Alfa
 * 	- #Calc_Amplitude
 *
 * From now on all the variable shall contain valid data.
 * \note No getters nor setters provided! Make sure nothing writes data
 *  to class members.
 * \note This class is made solely for LCRmeter project.
 */

class Waveform_arythmetics
{
private:
  //size of buffer used for conditioning
  static const uint32_t max_buffer_size=2000;
  /// Will ignore data on buffer boundaries to avoid processing not reliable data.
  static const uint_least8_t boundary_ignore_percentage=5;

public:
  ///length of time of 1 point in buffer in microseconds
  static uint32_t user_point_time;
  ///length of time of 1 point in buffer after averaging in microseconds
  static uint32_t point_time;
  ///Two dim. array that contain filtered ADC1 readings in 1st row and ADC2 in 2nd,
  static uint16_t filtered_buffer[2][max_buffer_size];

  //outputs
  ///size of filtered buffer ( after averaging it is smaller than original one).
  static uint32_t buffer_size;
  ///max value in ADC1
  static uint32_t peak1;
  ///max value in ADC2
  static uint32_t peak2;
  ///phase swift with resolution of 0.001 degrees
  static int32_t alfa; //TODO alfa is with resolution 330 or sth? why
  ///frequency  with resolution of 1 Hz
  static uint32_t frequency;
  /// 2nd max value in ADC1
  static uint32_t minor_peak1;
  /// 2nd max value in ADC2
  static uint32_t minor_peak2;
  /// value of the 0 point of input sine
  static uint32_t mid_voltage;
  //TODO fix units!
  /// amplitude in ADC1
  static uint32_t amplitude1;
  /// amplitude in ADC2
  static uint32_t amplitude2;

  /**@brief filtering data by moving mean
   * @param buffer pointer to array made of readings of ADC Data register
   * @param size of array
   * @param step of moving mean
   */
  static void Calc_Moving_Average (uint32_t *buffer, uint32_t size,
				   uint32_t step);

  /**@brief Finds 2 maximum peaks from filtered_buffer and stores them in peakx and minor_peakx
   * @note if minor_peakX=0 and and peakX !=0 means only one peak has been found
   */
  static void Find_Peaks (void);

  /** Calculates phase swift based on #peak1, #peak2, #frequency and #point_time variables
   */
  static void Calc_Alfa (void);

  /** Calculates amplitudes from #peak1, #peak2 and #filtered_buffer
   */
  static void Calc_Amplitude (void);

  /**@brief One function to take care of everything. Read class documentation for more info.
   * @param pointer to buffer array made of readings of ADC Data register
   * @param size of array
   * @param point_time_lenght sets #point_time class member
   */
  static void Process_Signal (uint32_t *buffer, uint32_t size,
			      uint32_t point_time_lenght);

  /**@brief Function used in oscilloscope configuration for edge detection.
   * @param level of ADC value (0-4095)
   * @param rising or falling trigger selection (true for rising)
   * @retval index of detected edge
   */
  static uint32_t Get_Edge_index (uint32_t level, bool is_rising);
};

/** @}*/
#endif /* WAVEFORMARYTHMETICS_HPP_ */
