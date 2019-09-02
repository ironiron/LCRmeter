/*
 * SSD1306_test.cpp
 *
 *  Created on: 22.08.2019
 *      Author: Rafa³
 */

#include <SSD1306.hpp>
#include "testing.hpp"
#include "I2C_fake.hpp"//included just
#include "catch.hpp"
#include <iostream>
#include <string>

static I2C i2c;

SSD1306 oled64(64,i2c);

TEST_CASE( "fills screen buffer within bounds")
{
  testing::ssd1306::data.clear();

  oled64.Fill(SSD1306::Color::WHITE);
  oled64.Update_Screen();

  REQUIRE(testing::ssd1306::data.size()==1030);//1024 data+ 6 bytes of commands

  REQUIRE(testing::ssd1306::data[0]==0x21);//commands
  REQUIRE(testing::ssd1306::data[1]==0);
  REQUIRE(testing::ssd1306::data[2]==127);
  REQUIRE(testing::ssd1306::data[3]==0x22);
  REQUIRE(testing::ssd1306::data[4]==0);
  REQUIRE(testing::ssd1306::data[5]==7);

  REQUIRE(testing::ssd1306::data[6]==0xff);//data
  REQUIRE(testing::ssd1306::data[50]==0xff);
  REQUIRE(testing::ssd1306::data[100]==0xff);
  REQUIRE(testing::ssd1306::data[1029]==0xff);//last data
}

TEST_CASE( "draws pixels")
{
  testing::ssd1306::data.clear();

  oled64.Clean();
  oled64.Draw_Pixel(0,0,SSD1306::Color::WHITE);
  oled64.Draw_Pixel(1,3,SSD1306::Color::WHITE);
  oled64.Update_Screen();

  REQUIRE(testing::ssd1306::data.size()==1030);//1024 data+ 6 bytes of commands

  REQUIRE(testing::ssd1306::data[6]==0x01);//data
  REQUIRE(testing::ssd1306::data[7]==0x08);
  REQUIRE(testing::ssd1306::data[8]==0);
}
