/*
 * SSD1306_hardware.cpp
 *
 *  Created on: 25.08.2019
 *      Author: Rafa³
 */

#include <SSD1306.hpp>
#include <vector>

namespace testing {
  std::vector<int>  data;
}

//TODO handle return values

void SSD1306::delay (uint32_t miliseconds)
{
}

void SSD1306::Write_Command (uint8_t com)
{
}

void SSD1306::Write_Data (std::array<uint8_t, SSD1306::buffer_size>  &data)
{
  testing::data.insert(testing::data.end(), data.begin(), data.end());
}

void SSD1306::Reset (void)
{
}
