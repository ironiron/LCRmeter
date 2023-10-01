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
  Waveform_arythmetics::hysteresis_samples=1;

  Waveform_arythmetics::filtered_buffer[0][0]=6;
  Waveform_arythmetics::filtered_buffer[0][1]=7;
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=5;
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=1;
  Waveform_arythmetics::filtered_buffer[0][8]=0;
  Waveform_arythmetics::filtered_buffer[0][9]=1;

  Waveform_arythmetics::filtered_buffer[1][0]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=2;
  Waveform_arythmetics::filtered_buffer[1][2]=3;
  Waveform_arythmetics::filtered_buffer[1][3]=6;
  Waveform_arythmetics::filtered_buffer[1][4]=7;
  Waveform_arythmetics::filtered_buffer[1][5]=9;//
  Waveform_arythmetics::filtered_buffer[1][6]=2;
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=0;
  Waveform_arythmetics::filtered_buffer[1][9]=1;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==1);
  REQUIRE(Waveform_arythmetics::nbr_of_peaks[1]==1);

  REQUIRE(Waveform_arythmetics::peaks[0][0]==2);
  REQUIRE(Waveform_arythmetics::peaks[1][0]==5);

}

TEST_CASE( "Finds more than one peak")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;


  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=6;
  Waveform_arythmetics::filtered_buffer[0][2]=7;
  Waveform_arythmetics::filtered_buffer[0][3]=8;//
  Waveform_arythmetics::filtered_buffer[0][4]=0;
  Waveform_arythmetics::filtered_buffer[0][5]=0;
  Waveform_arythmetics::filtered_buffer[0][6]=6;
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

  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==2);
  REQUIRE(Waveform_arythmetics::nbr_of_peaks[1]==1);
  REQUIRE(Waveform_arythmetics::peaks[0][0]==3);
  REQUIRE(Waveform_arythmetics::peaks[0][1]==7);
  REQUIRE(Waveform_arythmetics::peaks[1][0]==5);
  REQUIRE(Waveform_arythmetics::peaks[1][1]==0);
}

TEST_CASE( "Finds peaks 2nd scenario")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;


  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=6;
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=6;
  Waveform_arythmetics::filtered_buffer[0][6]=6;
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

  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==2);
  REQUIRE(Waveform_arythmetics::nbr_of_peaks[1]==1);
  REQUIRE(Waveform_arythmetics::peaks[0][0]==2);
  REQUIRE(Waveform_arythmetics::peaks[0][1]==7);
  REQUIRE(Waveform_arythmetics::peaks[1][0]==5);
  REQUIRE(Waveform_arythmetics::peaks[1][1]==0);
}

TEST_CASE( "Finds several peaks with different amplitude")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;


  Waveform_arythmetics::filtered_buffer[0][0]=0;
  Waveform_arythmetics::filtered_buffer[0][1]=7;
  Waveform_arythmetics::filtered_buffer[0][2]=8;//
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=4;
  Waveform_arythmetics::filtered_buffer[0][6]=6;
  Waveform_arythmetics::filtered_buffer[0][7]=7;//
  Waveform_arythmetics::filtered_buffer[0][8]=2;
  Waveform_arythmetics::filtered_buffer[0][9]=4;

  Waveform_arythmetics::filtered_buffer[1][0]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=8;
  Waveform_arythmetics::filtered_buffer[1][2]=9;//
  Waveform_arythmetics::filtered_buffer[1][3]=2;
  Waveform_arythmetics::filtered_buffer[1][4]=6;
  Waveform_arythmetics::filtered_buffer[1][5]=7;
  Waveform_arythmetics::filtered_buffer[1][6]=10;//
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=2;
  Waveform_arythmetics::filtered_buffer[1][9]=0;

  Waveform_arythmetics::Find_Peaks();


  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==2);
  REQUIRE(Waveform_arythmetics::nbr_of_peaks[1]==2);
  REQUIRE(Waveform_arythmetics::peaks[0][0]==2);
  REQUIRE(Waveform_arythmetics::peaks[0][1]==7);
  REQUIRE(Waveform_arythmetics::peaks[1][0]==2);
  REQUIRE(Waveform_arythmetics::peaks[1][1]==6);

}

