/*
 * SSD1306_hardware.cpp
 *
 *  Created on: 25.08.2019
 *      Author: Rafa³
 */

#include <SSD1306.hpp>
#include "delay.h"


void SSD1306::delay (uint32_t miliseconds)
{
  delay_ms(miliseconds);
}

void SSD1306::Write_Command (uint8_t com)
{

}

void SSD1306::Write_Data (std::array<uint8_t, SSD1306::buffer_size>  &data)
{
}

void SSD1306::Reset (void)
{
  //in I2C-empty instruction.
  //in SPI use:
  //TODO finish it
}
