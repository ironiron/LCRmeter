/*
 * MCP4725.cpp
 *
 *  Created on: 02.10.2019
 *      Author: Rafał
 */

#include <MCP4725.hpp>


//Place a note about sanitazing inputs
uint32_t MCP4725::Set_Output (uint16_t value)
{
  return conn.Send_Data (address, value);
}

uint32_t MCP4725::Set_Continuous (const uint16_t * value, uint32_t lenght)
{
  return conn.Send_Data_Circular (address, value, lenght);
}

uint32_t MCP4725::Reset (void)
{
  return conn.Send_Data (address, uint16_t(1<<14));//POR bit in DAC register
}

uint32_t MCP4725::Set_Power_mode (PowerMode mode)
{
  return conn.Send_Data (address, uint16_t(mode<<12));//PD0 and PD1 bits in DAC register
  //(enum is in range of 0-3)
}
