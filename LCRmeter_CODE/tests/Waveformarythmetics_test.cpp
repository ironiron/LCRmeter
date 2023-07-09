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
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][1]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][2]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][3]==0);

  REQUIRE(Waveform_arythmetics::filtered_buffer[1][0]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[1][1]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[1][2]==6);
  REQUIRE(Waveform_arythmetics::filtered_buffer[1][3]==0);

  REQUIRE(Waveform_arythmetics::buffer_size==3);

  Waveform_arythmetics::Calc_Moving_Average(buf,10,1);

  REQUIRE(Waveform_arythmetics::filtered_buffer[0][0]==10);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][1]==5);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][2]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][3]==1);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][4]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][5]==4);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][6]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][7]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][8]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][9]==1);

  REQUIRE(Waveform_arythmetics::buffer_size==10);

  Waveform_arythmetics::Calc_Moving_Average(buf,10,2);

  REQUIRE(Waveform_arythmetics::filtered_buffer[0][0]==7);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][1]==1);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][2]==3);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][3]==2);
  REQUIRE(Waveform_arythmetics::filtered_buffer[0][4]==1);

  REQUIRE(Waveform_arythmetics::buffer_size==5);
}

TEST_CASE( "Finds peaks")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::mid_voltage=5;

  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=2;
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=5;
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=1;
  Waveform_arythmetics::filtered_buffer[0][8]=0;
  Waveform_arythmetics::filtered_buffer[0][9]=1;

  Waveform_arythmetics::filtered_buffer[1][0]=10;//skip first so get wider view.
  Waveform_arythmetics::filtered_buffer[1][1]=2;
  Waveform_arythmetics::filtered_buffer[1][2]=3;
  Waveform_arythmetics::filtered_buffer[1][3]=9;//
  Waveform_arythmetics::filtered_buffer[1][4]=4;
  Waveform_arythmetics::filtered_buffer[1][5]=5;
  Waveform_arythmetics::filtered_buffer[1][6]=2;
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=0;
  Waveform_arythmetics::filtered_buffer[1][9]=1;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==2);
  REQUIRE(Waveform_arythmetics::peak2==3);

}

TEST_CASE( "Finds other peaks and decides which to choose; gets frequency")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;


  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=2;
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=4;
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=8;//
  Waveform_arythmetics::filtered_buffer[0][8]=2;
  Waveform_arythmetics::filtered_buffer[0][9]=4;

  Waveform_arythmetics::filtered_buffer[1][0]=1;
  Waveform_arythmetics::filtered_buffer[1][1]=2;
  Waveform_arythmetics::filtered_buffer[1][2]=4;
  Waveform_arythmetics::filtered_buffer[1][3]=2;
  Waveform_arythmetics::filtered_buffer[1][4]=6;
  Waveform_arythmetics::filtered_buffer[1][5]=10;//
  Waveform_arythmetics::filtered_buffer[1][6]=2;
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=2;
  Waveform_arythmetics::filtered_buffer[1][9]=0;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==2);
  REQUIRE(Waveform_arythmetics::minor_peak1==7);
  REQUIRE(Waveform_arythmetics::peak2==5);
  REQUIRE(Waveform_arythmetics::frequency==20000);

}

TEST_CASE( "Finds several peaks with different amplitude")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;


  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=2;
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=4;
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=7;//
  Waveform_arythmetics::filtered_buffer[0][8]=2;
  Waveform_arythmetics::filtered_buffer[0][9]=4;

  Waveform_arythmetics::filtered_buffer[1][0]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=9;//
  Waveform_arythmetics::filtered_buffer[1][2]=4;
  Waveform_arythmetics::filtered_buffer[1][3]=2;
  Waveform_arythmetics::filtered_buffer[1][4]=6;
  Waveform_arythmetics::filtered_buffer[1][5]=7;
  Waveform_arythmetics::filtered_buffer[1][6]=10;//
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=2;
  Waveform_arythmetics::filtered_buffer[1][9]=0;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==2);
  REQUIRE(Waveform_arythmetics::minor_peak1==7);
  REQUIRE(Waveform_arythmetics::peak2==1);
  REQUIRE(Waveform_arythmetics::minor_peak2==6);

}

