/*
 * I2C_test.cpp
 *
 *  Created on: 22.05.2019
 *      Author: Rafa³ Mazurkiewicz
 */
#include "I2C_hardware.hpp"
#include "I2C.hpp"
#include <iostream>
using namespace std;

namespace testing
{
  std::vector<uint8_t> send_data;
  bool status_addr_bit=1;//initial value
  bool status_nack_bit=0;//initial value
  bool status_bus_busy_bit=0;//initial value
  bool status_bus_error_bit=0;//initial value
  bool status_arbitration_lost_bit=0;//initial value
  bool status_TxE_bit=1;//initial value
}

void I2C::Send_Address (uint8_t addr, bool rw)
{

  testing::send_data.push_back (addr);
}

void I2C::Send_Byte (uint8_t byte)
{
  testing::send_data.push_back (byte);
}

void I2C::Send_Bytes (uint8_t *byte, uint32_t size)
{
  testing::send_data.insert (std::end (testing::send_data), byte, byte + size);
}

void I2C::Generate_Start (void)
{

}

void I2C::Send_Bytes_DMA(const uint8_t* bytes,uint32_t size,bool circular)
{
  dma->Set_Direction(true);
  dma->Set_Minc(true);
  dma->Set_Data_Count(size);
  dma->Set_Size_Tx(DMA::Size::BYTE);
  if(circular==true)
    {
      dma->Circular_Enable(true);
    }
  else
  {
      dma->Circular_Enable(false);
  }

  //dma->Set_Peripheral_Addr((uint32_t)(&I2C1->DR));
  dma->Set_Memory_Addr((uint32_t)(bytes));
  dma->Enable();

  testing::send_data.insert (std::end (testing::send_data), bytes, bytes + size);
}

void I2C::Generate_Stop (void)
{

  //TODO paste this to production code
  /*
   *     I2C1->CR1|= I2C_CR1_SWRST;
   HAL_Delay(50);
   I2C1->CR1&=~ I2C_CR1_SWRST;
   */
}

void I2C::Enable (void)
{

}

void I2C::Disable (void)
{

}

void I2C::Initialise (void)
{
  /*Initialise in full speed 400kHz
   * freq=20MHz=>TPCLK1=50ns
   * Thigh = 9 * CCR * TPCLK1
   * Tlow = 16 * CCR * TPCLK1
   * tr(SCL)=300ns
   * tw(SCLH)=1,3us
   * thigh = tr(SCL) + tw(SCLH)=1,6us
   * tf(SCL)=300ns
   * tw(SCLL)=0,6us
   * tlow = tr(SCL) + tw(SCLH)=0,9us
   * CCR~4
   */

  /*  i2c->CR2|=36;
   //CCR = 50ns
   i2c->CCR|=I2C_CCR_F/S| I2C_CCR_DUTY | 4;
   i2c->TRISE|=7;
   i2c->CR1|=I2C_CR1_ACK |I2C_CR1_START| I2C_CR1_PE;*/
}

void I2C::Set_Frequency (const uint32_t constUnsignedInt)
{
}

