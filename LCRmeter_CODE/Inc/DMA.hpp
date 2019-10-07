/**
  ******************************************************************************
  * @file    DMA.hpp
  * @author  Rafał Mazurkiewicz
  * @date    13.05.2019
  * @brief   Class single header file to configure STM32's DMA
  ******************************************************************************
  * @attention
  * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
  ******************************************************************************
  */


#ifndef DMA_HPP_
#define DMA_HPP_

//Include proper header files and modify implementation. Preferable use #ifdef.
#include "stm32f1xx.h"

/** \defgroup  DMA_group DMA library
 * This is a single header library for stm32's DMA
 *  @{
 */

/*! @class DMA
 *  @brief This class is configuring DMA
 */
class DMA
{
public:
  /**@brief Constructor sets proper DMA channel.
   * @param d: DMA channel from HAL library ex: DMA1_Channel6.
   */
  DMA (DMA_Channel_TypeDef *d): dma(d)
  {
  }

  /// This enum is for setting size of data to be transmitted or received.
  enum Size : uint8_t{BYTE, HALF_WORD, WORD};
  /// This enum is for setting priorrity.
  enum Priority: uint8_t {LOW,MEDIUM,HIGH,VERY_HIGH};

  /**@brief Sets data size to be send
   * @param data number of bytes, words, etc. to transmit
   */
  void Set_Data_Count(uint32_t data)
  {
    dma->CNDTR=data;
  }

  /**@brief Sets Peripheral address
   * @param data address
   */
  void Set_Peripheral_Addr(uint32_t data)
  {
    dma->CPAR=data;
  }

  /**@brief Sets Memory address
   * @param data address
   */
  void Set_Memory_Addr(uint32_t data)
  {
    dma->CMAR=data;
  }

  /**@brief Enables memory pointer increment
   * @param enabled desired state to set
   */
  void Set_Minc(bool enabled)
  {
    if(enabled !=0)
      {
    dma->CCR|=(1<<DMA_CCR_MINC_Pos);
      }
    else
      {
	dma->CCR &=~(1<<DMA_CCR_MINC_Pos);
      }
  }

  /**@brief Enables peripheral pointer increment
   * @param enabled desired state to set
   */
  void Set_Pinc(bool enabled)
  {
    if(enabled !=0)
      {
	dma->CCR|=(1<<DMA_CCR_PINC_Pos);
      }
    else
      {
	dma->CCR &=~(1<<DMA_CCR_PINC_Pos);
      }
  }

  /**@brief Sets direction - to peripheral or memory
   * @param toperipheral If false direction is set to memory
   */
  void Set_Direction(bool toperipheral)
  {
    if(toperipheral !=0)
      {
	dma->CCR|=(1<<DMA_CCR_DIR_Pos);
      }
    else
      {
	dma->CCR &=~(1<<DMA_CCR_DIR_Pos);
      }
  }

  /**@brief Enable DMA
   */
  void Enable(void)
  {
    dma->CCR|=DMA_CCR_EN;
  }

  /**@brief Disable DMA
   */
  void Disable(void)
  {
    dma->CCR &=~ DMA_CCR_EN;
  }

  /**@brief Sets size of memory
   * @param S Can be a value of #Size
   */
  void Set_Size_Memory(Size S)
  {
    dma->CCR &=~(3<<DMA_CCR_MSIZE_Pos);//clean register
    dma->CCR |=(S<<DMA_CCR_MSIZE_Pos);
  }

  /**@brief Sets size of peripheral
   * @param S Can be a value of #Size
   */
  void Set_Size_Peripheral(Size S)
  {
    dma->CCR &=~(3<<DMA_CCR_PSIZE_Pos);//clean register
    dma->CCR |=(S<<DMA_CCR_PSIZE_Pos);
  }

  /**@brief Sets priority level
   * @param priority Can be a value of Priority enum.
   */
  void Set_Priority(Priority priority)
  {
    dma->CCR &=~(3<<DMA_CCR_PL_Pos);//clean register
    dma->CCR |=(priority<<DMA_CCR_PL_Pos);
  }

  /**@brief Enables circular mode
   * @param enabled 0 for disabling
   */
  void Circular_Enable(bool enabled)
  {
      if(enabled !=0)
        {
	  dma->CCR |=(1<<DMA_CCR_CIRC_Pos);
        }
      else
        {
	  dma->CCR &=~(1<<DMA_CCR_CIRC_Pos);
        }
  }

private:
  DMA_Channel_TypeDef *dma;
};

/** @}*/
#endif /* DMA_HPP_ */
