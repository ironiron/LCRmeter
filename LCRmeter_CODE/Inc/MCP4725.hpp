/*
 * MCP47.h
 *
 *  Created on: 12.05.2019
 *      Author: Rafa�
 */

#ifndef MCP4725_HPP_
#define MCP4725_HPP_

#ifdef TEST
#include <I2C_fake.hpp>
#else
#include "I2C.hpp"
#endif

#include <stdint.h>

class MCP4725
{
public:
  MCP4725 (I2C & connection_port): conn(connection_port)
  {
  }

  enum PowerMode{NORMAL,GND_1K,GND_100K,GND_500K};

  uint32_t Set_Output(uint16_t voltage);
  uint32_t Set_Continuous(const uint16_t *value,uint32_t lenght);
  uint32_t Reset(void);
  uint32_t Set_Power_mode(PowerMode mode);

private:
  I2C & conn;//TODO check binary size
  const static uint8_t address=0xC0;
};

#endif /* MCP4725_HPP_ */
