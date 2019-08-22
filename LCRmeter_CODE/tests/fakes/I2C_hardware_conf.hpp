#ifndef FAKES_I2C_HARDWARE_CONF_HPP_
#define FAKES_I2C_HARDWARE_CONF_HPP_
//#include "fakes/DMA.hpp"
#include "DMA.hpp"
 struct  {
  uint16_t DR=0;
  uint32_t SR1=0;
  uint32_t SR2=0;
}typedef I2C_TYPE_DEF ;

//TODO check if this can be placed in I2C_hardware.hpp

typedef DMA DMA_TYPE_DEF ;//TODO tidy this up

//from CMSIS file
#define I2C_SR1_ADDR_Pos                    (1U)
#define I2C_SR1_ADDR_Msk                    (0x1U << I2C_SR1_ADDR_Pos)
#define I2C_SR1_ADDR                        I2C_SR1_ADDR_Msk




#endif /* FAKES_I2C_HARDWARE_CONF_HPP_ */
