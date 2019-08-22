/**
  ******************************************************************************
  * @file    DMA.hpp
  * @author  Rafa³ Mazurkiewicz
  * @date    13.05.2019
  * @brief   Class single header file to configure STM32's DMA
  ******************************************************************************
  * @attention
  * <h2><center>&copy; standard MIT License COPYRIGHT(c) 2019 Rafa³ Mazurkiewicz</center></h2>
  ******************************************************************************
  */


#ifndef I2C_HPP_
#define I2C_HPP_

#include <stdint.h>
#include "I2C_hardware_conf.hpp"

// TODO Send byte receive dma continous send enable disable

//TODO organize this so just one function is for sending bytes- send bytes(byte,number)?
//or send bytes(byte1,byt2...)?


/**
 * @brief This class is configuring I2C
 * @warning class not finished: no tests provided, and no good error handling,
 * no stop dma transfer function, nor single dma transfer, bad code organisation. BUT WORKS.
 */
class I2C
{
public:
  /**@brief Constructor sets DMA channel and I2C peripheral.
   * @param d: DMA channel from HAL library ex: DMA1_Channel6.
   */
  I2C (I2C_TYPE_DEF * i2c_com=0,DMA_TYPE_DEF  *d=0): i2c(i2c_com),dma(d)
  {
    // TODO Auto-generated constructor stub
  }

  /// This enum holds error cases available by drivers.
  enum ErrorCode{OK,TIMEOUT,NACK,BUS_ERROR,ARBITION_LOST,BUS_BUSY,
    GENERAL_ERROR,DMA_DISABLED};

 // void Send_Byte(uint8_t addr, uint8_t byte);//TODO this ought to be dma and non-dma as well
  //void Send_Byte_Cont(uint8_t addr, uint8_t *byte,uint32_t size);

  //////////////////////////////////////////


  I2C::ErrorCode Send_Data(uint8_t addr, uint8_t byte);
  I2C::ErrorCode Send_Data(uint8_t addr, uint16_t byte);
  I2C::ErrorCode Send_Data(uint8_t addr, uint8_t byte,uint8_t mem_addr);
  I2C::ErrorCode Send_Data(uint8_t addr, uint16_t byte,uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint8_t *byte,uint32_t size);
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint16_t *byte,uint32_t size);
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint8_t *byte,uint32_t size,uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint16_t *byte,uint32_t size,uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint8_t *byte,uint32_t size);
  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size);
  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint8_t *byte,uint32_t size,uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size,uint8_t mem_addr);

  void Enable(void);
  void Disable(void);
  void Enable_DMA(void);
  void Disable_DMA(void);

  void Set_Frequency(const uint32_t);
  void Reset(void);

  void Initialise(void);
  void Set_Timeout(uint32_t timeout){this->timeout=timeout;}

  inline static void Check_Errors_ISR(I2C &);

  I2C::ErrorCode Get_Last_Error(void){return this->lasterror;}//TODO naming convention- define here or in hpp?

private:
  I2C_TYPE_DEF * i2c;
  DMA_TYPE_DEF *dma;
  uint32_t timeout=200;
  bool circular=0;
  //rw not supported yet

  void Send_Address(uint8_t addr,bool rw=0);
  void Send_Byte(uint8_t byte);
  void Send_Bytes(uint8_t *byte,uint32_t size);
  inline bool Get_Status_Addr_Bit(void);
  inline bool Get_Status_NACK_Bit(void);
  inline bool Get_Status_Bus_Busy_Bit (void);
  inline bool Get_Status_Bus_Error_Bit (void);
  inline bool Get_Status_Start_Bit (void);
  inline bool Get_Status_Arbitration_Lost_Bit (void);
  inline bool Get_Status_TxE_Bit (void);
  inline uint32_t Get_Status1_Reg(void);
  inline uint32_t Get_Status2_Reg(void);
  void Generate_Start(void);
  void Generate_Stop(void);
  inline void delay(uint32_t);
  inline void reset(void);
  inline I2C::ErrorCode Check_Errors_After_Data (void);
  inline I2C::ErrorCode Check_Errors_After_Addr (void);
  void Allocate_Bytes_DMA(const uint8_t* bytes,uint32_t size,bool circular=true);
  ErrorCode lasterror=ErrorCode::OK;
  ErrorCode error=ErrorCode::OK;
};

#endif /* I2C_HPP_ */
