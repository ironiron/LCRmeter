/**
 ******************************************************************************
 * @file    I2C.hpp
 * @author  Rafał Mazurkiewicz
 * @date    13.05.2019
 * @brief   Class header to configure STM32's I2C
 ******************************************************************************
 * @attention
 * <h2><center>&copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz</center></h2>
 ******************************************************************************
 */

#ifndef I2C_HPP_
#define I2C_HPP_

#include <stdint.h>
#include "I2C_hardware_conf.hpp"
#include <vector>

//TODO add case to Errorcode enum for blocked interface.
//TODO add case for allocation problem
//TODO document those functions
//TODO is this a class? what's with private functions?

/**
 * @brief This class is configuring I2C
 * @warning class not finished: no tests provided, and no good error handling,
 * no stop dma transfer function, nor single dma transfer, bad code organisation.
 * Currently only
 * Send_Data(uint8_t addr, uint16_t byte, uint8_t mem_addr)
 * Send_Data_Circular(uint8_t addr, const uint8_t* byte,
 *		uint32_t size, uint8_t mem_addr)
 *		is tested and is working.
 *	The rest is not guaranted to work. This library seems like dead end, move to HAL if possible.
 *
 */
class I2C
{
public:
  /**@brief Constructor sets DMA channel and I2C peripheral.
   * @param d: DMA channel from HAL library ex: DMA1_Channel6.
   */
  I2C (I2C_TYPE_DEF * i2c_com = 0, DMA_TYPE_DEF *d = 0) :
      i2c (i2c_com), dma (d)
  {
  }

  /// This enum holds error cases available by drivers.
  enum ErrorCode
  {
    OK,
    TIMEOUT,
    NACK,
    BUS_ERROR,
    ARBITION_LOST,//TODO fix spelling
    BUS_BUSY,
    GENERAL_ERROR,
    DMA_DISABLED
  };

  I2C::ErrorCode Send_Data (uint8_t addr, uint8_t byte);
  I2C::ErrorCode Send_Data (uint8_t addr, uint16_t byte);
  I2C::ErrorCode Send_Data (uint8_t addr, uint8_t byte, uint8_t mem_addr);
  I2C::ErrorCode Send_Data (uint8_t addr, uint16_t byte, uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Cont (uint8_t addr, const uint8_t *byte,
				 uint32_t size);
  I2C::ErrorCode Send_Data_Cont (uint8_t addr, const uint16_t *byte,
				 uint32_t size);
  I2C::ErrorCode Send_Data_Cont (uint8_t addr, const uint8_t *byte,
				 uint32_t size, uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Cont (uint8_t addr, const uint16_t *byte,
				 uint32_t size, uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Circular (uint8_t addr, const uint8_t *byte,
				     uint32_t size);
  I2C::ErrorCode Send_Data_Circular (uint8_t addr, const uint16_t *byte,
				     uint32_t size);
  I2C::ErrorCode Send_Data_Circular (uint8_t addr, const uint8_t *byte,
				     uint32_t size, uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Circular (uint8_t addr, const uint16_t *byte,
				     uint32_t size, uint8_t mem_addr);
  void Stop_Transfer (void);

  void Enable (void);
  void Disable (void);
  void Enable_DMA (void);
  void Disable_DMA (void);
  void Set_Frequency (const uint32_t frequency);
  void Reset (void);
  void Initialise (void);
  void Set_Timeout (uint32_t timeout)
  {
    this->timeout = timeout;
  }

  static void Check_Errors_ISR (I2C &);
  I2C::ErrorCode Get_Last_Error (void)
  {
    return this->lasterror;
  } //TODO naming convention- define here or in hpp?

//private:
  I2C_TYPE_DEF * i2c;
  DMA_TYPE_DEF *dma;
  uint32_t timeout = 20000;
  bool circular = 0;
  ErrorCode lasterror = ErrorCode::OK;
  ErrorCode error = ErrorCode::OK;
  uint8_t *ptr_to_bytes=0;
  uint16_t size_of_data=0;

  I2C::ErrorCode Send_Bytes (uint8_t address, const uint8_t *data, int size);
  I2C::ErrorCode Send_Bytes (uint8_t address, const uint8_t *data, int size,
			     uint8_t *mem_bytes, int mem_size);
  I2C::ErrorCode Send_Bytes_DMA (uint8_t address, const uint8_t *data, int size,
				 uint8_t *mem_bytes = 0, int mem_size = 0);
  I2C::ErrorCode Check_Errors_After_Data (void);
  I2C::ErrorCode Check_Errors_After_Addr (void);
  void Allocate_Bytes_DMA (const uint8_t* bytes, uint32_t size, bool circular =
			       true);
  void Stop_DMA(void);
  void Send_Address (uint8_t addr, bool rw = 0);
  void Send_Byte (const uint8_t byte);
  bool Get_Status_Addr_Bit (void);
  bool Get_Status_NACK_Bit (void);
  bool Get_Status_Bus_Busy_Bit (void);
  bool Get_Status_Bus_Error_Bit (void);
  bool Get_Status_Start_Bit (void);
  bool Get_Status_Arbitration_Lost_Bit (void);
  bool Get_Status_TxE_Bit (void);
  uint32_t Get_Status1_Reg (void);
  uint32_t Get_Status2_Reg (void);
  void Generate_Start (void);
  void Generate_Stop (void);
  void Delay (uint32_t);
  void Reset_Bus (void);
};

#endif /* I2C_HPP_ */
