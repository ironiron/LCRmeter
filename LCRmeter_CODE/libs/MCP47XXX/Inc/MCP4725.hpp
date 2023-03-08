/**
  ******************************************************************************
  * @file    MCP4725.hpp
  * @author  Rafał Mazurkiewicz
  * @date    02.10.2019
  * @brief   Class header file for DAC based on MCP4725 chip
  ******************************************************************************
  * @attention
  * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
  ******************************************************************************
  */

#ifndef MCP4725_HPP_
#define MCP4725_HPP_

#ifdef TEST
#include <I2C_fake.hpp>
#else
#include "I2C.hpp"
#endif

#include <stdint.h>

/** \defgroup  MCP4725_group MCP4725 library
 * This is a library for DAC control. It consist of 2 files:
 * source and header
 *  @{
 */



/*! @class MCP4725
 *  @brief Class for controlling DAC
 */
class MCP4725
{
public:

  /**@brief Constructor. Nothing special done here
   * @param connection_port interface to which DAC is connected
   */
  MCP4725 (I2C & connection_port): conn(connection_port)
  {
  }
  /// This enum is for setting power mode of the device (GND means output
  /// is internally connected to ground with X value resistor.
  enum PowerMode{NORMAL,GND_1K,GND_100K,GND_500K};

  /**@brief Sets output voltage
   * @param voltage in bits(4095 max.)
   * @retval error code from underlying I2C interface class.
   * @warning It does not test arguments for out-of-range.
   */
  uint32_t Set_Output(uint16_t voltage);

  /**@brief Sets output voltage based on value stored in input array
   * @param voltage_array: byte of values bits(4095 max. value)
   * @param length: size of array
   * @retval error code from underlying I2C interface class.
   * @warning It does not test arguments for out-of-range.
   */
  uint32_t Set_Continuous(const uint16_t *voltage_array,uint32_t length);

  /**@brief Resets DAC internal circuit
   * @retval error code from underlying I2C interface class.
   */
  uint32_t Reset(void);

  /**@brief Sets Operating mode
   * @param mode Can be a value of #PowerMode
   * @retval error code from underlying I2C interface class.
   */
  uint32_t Set_Power_mode(PowerMode mode);

  /**@brief Sets Operating mode
   * @retval error code (not used now-hardcoded 0)
   */
  uint32_t Stop_DAC(void);

private:
  I2C & conn;
  const static uint8_t address=0xC0;
};

/** @}*/
#endif /* MCP4725_HPP_ */
