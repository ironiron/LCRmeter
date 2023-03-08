/**
  ******************************************************************************
  * @file    MCP4725.cpp
  * @author  Rafał Mazurkiewicz
  * @date    02.10.2019
  * @brief   Class source file for DAC based on MCP4725 chip
  ******************************************************************************
  * @attention
  * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
  ******************************************************************************
  */

#include <MCP4725.hpp>

uint32_t MCP4725::Set_Output (uint16_t value)
{
  return conn.Send_Data (address, value);
}

uint32_t MCP4725::Set_Continuous (const uint16_t * voltage_array, uint32_t length)
{
  return conn.Send_Data_Circular (address, voltage_array, length);
}

uint32_t MCP4725::Reset (void)
{
  return conn.Send_Data (address, uint16_t(1<<14));//POR bit in DAC register
}

uint32_t MCP4725::Set_Power_mode(PowerMode mode)
{
  //PD0 and PD1 bits in DAC register (enum is in range of 0-3)
  return conn.Send_Data(address, uint16_t(mode << 12));
}

uint32_t MCP4725::Stop_DAC (void)
{
  conn.Stop_Transfer();
  return 0;//TODO fix this hard coded 0
}