TEST_CASE( "Finds one peak and ignore boundary peaks")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;


  Waveform_arythmetics::filtered_buffer[0][0]=13;//Ignore first 4 even though hysteresis = 2
  Waveform_arythmetics::filtered_buffer[0][1]=12;
  Waveform_arythmetics::filtered_buffer[0][2]=11;
  Waveform_arythmetics::filtered_buffer[0][3]=11;
  Waveform_arythmetics::filtered_buffer[0][4]=3;
  Waveform_arythmetics::filtered_buffer[0][5]=3;
  Waveform_arythmetics::filtered_buffer[0][6]=6;
  Waveform_arythmetics::filtered_buffer[0][7]=8;//OK
  Waveform_arythmetics::filtered_buffer[0][8]=4;
  Waveform_arythmetics::filtered_buffer[0][9]=0;

  Waveform_arythmetics::filtered_buffer[1][0]=0;
  Waveform_arythmetics::filtered_buffer[1][1]=6;
  Waveform_arythmetics::filtered_buffer[1][2]=6;
  Waveform_arythmetics::filtered_buffer[1][3]=10;//ok
  Waveform_arythmetics::filtered_buffer[1][4]=4;
  Waveform_arythmetics::filtered_buffer[1][5]=2;
  Waveform_arythmetics::filtered_buffer[1][6]=10;//ignore the rest 4
  Waveform_arythmetics::filtered_buffer[1][7]=11;
  Waveform_arythmetics::filtered_buffer[1][8]=12;
  Waveform_arythmetics::filtered_buffer[1][9]=13;
  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==1);
  REQUIRE(Waveform_arythmetics::nbr_of_peaks[1]==1);
  REQUIRE(Waveform_arythmetics::peaks[0][0]==7);
  REQUIRE(Waveform_arythmetics::peaks[1][0]==3);
}

#include <iostream>

TEST_CASE( "Finds one minimum and ignore boundary peaks")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;


  Waveform_arythmetics::filtered_buffer[0][0]=0;//Ignore first 4 even though hysteresis = 2
  Waveform_arythmetics::filtered_buffer[0][1]=0;
  Waveform_arythmetics::filtered_buffer[0][2]=1;
  Waveform_arythmetics::filtered_buffer[0][3]=1;
  Waveform_arythmetics::filtered_buffer[0][4]=10;
  Waveform_arythmetics::filtered_buffer[0][5]=10;
  Waveform_arythmetics::filtered_buffer[0][6]=4;
  Waveform_arythmetics::filtered_buffer[0][7]=3;//OK
  Waveform_arythmetics::filtered_buffer[0][8]=4;
  Waveform_arythmetics::filtered_buffer[0][9]=10;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_minimas[0]==1);
  REQUIRE(Waveform_arythmetics::minimas[0][0]==7);
}

//TODO add frequncy calculate function

//TODO implement this later
//TODO check how catch2 can disable tests
//TEST_CASE( "Finds one peak and ignore boundary peaks - 2nd scenario")
//{
//  Waveform_arythmetics::buffer_size=10;
//  Waveform_arythmetics::user_point_time=10;//microseconds
//  Waveform_arythmetics::mid_voltage=5;
//  Waveform_arythmetics::hysteresis_samples=3;
//
////peak undetectable, since fluctuation is too big
//  Waveform_arythmetics::filtered_buffer[0][0]=14;
//  Waveform_arythmetics::filtered_buffer[0][1]=14;
//  Waveform_arythmetics::filtered_buffer[0][2]=13;
//  Waveform_arythmetics::filtered_buffer[0][3]=13;
//  Waveform_arythmetics::filtered_buffer[0][4]=15;//not a proper peak
//  Waveform_arythmetics::filtered_buffer[0][5]=0;
//  Waveform_arythmetics::filtered_buffer[0][6]=0;
//  Waveform_arythmetics::filtered_buffer[0][7]=0;
//  Waveform_arythmetics::filtered_buffer[0][8]=0;
//  Waveform_arythmetics::filtered_buffer[0][9]=0;
//
//  Waveform_arythmetics::Find_Peaks();
//
//  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==0);
//}




