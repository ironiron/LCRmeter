/*
 * I2C_hardware.hpp
 *
 *  Created on: 17.05.2019
 *      Author: Rafa³
 */

#ifndef FAKES_I2C_HARDWARE_HPP_
#define FAKES_I2C_HARDWARE_HPP_

#include "I2C.hpp"

#include <stdint.h>
#include <vector>

namespace testing
{
  extern std::vector<uint8_t> send_data;
  extern bool status_addr_bit;
  extern bool status_nack_bit;
  extern bool status_bus_busy_bit;
  extern bool status_bus_error_bit;
  extern bool status_arbitration_lost_bit;
  extern bool status_TxE_bit;
}

inline void I2C::reset (void)
{
}

inline void I2C::delay (uint32_t)
{

}

inline bool I2C::Get_Status_Addr_Bit (void)
{
  return testing::status_addr_bit;
}

inline bool I2C::Get_Status_NACK_Bit (void)
{
  return testing::status_nack_bit;
}

inline uint32_t I2C::Get_Status1_Reg (void)
{
  return i2c->SR1;
}

inline uint32_t I2C::Get_Status2_Reg (void)
{
  return i2c->SR2;
}

inline bool I2C::Get_Status_Bus_Busy_Bit (void)
{
  return testing::status_bus_busy_bit;
}

inline bool I2C::Get_Status_Bus_Error_Bit (void)
{
  return testing::status_bus_error_bit;
}

inline bool I2C::Get_Status_Arbitration_Lost_Bit (void)
{
  return testing::status_arbitration_lost_bit;
}

inline bool I2C::Get_Status_TxE_Bit (void)
{
  return testing::status_TxE_bit;
}

inline I2C::ErrorCode I2C::Check_Errors_After_Data (void)
{
  uint32_t i = 0;
  while(Get_Status_TxE_Bit()==0)
    {
      if(Get_Status_Bus_Busy_Bit() !=0 )
    	{
    	  return I2C::ErrorCode::BUS_BUSY;
    	}
      if(Get_Status_Bus_Error_Bit() !=0 )
    	{
    	  return I2C::ErrorCode::BUS_ERROR;
    	}
      if(Get_Status_Arbitration_Lost_Bit() !=0 )
    	{
    	  return I2C::ErrorCode::ARBITION_LOST;
    	}
      if(Get_Status_NACK_Bit() != 0)
    	{
    	  return I2C::ErrorCode::NACK;
    	}
      i++;
      if (i > timeout * 1000)
	{
	  return I2C::ErrorCode::TIMEOUT;
	}
      delay(1);
    }
  return I2C::ErrorCode::OK;
}

inline I2C::ErrorCode I2C::Check_Errors_After_Addr (void)
{
  uint32_t i = 0;
  while (Get_Status_Addr_Bit()==0)//Timeout routine
    {
      if(Get_Status_Bus_Busy_Bit() !=0 )
          	{
          	  return I2C::ErrorCode::BUS_BUSY;
          	}
            if(Get_Status_Bus_Error_Bit() !=0 )
          	{
          	  return I2C::ErrorCode::BUS_ERROR;
          	}
            if(Get_Status_Arbitration_Lost_Bit() !=0 )
          	{
          	  return I2C::ErrorCode::ARBITION_LOST;
          	}
            if(Get_Status_NACK_Bit() != 0)
          	{
          	  return I2C::ErrorCode::NACK;
          	}
      i++;
      if (i > timeout * 1000)
	{
	  return I2C::ErrorCode::TIMEOUT;
	}
      delay(1);
    }
  return I2C::ErrorCode::OK;
}

//TODO clean inline etc.


#endif /* FAKES_I2C_HARDWARE_HPP_ */
