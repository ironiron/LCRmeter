#ifndef FAKES_I2C_HARDWARE_CONF_HPP_
#define FAKES_I2C_HARDWARE_CONF_HPP_

#include "DMA.hpp"

 struct  {
  uint16_t DR=0;
  uint32_t SR1=0;
  uint32_t SR2=0;
}typedef I2C_TYPE_DEF ;

typedef DMA DMA_TYPE_DEF ;

#endif /* FAKES_I2C_HARDWARE_CONF_HPP_ */
