/*
 * I2C.h
 *
 *  Created on: 12.05.2019
 *      Author: Rafa³
 */

#ifndef I2C_HPP_
#define I2C_HPP_

//#include <stdint.h>

#include "stm32f1xx.h"
#include "DMA.hpp"

// TODO Send byte receive dma continous send enable disable
//TODO make i2c derived of dma class

class I2C
{
public:
  I2C ()
  {
    // TODO Auto-generated constructor stub

  }
  void Send_Byte(uint8_t addr, uint8_t byte);//TODO this ought to be dma and non-dma as well
  void Send_Byte_Cont(uint8_t addr, uint8_t *byte,uint32_t size);

  void Enable(void);
  void Disable(void);

  void Set_Frequency(const uint32_t);
  void Reset(void);


  void Initialise(void);
private:
  I2C_TypeDef *i2c;

};

#endif /* I2C_HPP_ */
