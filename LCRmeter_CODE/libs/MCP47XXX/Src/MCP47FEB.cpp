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

uint8_t MCP47FEB::make_write(uint8_t a)
{
	return (a << 3); //refer to datasheet. Memory address needs to be moved 3 bits
}

uint32_t MCP47FEB::Set_Output(uint16_t value)
{
	auto temp = make_write(MCP47FEB_addresses::DAC0_volatile);
	return conn.Send_Data(address, value, temp);
}

/// @Warning memory adress is ignored, because of I2C library. If it's fixed then remember about
/// value array input. (DAC requires |address-upper byte data-lower byte data|)
uint32_t MCP47FEB::Set_Continuous(const uint8_t * value, uint32_t lenght)
{
	//return 0;
	auto temp = make_write(MCP47FEB_addresses::DAC0_volatile);
	return conn.Send_Data_Circular(address, value, lenght, temp); //TODO why value here is 8 bit, sinewave also 8 bit with 2 additional, so now we have 3 bytes+ one DAC0 data???
}

uint32_t MCP47FEB::Set_Vref(Vref source)
{
	uint8_t temp = make_write(MCP47FEB_addresses::Vref_volatile);
  	return conn.Send_Data(address, uint16_t(source), temp);
}

uint32_t MCP47FEB::Stop_DAC(void)
{
	conn.Stop_Transfer();
	return 0;
}