TEST_CASE( "Finds one minimum")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;


  Waveform_arythmetics::filtered_buffer[0][0]=6;
  Waveform_arythmetics::filtered_buffer[0][1]=6;
  Waveform_arythmetics::filtered_buffer[0][2]=11;
  Waveform_arythmetics::filtered_buffer[0][3]=11;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=3;
  Waveform_arythmetics::filtered_buffer[0][6]=2;//ok
  Waveform_arythmetics::filtered_buffer[0][7]=8;
  Waveform_arythmetics::filtered_buffer[0][8]=9;
  Waveform_arythmetics::filtered_buffer[0][9]=10;

  Waveform_arythmetics::filtered_buffer[1][0]=8;
  Waveform_arythmetics::filtered_buffer[1][1]=6;
  Waveform_arythmetics::filtered_buffer[1][2]=6;
  Waveform_arythmetics::filtered_buffer[1][3]=6;
  Waveform_arythmetics::filtered_buffer[1][4]=1;
  Waveform_arythmetics::filtered_buffer[1][5]=2;//ok
  Waveform_arythmetics::filtered_buffer[1][6]=3;
  Waveform_arythmetics::filtered_buffer[1][7]=4;
  Waveform_arythmetics::filtered_buffer[1][8]=7;
  Waveform_arythmetics::filtered_buffer[1][9]=6;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_minimas[0]==1);
  REQUIRE(Waveform_arythmetics::nbr_of_minimas[1]==1);
  REQUIRE(Waveform_arythmetics::minimas[0][0]==6);
  REQUIRE(Waveform_arythmetics::minimas[1][0]==5);
}

TEST_CASE( "Finds series of minimas and maximas")
{
  Waveform_arythmetics::buffer_size=20;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;


  Waveform_arythmetics::filtered_buffer[0][0]=6;
  Waveform_arythmetics::filtered_buffer[0][1]=6;
  Waveform_arythmetics::filtered_buffer[0][2]=11;//ok
  Waveform_arythmetics::filtered_buffer[0][3]=1;
  Waveform_arythmetics::filtered_buffer[0][4]=4;
  Waveform_arythmetics::filtered_buffer[0][5]=3;
  Waveform_arythmetics::filtered_buffer[0][6]=2;//ok
  Waveform_arythmetics::filtered_buffer[0][7]=8;
  Waveform_arythmetics::filtered_buffer[0][8]=9;
  Waveform_arythmetics::filtered_buffer[0][9]=10;//ok
  Waveform_arythmetics::filtered_buffer[0][10]=6;
  Waveform_arythmetics::filtered_buffer[0][11]=6;
  Waveform_arythmetics::filtered_buffer[0][12]=2;
  Waveform_arythmetics::filtered_buffer[0][13]=0;//ok
  Waveform_arythmetics::filtered_buffer[0][14]=4;
  Waveform_arythmetics::filtered_buffer[0][15]=3;
  Waveform_arythmetics::filtered_buffer[0][16]=6;
  Waveform_arythmetics::filtered_buffer[0][17]=8;//ok
  Waveform_arythmetics::filtered_buffer[0][18]=0;
  Waveform_arythmetics::filtered_buffer[0][19]=0;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==3);
  REQUIRE(Waveform_arythmetics::nbr_of_minimas[0]==2);
  REQUIRE(Waveform_arythmetics::minimas[0][0]==6);
  REQUIRE(Waveform_arythmetics::minimas[0][1]==13);
  REQUIRE(Waveform_arythmetics::peaks[0][0]==2);
  REQUIRE(Waveform_arythmetics::peaks[0][1]==9);
  REQUIRE(Waveform_arythmetics::peaks[0][2]==17);
}


