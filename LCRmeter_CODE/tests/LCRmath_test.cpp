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

TEST_CASE( "calculate values for induction load - internet example")
{
  double amplitude1=1.717;
  double amplitude2=0.994;
  double phase_swift=-39.92;
  uint32_t frequency=15000;
  LCR_math::series_resistance=10;

  LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);

  REQUIRE(LCR_math::loss_angle<16.35);
  REQUIRE(LCR_math::loss_angle>16.3);
  REQUIRE(LCR_math::resistance<2.5);
  REQUIRE(LCR_math::resistance>2.4);
  REQUIRE(LCR_math::inductance<0.000089);
  REQUIRE(LCR_math::inductance>0.000088);
}


TEST_CASE( "calculate values for induction load - simulation example")
{
  double amplitude1=0.99922901;
  double amplitude2=0.34291082;
  double phase_swift=-68.8499999999515;
  uint32_t frequency=125000;
  LCR_math::series_resistance=1000;

  LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);

  REQUIRE(LCR_math::loss_angle<1.2);
  REQUIRE(LCR_math::loss_angle>1.0);
  REQUIRE(LCR_math::resistance<7.5);
  REQUIRE(LCR_math::resistance>6.5);
  REQUIRE(LCR_math::inductance<470e-6);
  REQUIRE(LCR_math::inductance>465e-6);
}

TEST_CASE( "calculate values for capacitive load - internet example")
{
  double amplitude1=0.417;
  double amplitude2=0.291;
  double phase_swift=33.18;
  uint32_t frequency=700;
  LCR_math::series_resistance=2.2;

  LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);

  REQUIRE(LCR_math::loss_angle<14.3);
  REQUIRE(LCR_math::loss_angle>14.2);
  REQUIRE(LCR_math::resistance<0.7);
  REQUIRE(LCR_math::resistance>0.6);
  REQUIRE(LCR_math::capacitance<0.000087);
  REQUIRE(LCR_math::capacitance>0.000086);
}

TEST_CASE( "calculate values for capacitive load - simulation data")
{
  double amplitude1=0.99982843;
  double amplitude2=0.71779795;
  double phase_swift=42.346116;
  uint32_t frequency=1000;
  LCR_math::series_resistance=1000;

  LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);

  REQUIRE(LCR_math::loss_angle<1.85);
  REQUIRE(LCR_math::loss_angle>1.75);
  REQUIRE(LCR_math::resistance<35);
  REQUIRE(LCR_math::resistance>30);
  REQUIRE(LCR_math::capacitance<150e-9);
  REQUIRE(LCR_math::capacitance>149e-9);
}

TEST_CASE( "Correctly recognizes load type")
{
  double amplitude1=1.0;
  double amplitude2=2.0;
  double phase_swift=12.3;
  uint32_t frequency=1000;
  LCR_math::series_resistance=1000;

  auto retvalue=LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);
  REQUIRE(retvalue ==  LCR_math::load_type_t::CAPACITIVE);

  phase_swift=-12.3;
  retvalue=LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);
  REQUIRE(retvalue ==  LCR_math::load_type_t::INDUCTIVE);

  phase_swift=0;
  retvalue=LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);
  REQUIRE(retvalue ==  LCR_math::load_type_t::RESISTIVE);
}

TEST_CASE( "Voltage is same level gives back an error")
{
  double amplitude1=1.0;
  double amplitude2=1.0;
  double phase_swift=0;
  uint32_t frequency=1000;
  LCR_math::series_resistance=1000;

  auto retvalue=LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);
  REQUIRE(retvalue ==  LCR_math::load_type_t::ERROR);
}


TEST_CASE( "calculate for pure resistive load")
{
  double amplitude1=2.0;
  double amplitude2=1.0;
  double phase_swift=0;
  uint32_t frequency=1000;
  LCR_math::series_resistance=500;

  auto retvalue=LCR_math::Calculate(amplitude1,amplitude2,phase_swift,frequency);
  REQUIRE(retvalue ==  LCR_math::load_type_t::RESISTIVE);
  REQUIRE(LCR_math::resistance<502);
  REQUIRE(LCR_math::resistance>498);
}
