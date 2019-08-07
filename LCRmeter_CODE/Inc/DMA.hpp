/*
 * DMA.hpp
 *
 *  Created on: 13.05.2019
 *      Author: Rafa³
 */

#ifndef DMA_HPP_
#define DMA_HPP_

#include "stm32f1xx.h"
//TODO fix this baba
class DMA
{
public:
  DMA (DMA_Channel_TypeDef *d): dma(d)
  {
    // TODO Auto-generated constructor stub
  }
  enum Size{BYTE, HALF_WORD_WORD};

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

  void Set_Size_Tx(Size S)
  {

  }

  void Set_Size_Rx(Size S)
  {

  }

  void Circular_Enable(bool enable)
  {

  }
private:
  DMA_Channel_TypeDef *dma;
};
#endif /* DMA_HPP_ */