TEST_CASE( "Find center of peak and minimas")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;

  Waveform_arythmetics::filtered_buffer[0][0]=7;
  Waveform_arythmetics::filtered_buffer[0][1]=7;
  Waveform_arythmetics::filtered_buffer[0][2]=8;
  Waveform_arythmetics::filtered_buffer[0][3]=8;//
  Waveform_arythmetics::filtered_buffer[0][4]=8;
  Waveform_arythmetics::filtered_buffer[0][5]=4;
  Waveform_arythmetics::filtered_buffer[0][6]=2;
  Waveform_arythmetics::filtered_buffer[0][7]=2;
  Waveform_arythmetics::filtered_buffer[0][8]=2;
  Waveform_arythmetics::filtered_buffer[0][9]=2;

  Waveform_arythmetics::filtered_buffer[1][0]=4;
  Waveform_arythmetics::filtered_buffer[1][1]=4;
  Waveform_arythmetics::filtered_buffer[1][2]=3;
  Waveform_arythmetics::filtered_buffer[1][3]=3;
  Waveform_arythmetics::filtered_buffer[1][4]=1;
  Waveform_arythmetics::filtered_buffer[1][5]=1;//
  Waveform_arythmetics::filtered_buffer[1][6]=1;
  Waveform_arythmetics::filtered_buffer[1][7]=1;
  Waveform_arythmetics::filtered_buffer[1][8]=12;
  Waveform_arythmetics::filtered_buffer[1][9]=20;

  Waveform_arythmetics::Find_Peaks();
//TODO this is strange!!!!!!!!!!!!!!!! why minimas is OK even though I haven't coded that yet!!
  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==1);
  REQUIRE(Waveform_arythmetics::nbr_of_minimas[1]==1);
  REQUIRE(Waveform_arythmetics::peaks[0][0]==3);
  REQUIRE(Waveform_arythmetics::minimas[1][0]==5);
}

TEST_CASE( "Find center of peak that is close to boundary condition")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=3;

  Waveform_arythmetics::filtered_buffer[0][0]=7;
  Waveform_arythmetics::filtered_buffer[0][1]=7;
  Waveform_arythmetics::filtered_buffer[0][2]=7;
  Waveform_arythmetics::filtered_buffer[0][3]=7;
  Waveform_arythmetics::filtered_buffer[0][4]=7;
  Waveform_arythmetics::filtered_buffer[0][5]=11;
  Waveform_arythmetics::filtered_buffer[0][6]=11;//
  Waveform_arythmetics::filtered_buffer[0][7]=11;
  Waveform_arythmetics::filtered_buffer[0][8]=10;
  Waveform_arythmetics::filtered_buffer[0][9]=8;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_peaks[0]==1);
  REQUIRE(Waveform_arythmetics::peaks[0][0]==6);
}

TEST_CASE( "Find center of minimum that is close to boundary condition")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=3;

  Waveform_arythmetics::filtered_buffer[0][0]=4;
  Waveform_arythmetics::filtered_buffer[0][1]=3;
  Waveform_arythmetics::filtered_buffer[0][2]=3;
  Waveform_arythmetics::filtered_buffer[0][3]=2;
  Waveform_arythmetics::filtered_buffer[0][4]=2;
  Waveform_arythmetics::filtered_buffer[0][5]=0;
  Waveform_arythmetics::filtered_buffer[0][6]=0;//
  Waveform_arythmetics::filtered_buffer[0][7]=0;
  Waveform_arythmetics::filtered_buffer[0][8]=1;
  Waveform_arythmetics::filtered_buffer[0][9]=2;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_minimas[0]==1);
  REQUIRE(Waveform_arythmetics::minimas[0][0]==6);
}

