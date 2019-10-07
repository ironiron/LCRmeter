/**
  ******************************************************************************
  * @file    MCP47FEB.cpp
  * @author  Rafał Mazurkiewicz
  * @date    02.10.2019
  * @brief   Class source file for DAC based on MCP47FEB chip
  ******************************************************************************
  * @attention
  * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
  ******************************************************************************
  */

#include <MCP47FEB.hpp>

uint8_t MCP47FEB::make_write (uint8_t a)
{
  return (a << 3);//refer to datasheet. Memory adress needs to be moved 3 bits
}

uint32_t MCP47FEB::Set_Output (uint16_t value)
{
  auto temp = make_write (MCP47FEB_addresses::DAC0_volatile);
  return conn.Send_Data (address, value, temp);
}

uint32_t MCP47FEB::Set_Continuous (const uint16_t * value, uint32_t lenght)
{
  auto temp = make_write (MCP47FEB_addresses::DAC0_volatile);
  return conn.Send_Data_Circular (address, value, lenght, temp);
}

uint32_t MCP47FEB::Set_Vref (Vref source)
{
  uint8_t temp = make_write (MCP47FEB_addresses::Vref_volatile);
  return conn.Send_Data (address, uint16_t (source), temp);
}

uint32_t MCP47FEB::Stop_DAC (void)
{
  conn.Stop_Transfer();
  return 0;
}
