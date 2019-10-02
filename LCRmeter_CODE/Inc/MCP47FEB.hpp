/*
 * MCP47.h
 *
 *  Created on: 12.05.2019
 *      Author: Rafa�
 */

/// @warning currently only 1 output is supported

#ifndef MCP47FEB_HPP_
#define MCP47FEB_HPP_

#ifdef TEST
#include <I2C_fake.hpp>
#else
#include "I2C.hpp"
#endif
#include <stdint.h>

//test
//#include <stdio.h>

//const uint8_t address=0xC0;
/**
 * Header only
 */
class MCP47FEB
{
public:
  MCP47FEB (I2C & connection_port): conn(connection_port)
  {

  }
  enum class Vref: uint16_t{Vdd,Vref,Vref_buffered,Vint};
  uint32_t Set_Output(uint16_t voltage);
  uint32_t Set_Continuous(const uint16_t *value,uint32_t lenght);
  void Output_Enable(bool on); //latch pin low
  void Set_Vref(MCP47FEB::Vref v);

  uint8_t address_write;
  uint8_t address_read;
  I2C & conn;//TODO check binary size

/////////////////////////////////////////////////////////////////////////////
private:
  ////////MCP47FEB
  //Make write and read are combining address(MCP47FEB) or
  //data(MCP4725) with control bits. Please refer to datasheet.



  const static uint8_t address=0xC0;
  enum class MCP47FEB_addresses: uint8_t
  {
     DAC0_volatile=0x00,
     DAC1_volatile=0x01,
     Vref_volatile=0x08,
     power_down_volatile=0x09,
     gain_status_volatile=0x0A,
     wiperlock_volatile=0x00,
    //////////////////////////////////////
     DAC0_nonvolatile=0x10,
     DAC1_nonvolatile=0x11,
     Vref_nonvolatile=0x18,
     power_down_nonvolatile=0x19,
     gain_status_nonvolatile=0x1A
  };

  uint8_t make_write(uint8_t a);
  uint8_t make_read(uint8_t a);
};




#endif /* MCP47FEB_HPP_ */