TEST_CASE( "ignore minimas and peak with no clear distinction.")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::user_point_time=10;//microseconds
  Waveform_arythmetics::mid_voltage=5;
  Waveform_arythmetics::hysteresis_samples=1;

  Waveform_arythmetics::filtered_buffer[0][0]=7;
  Waveform_arythmetics::filtered_buffer[0][1]=7;
  Waveform_arythmetics::filtered_buffer[0][2]=8;
  Waveform_arythmetics::filtered_buffer[0][3]=8;
  Waveform_arythmetics::filtered_buffer[0][4]=8;
  Waveform_arythmetics::filtered_buffer[0][5]=4;
  Waveform_arythmetics::filtered_buffer[0][6]=2;//ignore minima below
  Waveform_arythmetics::filtered_buffer[0][7]=2;
  Waveform_arythmetics::filtered_buffer[0][8]=2;
  Waveform_arythmetics::filtered_buffer[0][9]=2;

  Waveform_arythmetics::filtered_buffer[1][0]=4;
  Waveform_arythmetics::filtered_buffer[1][1]=4;
  Waveform_arythmetics::filtered_buffer[1][2]=3;
  Waveform_arythmetics::filtered_buffer[1][3]=3;
  Waveform_arythmetics::filtered_buffer[1][4]=1;
  Waveform_arythmetics::filtered_buffer[1][5]=10;//ignore peaks below
  Waveform_arythmetics::filtered_buffer[1][6]=10;
  Waveform_arythmetics::filtered_buffer[1][7]=10;
  Waveform_arythmetics::filtered_buffer[1][8]=10;
  Waveform_arythmetics::filtered_buffer[1][9]=10;

  Waveform_arythmetics::Find_Peaks();

  REQUIRE(Waveform_arythmetics::nbr_of_peaks[1]==0);
  REQUIRE(Waveform_arythmetics::nbr_of_minimas[0]==0);
}



TEST_CASE( "Gets frequency")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds

  Waveform_arythmetics::nbr_of_peaks[0]=2;
  Waveform_arythmetics::peaks[0][0]=2;
  Waveform_arythmetics::peaks[0][1]=7;

  Waveform_arythmetics::Calc_Frequency();

  REQUIRE(Waveform_arythmetics::frequency==20000);

}


TEST_CASE( "Gets frequency from minimas")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds

  Waveform_arythmetics::nbr_of_peaks[0]=0;
  Waveform_arythmetics::nbr_of_minimas[0]=2;
  Waveform_arythmetics::peaks[0][0]=0;
  Waveform_arythmetics::peaks[0][1]=0;
  Waveform_arythmetics::minimas[0][0]=2;
  Waveform_arythmetics::minimas[0][1]=7;

  Waveform_arythmetics::Calc_Frequency();

  REQUIRE(Waveform_arythmetics::frequency==20000);

}

TEST_CASE( "Gets frequency from one peak and one minimum")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds

  Waveform_arythmetics::nbr_of_peaks[0]=1;
  Waveform_arythmetics::nbr_of_minimas[0]=1;
  Waveform_arythmetics::peaks[0][0]=2;
  Waveform_arythmetics::peaks[0][1]=0;
  Waveform_arythmetics::minimas[0][0]=7;
  Waveform_arythmetics::minimas[0][1]=0;

  Waveform_arythmetics::Calc_Frequency();

  REQUIRE(Waveform_arythmetics::frequency==10000);

}

TEST_CASE( "gets 0 frequncy if cannot detect")
{
  Waveform_arythmetics::buffer_size=10;
  Waveform_arythmetics::point_time=10;//microseconds

  Waveform_arythmetics::nbr_of_peaks[0]=1;
  Waveform_arythmetics::nbr_of_minimas[0]=0;
  Waveform_arythmetics::peaks[0][0]=2;
  Waveform_arythmetics::peaks[0][1]=0;
  Waveform_arythmetics::minimas[0][0]=7;
  Waveform_arythmetics::minimas[0][1]=0;

  Waveform_arythmetics::Calc_Frequency();

  REQUIRE(Waveform_arythmetics::frequency==0);

}

