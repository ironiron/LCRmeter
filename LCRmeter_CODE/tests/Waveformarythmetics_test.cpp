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
#include "sine.hpp"


TEST_CASE( "calculating moving mean")
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

  REQUIRE(Waveform_arythmetics::buffer_size==3);

  Waveform_arythmetics::Calc_Moving_Average(buf,10,1);

  REQUIRE(Waveform_arythmetics::filtered_buffer[0][0]==10);
  REQUIRE(Waveform_arythmetics::filtered_buffer[1][0]==5);
  REQUIRE(Waveform_arythmetics::filtered_buffer[2][0]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[3][0]==1);
  REQUIRE(Waveform_arythmetics::filtered_buffer[4][0]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[5][0]==4);
  REQUIRE(Waveform_arythmetics::filtered_buffer[6][0]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[7][0]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[8][0]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[9][0]==1);

  REQUIRE(Waveform_arythmetics::buffer_size==10);

  Waveform_arythmetics::Calc_Moving_Average(buf,10,2);

  REQUIRE(Waveform_arythmetics::filtered_buffer[0][0]==7);
  REQUIRE(Waveform_arythmetics::filtered_buffer[1][0]==1);
  REQUIRE(Waveform_arythmetics::filtered_buffer[2][0]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[3][0]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[4][0]==1);

  REQUIRE(Waveform_arythmetics::buffer_size==5);
}

TEST_CASE( "Finds peaks")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::mid_voltage=5;

  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[1][0]=2;
  Waveform_arythmetics::filtered_buffer[2][0]=8;//
  Waveform_arythmetics::filtered_buffer[3][0]=2;
  Waveform_arythmetics::filtered_buffer[4][0]=4;
  Waveform_arythmetics::filtered_buffer[5][0]=5;
  Waveform_arythmetics::filtered_buffer[6][0]=2;
  Waveform_arythmetics::filtered_buffer[7][0]=1;
  Waveform_arythmetics::filtered_buffer[8][0]=0;
  Waveform_arythmetics::filtered_buffer[9][0]=1;

  Waveform_arythmetics::filtered_buffer[0][1]=10;//
  Waveform_arythmetics::filtered_buffer[1][1]=2;
  Waveform_arythmetics::filtered_buffer[2][1]=8;
  Waveform_arythmetics::filtered_buffer[3][1]=2;
  Waveform_arythmetics::filtered_buffer[4][1]=4;
  Waveform_arythmetics::filtered_buffer[5][1]=5;
  Waveform_arythmetics::filtered_buffer[6][1]=2;
  Waveform_arythmetics::filtered_buffer[7][1]=1;
  Waveform_arythmetics::filtered_buffer[8][1]=0;
  Waveform_arythmetics::filtered_buffer[9][1]=1;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==2);
  REQUIRE(Waveform_arythmetics::peak2==0);

}

TEST_CASE( "Finds other peaks and decides which to choose; gets frequency")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;


  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[1][0]=2;
  Waveform_arythmetics::filtered_buffer[2][0]=8;//
  Waveform_arythmetics::filtered_buffer[3][0]=2;
  Waveform_arythmetics::filtered_buffer[4][0]=4;
  Waveform_arythmetics::filtered_buffer[5][0]=4;
  Waveform_arythmetics::filtered_buffer[6][0]=2;
  Waveform_arythmetics::filtered_buffer[7][0]=8;//
  Waveform_arythmetics::filtered_buffer[8][0]=2;
  Waveform_arythmetics::filtered_buffer[9][0]=4;

  Waveform_arythmetics::filtered_buffer[0][1]=1;
  Waveform_arythmetics::filtered_buffer[1][1]=2;
  Waveform_arythmetics::filtered_buffer[2][1]=4;
  Waveform_arythmetics::filtered_buffer[3][1]=2;
  Waveform_arythmetics::filtered_buffer[4][1]=6;
  Waveform_arythmetics::filtered_buffer[5][1]=10;//
  Waveform_arythmetics::filtered_buffer[6][1]=2;
  Waveform_arythmetics::filtered_buffer[7][1]=1;
  Waveform_arythmetics::filtered_buffer[8][1]=2;
  Waveform_arythmetics::filtered_buffer[9][1]=0;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==2);
  REQUIRE(Waveform_arythmetics::minor_peak1==7);
  REQUIRE(Waveform_arythmetics::peak2==5);
  REQUIRE(Waveform_arythmetics::frequency==20000);

}

