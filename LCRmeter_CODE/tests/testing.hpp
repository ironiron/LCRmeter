/*
 * testing.hpp
 *
 *  Created on: 01.09.2019
 *      Author: Rafa�
 */

#ifndef TESTING_HPP_
#define TESTING_HPP_

#include <stdint.h>
#include <vector>

namespace testing
{
  namespace i2c
  {
    extern std::vector<uint8_t> send_data;
    extern bool status_addr_bit;
    extern bool status_nack_bit;
    extern bool status_bus_busy_bit;
    extern bool status_bus_error_bit;
    extern bool status_arbitration_lost_bit;
    extern bool status_TxE_bit;
    extern bool status_start_bit;
  }
  namespace pwm
  {
    extern int prescaler;
    extern int counter;
    extern int compare;
    extern int clockfreq;
  }
  namespace mcp47
  {

  }


//  namespace ssd1306
//  {
//    extern std::vector<uint8_t>  data;
//  }
}



#endif /* TESTING_HPP_ */