TEST_CASE( "Calculate phase swift")
{
  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=10;

  Waveform_arythmetics::nbr_of_peaks[0]=1;
  Waveform_arythmetics::nbr_of_peaks[1]=1;
  Waveform_arythmetics::nbr_of_minimas[0]=0;
  Waveform_arythmetics::nbr_of_minimas[1]=0;
  Waveform_arythmetics::peaks[0][0]=100;
  Waveform_arythmetics::peaks[1][0]=105;//Initial values: 10 points per full period(100us).
  Waveform_arythmetics::peaks[0][1]=0;
  Waveform_arythmetics::peaks[1][1]=0;

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE((Waveform_arythmetics::alfa==180000 || Waveform_arythmetics::alfa==-180000));

  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=1;
  Waveform_arythmetics::peaks[0][0]=132;
  Waveform_arythmetics::peaks[1][0]=100;//Initial values: 100 points per full period(100us).

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==(115200));
}

TEST_CASE( "Calculate phase swift and decide sign (+/-)")
{
  Waveform_arythmetics::frequency=10000;
  Waveform_arythmetics::point_time=10;
  Waveform_arythmetics::mid_voltage=5;

  Waveform_arythmetics::peaks[0][0]=0;
  Waveform_arythmetics::peaks[1][0]=2;
  Waveform_arythmetics::peaks[0][1]=10;
  Waveform_arythmetics::peaks[1][1]=12;

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE((Waveform_arythmetics::alfa==72000 ||Waveform_arythmetics::alfa==-72000));

  Waveform_arythmetics::peaks[0][0]=0;
  Waveform_arythmetics::peaks[1][0]=8;
  Waveform_arythmetics::peaks[0][1]=10;
  Waveform_arythmetics::peaks[1][1]=18;

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==(-72000));
}

TEST_CASE( "Calculate phase swift on the boundary conditions")
{
  Waveform_arythmetics::frequency=100000;
  Waveform_arythmetics::point_time=1;
  Waveform_arythmetics::mid_voltage=5;

  //Initial values: 10 points per full period(1us).
  Waveform_arythmetics::peaks[0][0]=10;
  Waveform_arythmetics::peaks[1][0]=14;
  Waveform_arythmetics::Calc_Alfa();
  REQUIRE(Waveform_arythmetics::alfa==144000);

  Waveform_arythmetics::peaks[0][0]=10;
  Waveform_arythmetics::peaks[1][0]=11;
  Waveform_arythmetics::Calc_Alfa();
  REQUIRE(Waveform_arythmetics::alfa==36000);

  Waveform_arythmetics::peaks[0][0]=10;
  Waveform_arythmetics::peaks[1][0]=16;
  Waveform_arythmetics::Calc_Alfa();
  REQUIRE(Waveform_arythmetics::alfa==-144000);

  Waveform_arythmetics::peaks[0][0]=0;
  Waveform_arythmetics::peaks[1][0]=8;
  Waveform_arythmetics::peaks[0][1]=10;
  Waveform_arythmetics::peaks[1][1]=18;

  Waveform_arythmetics::Calc_Alfa();

  REQUIRE(Waveform_arythmetics::alfa==(-72000));
}

TEST_CASE( "Calculate Amplitude from 2 peaks")
{
  Waveform_arythmetics::peaks[0][0]=2;
  Waveform_arythmetics::peaks[1][0]=8;
  Waveform_arythmetics::minimas[0][0]=5;
  Waveform_arythmetics::minimas[1][0]=4;

  Waveform_arythmetics::filtered_buffer[0][2]=8;
  Waveform_arythmetics::filtered_buffer[0][5]=2;

  Waveform_arythmetics::filtered_buffer[1][4]=1;//
  Waveform_arythmetics::filtered_buffer[1][8]=6;//

  Waveform_arythmetics::Calc_Amplitude();

  REQUIRE(Waveform_arythmetics::amplitude1==3);
  REQUIRE(Waveform_arythmetics::amplitude2==2);
}

//TODO test case with real values or at least with feasible imaginary and check if inductance etc is OK

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
//
//
//TEST_CASE( "Checks everything for 0 value case")
//{
////TODO in the future
//
//}
