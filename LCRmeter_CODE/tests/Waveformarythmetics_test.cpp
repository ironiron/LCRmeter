/**
 ******************************************************************************
 * @file    Waveformarythmetics_test.cpp
 * @author  Rafał Mazurkiewicz
 * @date    09.10.2019
 * @brief   Signal conditioning for LCRmeter's ADC purposes
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#include <Waveformarythmetics.hpp>

#include <catch.hpp>
#include <testing.hpp>
#include <stdint.h>

TEST_CASE( "calculating mean average")
{
  uint32_t buf[10]={10 | (3<<16),
		      5 | (5<<16),
		      2 | (2<<16),
		      1 | (3<<16),
		      3 | (3<<16),
		      4 | (3<<16),
		      2 | (4<<16),
		      2 | (9<<16),
		      2 | (5<<16),
		      1 | (3<<16)};

  Waveform_arythmetics::Calc_Moving_Average(buf,10,3);

  REQUIRE(Waveform_arythmetics::filtered_buffer[0][0]==5);
  REQUIRE(Waveform_arythmetics::filtered_buffer[1][0]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[2][0]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[3][0]==0);

  REQUIRE(Waveform_arythmetics::filtered_buffer[0][1]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[1][1]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[2][1]==6);
  REQUIRE(Waveform_arythmetics::filtered_buffer[3][1]==0);
}