TEST_CASE( "Finds one peak and ignore boundary peaks")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;


  Waveform_arythmetics::filtered_buffer[0][0]=10;//Ignore 3 first
  Waveform_arythmetics::filtered_buffer[0][1]=9;//
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=8;//OK
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=7;//
  Waveform_arythmetics::filtered_buffer[0][8]=8;
  Waveform_arythmetics::filtered_buffer[0][9]=8;

  Waveform_arythmetics::filtered_buffer[1][0]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=9;//ignore
  Waveform_arythmetics::filtered_buffer[1][2]=4;
  Waveform_arythmetics::filtered_buffer[1][3]=2;
  Waveform_arythmetics::filtered_buffer[1][4]=6;
  Waveform_arythmetics::filtered_buffer[1][5]=7;
  Waveform_arythmetics::filtered_buffer[1][6]=10;// OK
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=2;
  Waveform_arythmetics::filtered_buffer[1][9]=0;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==5);
  REQUIRE_FALSE(Waveform_arythmetics::minor_peak1==7);
  REQUIRE(Waveform_arythmetics::peak2==6);
  REQUIRE_FALSE(Waveform_arythmetics::minor_peak2==6);

}

TEST_CASE( "Find center of peak")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;

  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=2;
  Waveform_arythmetics::filtered_buffer[0][2]=8;
  Waveform_arythmetics::filtered_buffer[0][3]=8;//
  Waveform_arythmetics::filtered_buffer[0][4]=8;
  Waveform_arythmetics::filtered_buffer[0][5]=4;
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=2;
  Waveform_arythmetics::filtered_buffer[0][8]=2;
  Waveform_arythmetics::filtered_buffer[0][9]=2;

  Waveform_arythmetics::filtered_buffer[1][0]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=1;
  Waveform_arythmetics::filtered_buffer[1][2]=2;
  Waveform_arythmetics::filtered_buffer[1][3]=2;
  Waveform_arythmetics::filtered_buffer[1][4]=10;
  Waveform_arythmetics::filtered_buffer[1][5]=10;//
  Waveform_arythmetics::filtered_buffer[1][6]=10;
  Waveform_arythmetics::filtered_buffer[1][7]=10;
  Waveform_arythmetics::filtered_buffer[1][8]=2;
  Waveform_arythmetics::filtered_buffer[1][9]=0;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::peak1==3);
  REQUIRE(Waveform_arythmetics::minor_peak1==0);
  REQUIRE(Waveform_arythmetics::peak2==5);
  REQUIRE(Waveform_arythmetics::minor_peak2==0);
}

TEST_CASE( "Calculate phase swift")
{
  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=10;
  Waveform_arythmetics::peak1=100;
  Waveform_arythmetics::peak2=105;//Initial values: 10 points per full period(100us).
  Waveform_arythmetics::minor_peak1=0;
  Waveform_arythmetics::minor_peak2=0;

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==180000);

  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=1;
  Waveform_arythmetics::peak1=132;
  Waveform_arythmetics::peak2=100;//Initial values: 100 points per full period(100us).

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==(-115200));
}

TEST_CASE( "Calculate phase swift and decide sign (+/-)")
{
  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=10;
  Waveform_arythmetics::mid_voltage=5;

  Waveform_arythmetics::peak1=0;
  Waveform_arythmetics::peak2=2;
  Waveform_arythmetics::minor_peak1=10;
  Waveform_arythmetics::minor_peak2=12;

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==(72000));

  Waveform_arythmetics::peak1=0;
  Waveform_arythmetics::peak2=8;
  Waveform_arythmetics::minor_peak1=10;
  Waveform_arythmetics::minor_peak2=18;

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==(-72000));
}

