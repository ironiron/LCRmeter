/*
 * LCRmath.h
 *
 *  Created on: 03.11.2019
 *      Author: Rafał
 */

#ifndef LCRMATH_HPP_
#define LCRMATH_HPP_

#include <stdint.h>

class LCR_math
{
public:
  static float reactance;
  static float loss_angle;
  static float inductance;
  static uint32_t resistance;
  static uint32_t capacity;
  static const uint32_t series_resistance=10;

  static bool Calculate (float amplitude1, float apmlitude2, float angle,
		  uint32_t frequency);

};

#endif /* LCRMATH_HPP_ */
