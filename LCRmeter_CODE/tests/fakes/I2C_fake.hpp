/*
 * I2C.h
 *
 *  Created on: 12.05.2019
 *      Author: Rafa³
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
  I2C ()
  {
    // TODO Auto-generated constructor stub

  }
  void Send_Data(uint8_t addr, uint8_t byte);
  void Send_Data(uint8_t addr, uint16_t byte)
  {
    index++;
    data.push_back(byte);
    adress.push_back(addr);
  }
  void Send_Data(uint8_t addr, uint8_t byte,uint8_t mem_addr);
  void Send_Data(uint8_t addr, uint16_t byte,uint8_t mem_addr)
  {
    index++;
    data.push_back(byte);
    adress.push_back(addr);
    memoryaddress.push_back(mem_addr);
  }
  void Send_Data(uint8_t addr, uint16_t byte,uint16_t mem_addr)
  {
    index++;
    data.push_back(byte);
    adress.push_back(addr);
    memoryaddress.push_back(mem_addr);
  }
  void Send_Data_Cont(uint8_t addr, const uint8_t *byte,uint32_t size);
  void Send_Data_Cont(uint8_t addr, const uint16_t *byte,uint32_t size)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
      }
  }
  void Send_Data_Cont(uint8_t addr, const uint8_t *byte,uint32_t size,uint8_t mem_addr);
  void Send_Data_Cont(uint8_t addr, const uint16_t *byte,uint32_t size,uint16_t mem_addr)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
	memoryaddress.push_back (mem_addr);
      }
  }

  void Send_Data_Circular(uint8_t addr, const uint8_t *byte,uint32_t size);

  void Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
      }
  }
  void Send_Data_Circular(uint8_t addr, const uint8_t *byte,uint32_t size,uint8_t mem_addr);
  void Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size,uint8_t mem_addr)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
	memoryaddress.push_back (mem_addr);
      }
  }
  void Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size,uint16_t mem_addr)
  {
    for (uint32_t in = 0; in < size; ++in)
      {
	index++;
	data.push_back (byte[in]);
	adress.push_back (addr);
	memoryaddress.push_back (mem_addr);
      }
  }

  void Enable(void);
  void Disable(void);
  void Set_Frequency(const uint32_t);
  void Reset(void);
  void Initialise(void);

  int index=0;
  std::vector<uint16_t> data={0};
  std::vector<uint16_t> adress={0};
  std::vector<uint16_t> memoryaddress={0};

private:

};
#endif /* I2C_HPP_ */
