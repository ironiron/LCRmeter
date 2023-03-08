/*
 * I2C.h
 *
 *  Created on: 12.05.2019
 *      Author: Rafa�
 */

#ifndef I2C_HPP_
#define I2C_HPP_


#include <stdint.h>
#include <vector>

/**
 * @class I2C
 * @brief This class handles communication either by DMA or blocking transmission
 *
 */

class I2C
{
public:
  I2C (){}

  enum ErrorCode{OK,TIMEOUT,NACK,BUS_ERROR,ARBITION_LOST,BUS_BUSY,
      GENERAL_ERROR,DMA_DISABLED};//TODO consider this

  I2C::ErrorCode Send_Data(uint8_t addr, uint8_t byte);
  I2C::ErrorCode Send_Data(uint8_t addr, uint16_t byte)
  {
    index++;
    data.push_back(byte);
    adress.push_back(addr);
  }
  I2C::ErrorCode Send_Data(uint8_t addr, uint8_t byte,uint8_t mem_addr);
  I2C::ErrorCode Send_Data(uint8_t addr, uint16_t byte,uint8_t mem_addr)
  {
    index++;
    data.push_back(byte);
    adress.push_back(addr);
    memoryaddress.push_back(mem_addr);
  }
  I2C::ErrorCode Send_Data(uint8_t addr, uint16_t byte,uint16_t mem_addr)
  {
    index++;
    data.push_back(byte);
    adress.push_back(addr);
    memoryaddress.push_back(mem_addr);
  }
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint8_t *byte,uint32_t size);
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint16_t *byte,uint32_t size)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
      }
  }
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint8_t *byte,uint32_t size,uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Cont(uint8_t addr, const uint16_t *byte,uint32_t size,uint16_t mem_addr)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
	memoryaddress.push_back (mem_addr);
      }
  }

  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint8_t *byte,uint32_t size);

  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
      }
  }
  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint8_t *byte,uint32_t size,uint8_t mem_addr);
  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size,uint8_t mem_addr)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
	memoryaddress.push_back (mem_addr);
      }
  }
  I2C::ErrorCode Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size,uint16_t mem_addr)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
	memoryaddress.push_back (mem_addr);
      }
  }
  int index=0;
  std::vector<uint16_t> data={0};
  std::vector<uint16_t> adress={0};
  std::vector<uint16_t> memoryaddress={0};

  void Stop_Transfer (void);
};
#endif /* I2C_HPP_ */
