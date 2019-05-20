/*
 * DMA.hpp
 *
 *  Created on: 13.05.2019
 *      Author: Rafa³
 */

#ifndef DMA_HPP_
#define DMA_HPP_

#include "stm32f1xx.h"

class DMA
{
public:
  DMA (DMA_Channel_TypeDef *d): dma(d)
  {
    // TODO Auto-generated constructor stub
  }
   DMA_Channel_TypeDef *dma;

  void baba(void)const{
    dma->CNDTR=5;
  }
  void Set_Data_Count(uint32_t data)
  {
    dma->CNDTR=data;
  }

  void Set_Peripheral_Addr(uint32_t data)
  {
    dma->CPAR=data;
  }

  void Set_Memory_Addr(uint32_t data)
  {
    dma->CMAR=data;
  }

  void Set_Minc(bool enabled)
  {
    dma->CCR|=(enabled<<DMA_CCR_MINC_Pos);
  }

  void Set_Pinc(bool enabled)
  {
    dma->CCR|=(enabled<<DMA_CCR_PINC_Pos);
  }

  void Set_Direction(bool toperipheral)
  {
    dma->CCR|=(toperipheral<<DMA_CCR_DIR_Pos);
  }

  void Enable(void)
  {
    dma->CCR|=DMA_CCR_EN;
  }

  void Disable(void)
  {
    dma->CCR &=~ DMA_CCR_EN;
  }
};
#endif /* DMA_HPP_ */
