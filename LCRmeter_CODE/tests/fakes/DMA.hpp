/*
 * DMA.hpp
 *
 *  Created on: 13.05.2019
 *      Author: Rafa³
 */

#ifndef DMA_HPP_
#define DMA_HPP_



class DMA
{
public:
  DMA ()//: dma(d)
  {
    // TODO Auto-generated constructor stub
  }
  // DMA_Channel_TypeDef *dma;
  enum Size{BYTE, HALF_WORD_WORD};

  void Set_Data_Count(uint32_t data)
  {
  }

  void Set_Peripheral_Addr(uint32_t data)
  {
  }

  void Set_Memory_Addr(uint32_t data)
  {
  }

  void Set_Minc(bool enabled)
  {
  }

  void Set_Pinc(bool enabled)
  {
  }

  void Set_Direction(bool toperipheral)
  {
  }

  void Circular_Enable(bool enable)
  {

  }

  void Enable(void)
  {

  }

  void Disable(void)
  {

  }

  void Set_Size_Tx(Size S)
  {

  }

  void Set_Size_Rx(Size S)
  {

  }
};
#endif /* DMA_HPP_ */
