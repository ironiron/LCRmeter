/*
 * MCP47_test.cpp
 *
 *  Created on: 12.05.2019
 *      Author: Rafa�
 */

#include "catch.hpp"

#include <I2C_fake.hpp>
#include <MCP4725.hpp>
#include "sine.hpp"

static I2C i2c;//fake
MCP4725 dac4725(i2c);
//
//This tests needs to be rewritten: update testing namespace, get rid of i2C fake,write better tests
//
//void Reset_Data(void)
//{
//  i2c.data.clear();
//  i2c.adress.clear();
//  i2c.memoryaddress.clear();
//  i2c.index=0;
//  //Initial value
//  i2c.data.push_back (0);
//  i2c.adress.push_back (0);
//  i2c.memoryaddress.push_back (0);
//}
//
//TEST_CASE( "set output value, and checks if no more data is written")
//{
//
//      Reset_Data();
//    dac.Set_Output (50);
//  //before data
//    REQUIRE(i2c.data[i2c.index - 1] == 0);
//    REQUIRE(i2c.adress[i2c.index-1] == 0);
//  //data
//    REQUIRE(i2c.data[i2c.index] == 50);
//    REQUIRE(i2c.adress[i2c.index] == 0xc0);//address of chip
//  //no more data
//    REQUIRE(i2c.index <= 1);
//
//}
//
//TEST_CASE( "set continuous value")
//{
//  Reset_Data();
//  //before data
//    REQUIRE(i2c.data[0] == 0);
//    REQUIRE(i2c.adress[0] == 0);
//  //data
//    REQUIRE(i2c.data[i2c.index] == 50);
//    REQUIRE(i2c.adress[i2c.index] == 0xc0);//address of chip
//  //no more data
//    REQUIRE(i2c.index <= 1);
//  dac.Set_Continuous (sine_table, sine_table_lenght);
//  REQUIRE(i2c.data[i2c.index] == sine_table[sine_table_lenght-1]); //one test case before, so -1.
//  REQUIRE(i2c.adress[100] == 0);
//}


