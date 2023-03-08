/*
 * testing.cpp
 *
 *  Created on: 01.09.2019
 *      Author: Rafa�
 */


#include "testing.hpp"

namespace testing
{
  namespace i2c
  {
    std::vector<uint8_t> send_data;
    bool status_addr_bit=1;//initial value
    bool status_nack_bit=0;//initial value
    bool status_bus_busy_bit=0;//initial value
    bool status_bus_error_bit=0;//initial value
    bool status_arbitration_lost_bit=0;//initial value
    bool status_TxE_bit=1;//initial value
    bool status_start_bit=1;
  }
    namespace pwm
    {
      int prescaler=200;
      int counter=100;
      int compare=0;
      int clockfreq=70000000;
    }

    namespace mcp47
    {

    }

//    namespace ssd1306
//    {
//      std::vector<uint8_t>  data;
//
//    }
}
