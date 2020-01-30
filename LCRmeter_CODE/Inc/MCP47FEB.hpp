/**
  ******************************************************************************
  * @file    MCP47FEB.hpp
  * @author  Rafał Mazurkiewicz
  * @date    02.10.2019
  * @brief   Class header file for DAC based on MCP47FEB chip
  ******************************************************************************
  * @attention
  * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
  ******************************************************************************
  */

#ifndef MCP47FEB_HPP_
#define MCP47FEB_HPP_

#ifdef TEST
#include <I2C_fake.hpp>
#else
#include "I2C.hpp"
#endif
#include <stdint.h>

/** \defgroup  MCP47FEB_group MCP47FEB library
 * This is a library for DAC control. It consist of 3 files:
 * source, header and hardware related source file
 * @warning currently only 1 output is supported
 *  @{
 */

/*! @class MCP47FEB
 *  @brief Class for controlling DAC
 */
class MCP47FEB
{
public:

  /**@brief Constructor. Nothing special done here
   * @param connection_port interface to which DAC is connected
   */
  MCP47FEB (I2C & connection_port): conn(connection_port)
  {

  }
  /// This enum is for setting reference voltage
  enum Vref: uint16_t{VDD,VINT,VREF,VREF_BUFFERED};

  /**@brief Sets output voltage
   * @param voltage in bits(4095 max.)
   * @retval error code from underlaying I2C interface class.
   * @warning It does not test arguments for out-of-range.
   */
  uint32_t Set_Output(uint16_t voltage);

  /**@brief Sets output voltage
   * @param value byte of values bits(4095 max. value)
   * @param lenght size of array
   * @retval error code from underlaying I2C interface class.
   * @warning It does not test arguments for out-of-range.
   */
  uint32_t Set_Continuous(const uint8_t *value,uint32_t lenght);

  /**@brief Hardware related function used for setting latch pin
   */
  void Enable_Output(void);

  /**@brief Hardware related function used for setting latch pin
   */
  void Disable_Output(void);

  /**@brief Sets voltage reference source
   * @param source of voltage can be a value of #Vref enum
   * @retval error code from underlaying I2C interface class.
   */
  uint32_t Set_Vref(Vref source);

  /**@brief Sets Operating mode
   * @retval error code (not used now-hardcoded 0)
   */
  uint32_t Stop_DAC(void);

private:
  const static uint8_t address=0xC0;

  ///enum that contain addresses of chip's internal registers
  enum MCP47FEB_addresses: uint8_t
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
  I2C & conn;
  uint8_t make_write(uint8_t a);
};

/** @}*/
#endif /* MCP47FEB_HPP_ */
