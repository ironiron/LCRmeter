#ifndef FAKES_I2C_HARDWARE_CONF_HPP_
#define FAKES_I2C_HARDWARE_CONF_HPP_

//include hardware related headers
#include "stm32f1xx.h"
#include "DMA.hpp"

/// typedef for I2C structure. Use Hal structure by default.
typedef I2C_TypeDef I2C_TYPE_DEF ;//from CMSIS
/// typedef for DMA structure. Use Hal structure by default.
typedef DMA DMA_TYPE_DEF ;

#endif /* I2C_HARDWARE_CONF_HPP_ */
