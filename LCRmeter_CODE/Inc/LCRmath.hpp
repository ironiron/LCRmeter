/**
 ******************************************************************************
 * @file    LCRmath.hpp
 * @author  Rafał Mazurkiewicz
 * @date    03.11.2019
 * @brief   Calculates LCR values across device leads
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#ifndef LCRMATH_HPP_
#define LCRMATH_HPP_

#include <stdint.h>

/** \defgroup  LCR_math LCR math
 * @brief Does all the stuff with math
 *  @{
 */

/**
 *
 *
 * This class is made for calculating values of inductance, capacitance and ESR.
 * It is made solely for LCRmeter purposes. Should be used after \ref Waveform
 *
 * @warning Before using make sure #series_resistance contain proper value
 *
 * After calculating, One can read results from static class members.
 *
 * \note No getters nor setters provided! Make sure nothing writes data
 *  to class members.
 * \note This class is made solely for LCRmeter project!
 */

class LCR_math
{
public:
  static double reactance;
  static double loss_angle;
  static double inductance;
  static double resistance;
  static double capacitance;
  static double series_resistance;
  static constexpr double pi = 3.14159;

  /**@brief Do all the math stuff.
   * @param amplitude1 -amplitude across leads+series resistor
   * @param amplitude2 -amplitude across leads
   * @param angle -phase swift angle in degrees
   * @param frequency -frequency of sine wave
   */
  static bool Calculate (double amplitude1, double apmlitude2, double angle,
			 uint32_t frequency);

  /**@brief simple converter.
   * @param degrees
   * @retval radians
   */
  static double Deg_to_Rad (double degrees)
  {
    return degrees * pi / 180.0;
  }
  /**@brief simple converter.
   * @param radians
   * @retval degrees
   */
  static double Rad_to_Deg (double radians)
  {
    return radians * 180.0 / 3.14159;
  }
};
/** @}*/
#endif /* LCRMATH_HPP_ */
