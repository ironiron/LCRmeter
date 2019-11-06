/*
 * LCRmath_test.cpp
 *
 *  Created on: 03.11.2019
 *      Author: Rafał
 */

#include <LCRmath.hpp>
#include <catch.hpp>
#include <testing.hpp>
#include <stdint.h>
#include <cmath>

TEST_CASE( "calculate value of loss angle")
{
//  float amplitude1=1;
//  float amplitude2=0.5;
//  float phase_swift=20;
//  uint32_t frequency=10000;
//
//  LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);
//  REQUIRE(LCR_math::loss_angle<0.93);
//  REQUIRE(LCR_math::loss_angle>0.9);
  float amplitude1=1.717;
  float amplitude2=0.994;
  float phase_swift=-39.92;
  uint32_t frequency=15000;

  LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);

  REQUIRE(LCR_math::inductance<1.15);
  REQUIRE(LCR_math::inductance>1.1);
  REQUIRE(LCR_math::loss_angle<16.35);
  REQUIRE(LCR_math::loss_angle>16.3);
  REQUIRE(LCR_math::reactance<8.35);
  REQUIRE(LCR_math::reactance>8.25);
}
