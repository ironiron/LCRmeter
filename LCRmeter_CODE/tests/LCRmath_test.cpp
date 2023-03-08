/**
 ******************************************************************************
 * @file    LCRmath_test.hpp
 * @author  Rafał Mazurkiewicz
 * @date    03.11.2019
 * @brief   Calculates LCR values across device leads
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#include <LCRmath.hpp>
#include <catch.hpp>
#include <testing.hpp>
#include <stdint.h>
#include <cmath>

TEST_CASE( "calculate values for induction load")
{
//  float amplitude1=1;
//  float amplitude2=0.5;
//  float phase_swift=20;
//  uint32_t frequency=10000;
//
//  LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);
//  REQUIRE(LCR_math::loss_angle<0.93);
//  REQUIRE(LCR_math::loss_angle>0.9);
  double amplitude1=1.717;
  double amplitude2=0.994;
  double phase_swift=-39.92;
  uint32_t frequency=15000;
  LCR_math::series_resistance=10;
  bool retvalue;

  retvalue=LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);

  REQUIRE(retvalue==true);
  REQUIRE(LCR_math::loss_angle<16.35);
  REQUIRE(LCR_math::loss_angle>16.3);
  REQUIRE(LCR_math::reactance<8.35);
  REQUIRE(LCR_math::reactance>8.25);
  REQUIRE(LCR_math::resistance<2.5);
  REQUIRE(LCR_math::resistance>2.4);
  REQUIRE(LCR_math::inductance<0.000089);
  REQUIRE(LCR_math::inductance>0.000088);
}

TEST_CASE( "calculate values for capacitive load")
{
  double amplitude1=0.417;
  double amplitude2=0.291;
  double phase_swift=33.18;
  uint32_t frequency=700;
  LCR_math::series_resistance=2.2;
  bool retvalue;

  retvalue=LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);

  REQUIRE(retvalue==false);
  REQUIRE(LCR_math::loss_angle<14.3);
  REQUIRE(LCR_math::loss_angle>14.2);
  REQUIRE(LCR_math::reactance<2.7);
  REQUIRE(LCR_math::reactance>2.6);
  REQUIRE(LCR_math::resistance<0.7);
  REQUIRE(LCR_math::resistance>0.6);
  REQUIRE(LCR_math::capacitance<0.000087);
  REQUIRE(LCR_math::capacitance>0.000086);
}
