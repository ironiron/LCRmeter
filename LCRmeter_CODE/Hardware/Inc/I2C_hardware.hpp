///*
// * I2C_hardware.hpp
// *
// *  Created on: 17.05.2019
// *      Author: Rafa³
// */
//
//#ifndef I2C_HARDWARE_HPP_
//#define I2C_HARDWARE_HPP_
//#include "I2C.hpp"
////#include "../Inc/I2C.hpp"
//#include "I2C_hardware_conf.hpp"
//
//#include "delay.h"
//#include <stdint.h>
//
//inline I2C::ErrorCode I2C::Check_Errors_After_Data (void)//TODO move this to I2C.cpp file- not really hardware
//{
//  uint32_t i = 0;
//  while(Get_Status_TxE_Bit()==0)
//    {
//      if(Get_Status_Bus_Busy_Bit() !=0 )
//    	{
//    	  return I2C::ErrorCode::BUS_BUSY;
//	  i2c->CR1|= I2C_CR1_SWRST;
//	  HAL_Delay(50);
//	  i2c->CR1&=~ I2C_CR1_SWRST;
//    	}
//      if(Get_Status_Bus_Error_Bit() !=0 )
//    	{
//	  //return I2C::ErrorCode::BUS_ERROR;
//	  //This bit is set when start or stop  are generated on high SCL line.
//	  //TODO fix this for self-testing and self-fix.
//	  return I2C::ErrorCode::OK;
//    	}
//      if(Get_Status_Arbitration_Lost_Bit() !=0 )
//    	{
//    	  return I2C::ErrorCode::ARBITION_LOST;
//    	}
//      if(Get_Status_NACK_Bit() != 0)
//    	{
//    	  return I2C::ErrorCode::NACK;
//    	}
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay(1);
//    }
//  return I2C::ErrorCode::OK;
//}
//
//inline I2C::ErrorCode I2C::Check_Errors_After_Addr (void)
//{
//  uint32_t i = 0;
//  while (Get_Status_Addr_Bit () == 0) //Timeout routine
//    {
//      if (Get_Status_Bus_Busy_Bit () != 0)
//	{
//	  return I2C::ErrorCode::BUS_BUSY;
//	  /////////////////////////////////////TEST//////////////////////
//	  /////////////////////////////////
//	  i2c->CR1|= I2C_CR1_SWRST;
//	  HAL_Delay(50);
//	  i2c->CR1&=~ I2C_CR1_SWRST;
//	  //////////////////////////////
//	  /////////////////////////////////////////////////////////////////
//	}
//      if (Get_Status_Bus_Error_Bit () != 0)
//	{
//	  //return I2C::ErrorCode::BUS_ERROR;
//	  //This bit is set when start or stop  are generated on high SCL line.
//	  return I2C::ErrorCode::OK;
//	}
//      if (Get_Status_Arbitration_Lost_Bit () != 0)
//	{
//	  return I2C::ErrorCode::ARBITION_LOST;
//	}
//      if (Get_Status_NACK_Bit () != 0)
//	{
//	  return I2C::ErrorCode::NACK;
//	}
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//  return I2C::ErrorCode::OK;
//}
//
//#endif /* I2C_HARDWARE_HPP_ */
