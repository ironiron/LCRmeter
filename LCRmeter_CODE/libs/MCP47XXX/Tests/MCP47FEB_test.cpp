/*
 * MCP47_test.cpp
 *
 *  Created on: 12.05.2019
 *      Author: Rafa�
 */

#include "catch.hpp"

#include <I2C_fake.hpp>
#include <MCP47FEB.hpp>
#include "sine.hpp"

static I2C i2c;//fake
MCP47FEB dac47FEB(i2c);

//TODO adjust tests for real hardware

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
//    SECTION("MCP7425")
//    {
//      Reset_Data();
//    dac4725.Set_Output (5);
//  //before data
//    REQUIRE(i2c.data[i2c.index - 1] == 0);
//    REQUIRE(i2c.adress[i2c.index-1] == 0);
//  //data
//    REQUIRE(i2c.data[i2c.index] == 5);
//    REQUIRE(i2c.adress[i2c.index] == dac4725.address_write);
//  //no more data
//    REQUIRE(i2c.index <= 1);
//  }
//  SECTION("MCPFEB")
//  {
//    Reset_Data();
//    dac47feb.Set_Output (5);
//  //before data
//    REQUIRE(i2c.data[i2c.index - 1] == 0);
//    REQUIRE(i2c.adress[i2c.index-1] == 0);
//  //data
//    REQUIRE(i2c.data[i2c.index] == 5);
//    REQUIRE(i2c.adress[i2c.index] == dac47feb.address_write);
//    REQUIRE(i2c.memoryaddress[i2c.index] == 0x00);//dac0 address
//  //no more data
//    REQUIRE(i2c.index <= 1);
//  }
//}
//
//TEST_CASE( "set continuous value")
//{
//  //TODO check with real hardware, i don't know how it should work.
////  SECTION("MCP7425")
////  {
////  dac4725.Set_Continuous (sine_table, sine_table_lenght);
////  REQUIRE(i2c.data[i2c.index] == sine_table[sine_table_lenght-1]); //one test case before, so -1.
////  REQUIRE(i2c.adress[100] == dac4725.address_write);
////  }
//SECTION("MCPFEB")
//  {
//  Reset_Data();
//  dac47feb.Set_Continuous (sine_table, sine_table_lenght);
//  REQUIRE(i2c.data[i2c.index] == sine_table[sine_table_lenght-1]); //one test case before, so -1.
//  REQUIRE(i2c.adress[100] == dac47feb.address_write);
//  REQUIRE(i2c.memoryaddress[i2c.index] == 0x00);//dac0 address
//  }
//}


