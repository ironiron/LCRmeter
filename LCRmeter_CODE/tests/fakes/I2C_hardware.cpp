/*
 * I2C_test.cpp
 *
 *  Created on: 22.05.2019
 *      Author: Rafa� Mazurkiewicz
 */

#include "I2C.hpp"
#include "testing.hpp"

#include <iostream>
using namespace std;

void I2C::Stop_DMA(void){}

void I2C::Send_Address (uint8_t addr, bool rw)
{
  testing::i2c::send_data.push_back (addr);
}

void I2C::Send_Byte (const uint8_t byte)
{
  testing::i2c::send_data.push_back (byte);
}

void I2C::Generate_Start (void)
{
}

void I2C::Allocate_Bytes_DMA(const uint8_t* bytes,uint32_t size,bool circular)
{
  testing::i2c::send_data.insert (std::end (testing::i2c::send_data), bytes, bytes + size);
}

void I2C::Generate_Stop (void)
{
}

void I2C::Enable (void)
{
}

void I2C::Disable (void)
{
}

void I2C::Initialise (void)
{
}

void I2C::Reset_Bus (void)
{
}

void I2C::Delay (uint32_t)
{
}

bool I2C::Get_Status_Addr_Bit (void)
{
 return testing::i2c::status_addr_bit;
}

bool I2C::Get_Status_NACK_Bit (void)
{
 return testing::i2c::status_nack_bit;
}

uint32_t I2C::Get_Status1_Reg (void)
{
 return i2c->SR1;
}

bool I2C::Get_Status_Start_Bit (void)
{
 return testing::i2c::status_start_bit;
}

uint32_t I2C::Get_Status2_Reg (void)
{
 return i2c->SR2;
}

bool I2C::Get_Status_Bus_Busy_Bit (void)
{
 return testing::i2c::status_bus_busy_bit;
}

bool I2C::Get_Status_Bus_Error_Bit (void)
{
 return testing::i2c::status_bus_error_bit;
}

bool I2C::Get_Status_Arbitration_Lost_Bit (void)
{
 return testing::i2c::status_arbitration_lost_bit;
}

bool I2C::Get_Status_TxE_Bit (void)
{
 return testing::i2c::status_TxE_bit;
}



