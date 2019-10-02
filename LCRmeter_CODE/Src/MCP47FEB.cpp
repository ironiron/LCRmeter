/*
 * MCP47FEB.cpp
 *
 *  Created on: 02.10.2019
 *      Author: Rafał
 */

#include <MCP47FEB.hpp>

  uint8_t MCP47FEB::make_write(uint8_t a)
  {
    return (a<<3);
  }
  uint8_t MCP47FEB::make_read(uint8_t a)
  {
    return (a<<3)|(1<<2)|(1<<1);
  }

uint32_t MCP47FEB::Set_Output (uint16_t value)
  {
      auto temp=make_write(static_cast<uint8_t>(MCP47FEB_addresses::DAC0_volatile));

      return conn.Send_Data(address,value,temp);
  }

//not sure how is this supposed to work
//TODO check with real hardware
uint32_t MCP47FEB::Set_Continuous (const uint16_t * value,
					    uint32_t lenght)
  {
      volatile auto temp=make_write(static_cast<uint8_t>(MCP47FEB_addresses::DAC0_volatile));
      volatile I2C::ErrorCode e=conn.Send_Data_Circular(address,value, lenght,temp);
return 0;
  }
