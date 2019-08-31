/*
 * I2C_hardware.hpp
 *
 *  Created on: 17.05.2019
 *      Author: Rafa³
 */

#ifndef I2C_HARDWARE_HPP_
#define I2C_HARDWARE_HPP_
#include "I2C.hpp"
//#include "../Inc/I2C.hpp"
#include "I2C_hardware_conf.hpp"

#include "delay.h"
#include <stdint.h>


inline void I2C::reset (void)
{
  I2C1->CR1|= I2C_CR1_SWRST;
  HAL_Delay(50);
  I2C1->CR1&=~ I2C_CR1_SWRST;
}

inline void I2C::delay (uint32_t microseconds)
{
  delay_us(microseconds);
}

inline bool I2C::Get_Status_Addr_Bit (void)
{
  return i2c->SR1 & I2C_SR1_ADDR;
}

inline bool I2C::Get_Status_NACK_Bit (void)
{
  return i2c->SR1 & I2C_SR1_AF;
}

inline uint32_t I2C::Get_Status1_Reg (void)
{
  return *(volatile uint32_t *)i2c->SR1;
}

inline uint32_t I2C::Get_Status2_Reg (void)
{
  return *(volatile uint32_t *)i2c->SR2;
}

inline bool I2C::Get_Status_Bus_Busy_Bit (void)
{
  return i2c->SR1 & I2C_SR2_BUSY;
}

inline bool I2C::Get_Status_Start_Bit (void)
{
  return i2c->SR1 & I2C_SR1_SB;
}

inline bool I2C::Get_Status_Bus_Error_Bit (void)
{
  return i2c->SR1 & I2C_SR1_BERR;
}

inline bool I2C::Get_Status_Arbitration_Lost_Bit (void)
{
  return i2c->SR1 & I2C_SR1_ARLO;
}

inline bool I2C::Get_Status_TxE_Bit (void)
{
  return i2c->SR1 & I2C_SR1_TXE;
}

inline I2C::ErrorCode I2C::Check_Errors_After_Data (void)//TODO move this to I2C.cpp file- not really hardware
{
  uint32_t i = 0;
  while(Get_Status_TxE_Bit()==0)
    {
      if(Get_Status_Bus_Busy_Bit() !=0 )
    	{
    	  return I2C::ErrorCode::BUS_BUSY;
	  i2c->CR1|= I2C_CR1_SWRST;
	  HAL_Delay(50);
	  i2c->CR1&=~ I2C_CR1_SWRST;
    	}
      if(Get_Status_Bus_Error_Bit() !=0 )
    	{
	  //return I2C::ErrorCode::BUS_ERROR;
	  //This bit is set when start or stop  are generated on high SCL line.
	  //TODO fix this for self-testing and self-fix.
	  return I2C::ErrorCode::OK;
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
      I2C::delay(1);
    }
  return I2C::ErrorCode::OK;
}

inline I2C::ErrorCode I2C::Check_Errors_After_Addr (void)
{
  uint32_t i = 0;
  while (Get_Status_Addr_Bit () == 0) //Timeout routine
    {
      if (Get_Status_Bus_Busy_Bit () != 0)
	{
	  return I2C::ErrorCode::BUS_BUSY;
	  /////////////////////////////////////TEST//////////////////////
	  /////////////////////////////////
	  i2c->CR1|= I2C_CR1_SWRST;
	  HAL_Delay(50);
	  i2c->CR1&=~ I2C_CR1_SWRST;
	  //////////////////////////////
	  /////////////////////////////////////////////////////////////////
	}
      if (Get_Status_Bus_Error_Bit () != 0)
	{
	  //return I2C::ErrorCode::BUS_ERROR;
	  //This bit is set when start or stop  are generated on high SCL line.
	  return I2C::ErrorCode::OK;
	}
      if (Get_Status_Arbitration_Lost_Bit () != 0)
	{
	  return I2C::ErrorCode::ARBITION_LOST;
	}
      if (Get_Status_NACK_Bit () != 0)
	{
	  return I2C::ErrorCode::NACK;
	}
      i++;
      if (i > timeout * 1000)
	{
	  return I2C::ErrorCode::TIMEOUT;
	}
      I2C::delay (1);
    }
  return I2C::ErrorCode::OK;
}

#endif /* I2C_HARDWARE_HPP_ */
