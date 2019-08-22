/*
 * MCP47.h
 *
 *  Created on: 12.05.2019
 *      Author: Rafa³
 */

#ifndef MCP47_HPP_
#define MCP47_HPP_

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
template<bool use_MCP4725>
class MCP47
{
public:
  MCP47 (I2C & connection_port): conn(connection_port)
  {
    if (use_MCP4725 !=0 )
      {
	address_write=MCP4725_address;
      }
    else
      {
	address_write=MCP47FEB_address;
      }
  }
  enum class Vref: uint16_t{Vdd,Vref,Vref_buffered,Vint};
  void Set_Output(uint16_t voltage);
  void Set_Continuous(const uint16_t *value,uint32_t lenght);
  void Output_Enable(bool on); //latch pin low
  void Set_Vref(MCP47::Vref v);

  uint8_t address_write;
  uint8_t address_read;
  I2C & conn;//TODO check binary size
  enum device { MCP4725,MCP47FEB};

/////////////////////////////////////////////////////////////////////////////
private:
  ////////MCP47FEB
  //Make write and read are combining address(MCP47FEB) or
  //data(MCP4725) with control bits. Please refer to datasheet.
  uint8_t make_write(uint8_t a)
  {
    return (a<<3);
  }
  uint8_t make_read(uint8_t a)
  {
    return (a<<3)|(1<<2)|(1<<1);
  }
  /////// MCP4725
  uint16_t make_write(uint16_t a)
  {
    //uncomment if hardware require it
    return (a);//|(1<<14);
  }
  const uint16_t * make_write(const uint16_t *a,uint32_t lenght)//for continous write
  {
//    for (;lenght>0;)
//      {
//	lenght--;
//	//a[lenght]=(a[lenght])|(1<<14);
//      }
    return a;
  }
  uint16_t make_read(uint16_t a)
  {
    return (a<<3)|(1<<2)|(1<<1);//TODO explain this a little????
  }

  const static uint8_t MCP4725_address=0xC0;
  const static uint8_t MCP47FEB_address=0xC0;
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
};

template<bool use_MCP4725>
  void MCP47<use_MCP4725>::Set_Output (uint16_t value)
  {
  if (use_MCP4725 !=0 )//TODO make it ==true
    {
      I2C::ErrorCode e=conn.Send_Data(address_write,make_write(value));
    }
  else
    {
      auto temp=make_write(static_cast<uint8_t>(MCP47FEB_addresses::DAC0_volatile));
      conn.Send_Data(address_write,value,temp);
    }
  }

template<bool use_MCP4725>//not sure how is this supposed to work
//TODO check with real hardware
  void MCP47<use_MCP4725>::Set_Continuous (const uint16_t * value,
					    uint32_t lenght)
  {
  if (use_MCP4725 !=0 )
    {
      I2C::ErrorCode e=conn.Send_Data_Circular(address_write,make_write(value,lenght), lenght);
    }
  else
    {
      auto temp=make_write(static_cast<uint8_t>(MCP47FEB_addresses::DAC0_volatile));
      conn.Send_Data_Circular(address_write,value, lenght,temp);
    }

  }


#endif /* MCP47_HPP_ */