TEST_CASE( "Calculate Amplitude")
{
  Waveform_arythmetics::peak1=2;
  Waveform_arythmetics::minor_peak1=7;
  Waveform_arythmetics::peak2=1;
  Waveform_arythmetics::minor_peak2=6;

  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=2;
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=4;
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=8;//
  Waveform_arythmetics::filtered_buffer[0][8]=2;
  Waveform_arythmetics::filtered_buffer[0][9]=4;

  Waveform_arythmetics::filtered_buffer[1][0]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=10;//
  Waveform_arythmetics::filtered_buffer[1][2]=4;
  Waveform_arythmetics::filtered_buffer[1][3]=2;
  Waveform_arythmetics::filtered_buffer[1][4]=6;
  Waveform_arythmetics::filtered_buffer[1][5]=7;
  Waveform_arythmetics::filtered_buffer[1][6]=10;//
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=2;
  Waveform_arythmetics::filtered_buffer[1][9]=0;

  Waveform_arythmetics::Calc_Amplitude();

  REQUIRE(Waveform_arythmetics::amplitude1==3);
  REQUIRE(Waveform_arythmetics::amplitude2==4);

}

TEST_CASE( "Gets index of rising edge for oscilloscope")
{
  Waveform_arythmetics::filtered_buffer[0][0]=8;
  Waveform_arythmetics::filtered_buffer[0][1]=5;
  Waveform_arythmetics::filtered_buffer[0][2]=3;
  Waveform_arythmetics::filtered_buffer[0][3]=7;
  Waveform_arythmetics::filtered_buffer[0][4]=3;
  Waveform_arythmetics::filtered_buffer[0][5]=9;
  Waveform_arythmetics::filtered_buffer[0][6]=3;
  Waveform_arythmetics::filtered_buffer[0][7]=3;
  Waveform_arythmetics::filtered_buffer[0][8]=5;
  Waveform_arythmetics::filtered_buffer[0][9]=5;

  uint32_t retval=0;
  retval=Waveform_arythmetics::Get_Edge_index(5,true);

  REQUIRE(retval==3);
}

TEST_CASE( "Gets index of falling edge for oscilloscope")
{
  Waveform_arythmetics::filtered_buffer[0][0]=8;
  Waveform_arythmetics::filtered_buffer[0][1]=5;
  Waveform_arythmetics::filtered_buffer[0][2]=3;
  Waveform_arythmetics::filtered_buffer[0][3]=7;
  Waveform_arythmetics::filtered_buffer[0][4]=3;
  Waveform_arythmetics::filtered_buffer[0][5]=9;
  Waveform_arythmetics::filtered_buffer[0][6]=3;
  Waveform_arythmetics::filtered_buffer[0][7]=3;
  Waveform_arythmetics::filtered_buffer[0][8]=5;
  Waveform_arythmetics::filtered_buffer[0][9]=5;

  uint32_t retval=0;
  retval=Waveform_arythmetics::Get_Edge_index(5,false);

  REQUIRE(retval==2);
}

TEST_CASE( "Gets index of rising edge for oscilloscope. PWM input starting at 0 time")
{
  Waveform_arythmetics::filtered_buffer[0][0]=10;
  Waveform_arythmetics::filtered_buffer[0][1]=10;
  Waveform_arythmetics::filtered_buffer[0][2]=10;
  Waveform_arythmetics::filtered_buffer[0][3]=0;
  Waveform_arythmetics::filtered_buffer[0][4]=0;
  Waveform_arythmetics::filtered_buffer[0][5]=0;
  Waveform_arythmetics::filtered_buffer[0][6]=0;
  Waveform_arythmetics::filtered_buffer[0][7]=0;
  Waveform_arythmetics::filtered_buffer[0][8]=10;
  Waveform_arythmetics::filtered_buffer[0][9]=10;

  uint32_t retval=0;
  retval=Waveform_arythmetics::Get_Edge_index(5,true);

  REQUIRE(retval==8);
}


TEST_CASE( "Checks everything for 0 value case")
{
//TODO in the future

}