TEST_CASE( "Finds several peaks with different amplitude")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;


  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[1][0]=2;
  Waveform_arythmetics::filtered_buffer[2][0]=8;//
  Waveform_arythmetics::filtered_buffer[3][0]=2;
  Waveform_arythmetics::filtered_buffer[4][0]=4;
  Waveform_arythmetics::filtered_buffer[5][0]=4;
  Waveform_arythmetics::filtered_buffer[6][0]=2;
  Waveform_arythmetics::filtered_buffer[7][0]=7;//
  Waveform_arythmetics::filtered_buffer[8][0]=2;
  Waveform_arythmetics::filtered_buffer[9][0]=4;

  Waveform_arythmetics::filtered_buffer[0][1]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=9;//
  Waveform_arythmetics::filtered_buffer[2][1]=4;
  Waveform_arythmetics::filtered_buffer[3][1]=2;
  Waveform_arythmetics::filtered_buffer[4][1]=6;
  Waveform_arythmetics::filtered_buffer[5][1]=7;
  Waveform_arythmetics::filtered_buffer[6][1]=10;//
  Waveform_arythmetics::filtered_buffer[7][1]=1;
  Waveform_arythmetics::filtered_buffer[8][1]=2;
  Waveform_arythmetics::filtered_buffer[9][1]=0;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==2);
  REQUIRE(Waveform_arythmetics::minor_peak1==7);
  REQUIRE(Waveform_arythmetics::peak2==1);
  REQUIRE(Waveform_arythmetics::minor_peak2==6);

}

TEST_CASE( "Calculate phase swift")
{
  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=10;
  Waveform_arythmetics::peak1=100;
  Waveform_arythmetics::peak2=105;//Initial values: 10 points per full period(100us).

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==180000);

  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=1;
  Waveform_arythmetics::peak1=132;
  Waveform_arythmetics::peak2=100;//Initial values: 100 points per full period(100us).

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==(-115200));
}

TEST_CASE( "Calculate Amplitude")
{
  Waveform_arythmetics::peak1=2;
  Waveform_arythmetics::minor_peak1=7;
  Waveform_arythmetics::peak2=1;
  Waveform_arythmetics::minor_peak2=6;

  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[1][0]=2;
  Waveform_arythmetics::filtered_buffer[2][0]=8;//
  Waveform_arythmetics::filtered_buffer[3][0]=2;
  Waveform_arythmetics::filtered_buffer[4][0]=4;
  Waveform_arythmetics::filtered_buffer[5][0]=4;
  Waveform_arythmetics::filtered_buffer[6][0]=2;
  Waveform_arythmetics::filtered_buffer[7][0]=8;//
  Waveform_arythmetics::filtered_buffer[8][0]=2;
  Waveform_arythmetics::filtered_buffer[9][0]=4;

  Waveform_arythmetics::filtered_buffer[0][1]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=10;//
  Waveform_arythmetics::filtered_buffer[2][1]=4;
  Waveform_arythmetics::filtered_buffer[3][1]=2;
  Waveform_arythmetics::filtered_buffer[4][1]=6;
  Waveform_arythmetics::filtered_buffer[5][1]=7;
  Waveform_arythmetics::filtered_buffer[6][1]=10;//
  Waveform_arythmetics::filtered_buffer[7][1]=1;
  Waveform_arythmetics::filtered_buffer[8][1]=2;
  Waveform_arythmetics::filtered_buffer[9][1]=0;

  Waveform_arythmetics::Calc_Amplitude();

  REQUIRE(Waveform_arythmetics::amplitude1==3);
  REQUIRE(Waveform_arythmetics::amplitude2==4);

}

