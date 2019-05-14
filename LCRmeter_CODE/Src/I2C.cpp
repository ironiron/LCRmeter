/*
 * I2C.cpp
 *
 *  Created on: 12.05.2019
 *      Author: Rafa³
 */

#include <I2C.hpp>

void I2C::Send_Byte (uint8_t addr, uint8_t byte)
{
}

void I2C::Send_Byte_Cont (uint8_t addr, uint8_t* byte, uint32_t size)
{
}

void
I2C::Enable (void)
{
}

void
I2C::Disable (void)
{
}


void
I2C::Initialise (void)
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
