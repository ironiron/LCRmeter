/*
 * I2C.h
 *
 *  Created on: 12.05.2019
 *      Author: Rafa³
 */

#ifndef I2C_HPP_
#define I2C_HPP_

#include <stdint.h>
#include "I2C_hardware_conf.hpp"
#include "fakes/DMA.hpp"


// TODO Send byte receive dma continous send enable disable
//TODO make i2c derived of dma class
//TODO rename


/**
 * @warning class not finished: no tests provided, and no good error handling
 */
class I2C
{
public:
  I2C (I2C_TYPE_DEF * i2c_com=0,DMA_TYPE_DEF  *d=0): i2c(i2c_com),dma(d)
  {
    // TODO Auto-generated constructor stub

  }
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

  void Set_Frequency(const uint32_t);
  void Reset(void);

  void Initialise(void);
  void Set_Timeout(uint32_t timeout){this->timeout=timeout;}

private:
  I2C_TYPE_DEF * i2c;
  DMA_TYPE_DEF *dma;
  uint32_t timeout=100;
  //rw not supported yet

  void Send_Address(uint8_t addr,bool rw=0);
  void Send_Byte(uint8_t byte);
  void Send_Bytes(uint8_t *byte,uint32_t size);
  inline bool Get_Status_Addr_Bit(void);
  inline bool Get_Status_NACK_Bit(void);
  inline bool Get_Status_Bus_Busy_Bit (void);
  inline bool Get_Status_Bus_Error_Bit (void);
  inline bool Get_Status_Arbitration_Lost_Bit (void);
  inline bool Get_Status_TxE_Bit (void);
  inline uint32_t Get_Status1_Reg(void);
  inline uint32_t Get_Status2_Reg(void);
  void Generate_Start(void);
  void Generate_Stop(void);
  inline void delay_us(uint32_t);
  inline void reset(void);
  inline I2C::ErrorCode Check_Errors_After_Data (void);
  inline I2C::ErrorCode Check_Errors_After_Addr (void);
  void Send_Bytes_DMA(const uint8_t* bytes,uint32_t size,bool circular=true);


  ErrorCode error=ErrorCode::OK;
};

#endif /* I2C_HPP_ */
