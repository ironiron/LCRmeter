/*
 * I2C.cpp
 *
 *  Created on: 12.05.2019
 *      Author: Rafa³
 */

#include <I2C.hpp>
#include "I2C_hardware.hpp"
#include <vector>
#include <array>

//TODO add stop DMA transfer function
//TODO manual allocation of arrays for DMA automatic for non dma

uint32_t i = 0;
I2C::ErrorCode I2C::Send_Data (uint8_t addr, uint8_t byte)
{
  uint8_t arr[]={byte};
 // std::vector<uint8_t>arr{byte};

  return  Send_Bytes(addr,arr,1);
//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//
//   Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  Get_Status1_Reg();
//  Get_Status2_Reg();
// Send_Byte (byte);
// error=Check_Errors_After_Data();
// if(error!=ErrorCode::OK)
//   {
//     return error;
//   }
//  Generate_Stop();
//  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data (uint8_t addr, uint16_t byte)
{

  //std::vector<uint8_t>  *arr=  new std::vector<uint8_t>{uint8_t(byte>>8),uint8_t(byte&0xff)};
  uint8_t arr[]={uint8_t(byte>>8),uint8_t(byte&0xff)};
  Send_Bytes(addr,arr,2);

//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//
//  Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  //procedure required by hardware to clear Addr bit.
//  Get_Status1_Reg();
//  Get_Status2_Reg();
//
//  Send_Byte (byte>>8);//send upper byte first
//  error=Check_Errors_After_Data();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//
//  Send_Byte (byte & 0xff);
//  error=Check_Errors_After_Data();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//
//  Generate_Stop();
//  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data (uint8_t addr, uint8_t byte, uint8_t mem_addr)
{

  uint8_t arr[]={mem_addr,byte};
  return Send_Bytes(addr,&byte,1,&mem_addr,1);
//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//  Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  //uint32_t t;
//  //procedure required by hardware to clear Addr bit.
//  Get_Status1_Reg();
//  Get_Status2_Reg();
//
//  Send_Byte (mem_addr);
//  error=Check_Errors_After_Data();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
// Send_Byte (byte);
// error=Check_Errors_After_Data();
// if(error!=ErrorCode::OK)
//   {
//     return error;
//   }
//
//  Generate_Stop();
//  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data(uint8_t addr, uint16_t byte,uint8_t mem_addr)
{
  uint8_t arr[]={mem_addr,uint8_t(byte>>8),uint8_t(byte&0xff)};
  Send_Bytes(addr,arr,3);
//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//  Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  //uint32_t t;
//  //procedure required by hardware to clear Addr bit.
//  Get_Status1_Reg();
//  Get_Status2_Reg();
//
//  Send_Byte (mem_addr);
//  error=Check_Errors_After_Data();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  Send_Byte (byte >> 8);
//  error=Check_Errors_After_Data();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  Send_Byte (byte & 0xff);
//  error=Check_Errors_After_Data();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  Generate_Stop();
//  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Cont (uint8_t addr, const uint8_t* bytes, uint32_t size)
{
  //uint8_t arr[]={uint8_t(byte>>8),uint8_t(byte&0xff)};
  Send_Bytes(addr,bytes,size);
//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//  Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  //uint32_t t;
//  //procedure required by hardware to clear Addr bit.
//  Get_Status1_Reg();
//  Get_Status2_Reg();
//
//  for (uint32_t i=0;i<size*2;i++)
//    {
//      Send_Byte (bytes[i]);
//      error=Check_Errors_After_Data();
//      if(error!=ErrorCode::OK)
//        {
//          return error;
//        }
//    }
//
//  Generate_Stop();
//  return I2C::ErrorCode::OK;
}

//not recommended - uses memory allocation under the hood (16 bit)
I2C::ErrorCode I2C::Send_Data_Cont(uint8_t addr, const uint16_t* bytes,uint32_t size)
{
    uint8_t *array = new (std::nothrow)uint8_t[size*2];
    if (array==0)//TODO Test for fail in real hardware
      {
        return I2C::ErrorCode::GENERAL_ERROR;
      }
    for(uint32_t i=0,j=0;i<size;i++)
      {
        array[j++]=(bytes[i]>>8);
      array[j++]=(bytes[i]&0xff);
      }
    return Send_Bytes(addr,array,size*2);
//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//  Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  //uint32_t t;
//  //procedure required by hardware to clear Addr bit.
//  Get_Status1_Reg();
//  Get_Status2_Reg();
//
//  uint8_t *array = new (std::nothrow)uint8_t[size*2];
//  if (array==0)//TODO Test for fail in real hardware
//    {
//      return I2C::ErrorCode::GENERAL_ERROR;
//    }
//  for(uint32_t i=0,j=0;i<size;i++)
//    {
//      array[j++]=(bytes[i]>>8);
//    array[j++]=(bytes[i]&0xff);
//    }
//  for (uint32_t i=0;i<size*2;i++)
//    {
//      Send_Byte (array[i]);
//      error=Check_Errors_After_Data();
//      if(error!=ErrorCode::OK)
//        {
//          return error;
//        }
//    }
//
//  delete array;
//  Generate_Stop();
//  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Cont (uint8_t addr, const uint8_t* bytes, uint32_t size,
			  uint8_t mem_addr)
{
  //std::array<uint8_t,size*2+1>{mem_addr,bytes};
   //uint8_t *array = new (std::nothrow)uint8_t[size+1]{mem_addr,bytes};
//  if (array==0)//TODO Test for fail in real hardware
//    {
//      return I2C::ErrorCode::GENERAL_ERROR;
//    }
return Send_Bytes(addr,bytes,size,&mem_addr,1);

//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//    Send_Address (addr);
//    error=Check_Errors_After_Addr();
//    if(error!=ErrorCode::OK)
//      {
//        return error;
//      }
//    //uint32_t t;
//    //procedure required by hardware to clear Addr bit.
//    Get_Status1_Reg();
//    Get_Status2_Reg();
//
//
//    Send_Byte (mem_addr);
//    error=Check_Errors_After_Data();
//    if(error!=ErrorCode::OK)
//      {
//        return error;
//      }
//
//    for (uint32_t i=0;i<size;i++)
//      {
//        Send_Byte (bytes[i]);
//        error=Check_Errors_After_Data();
//        if(error!=ErrorCode::OK)
//          {
//            return error;
//          }
//      }
//    Generate_Stop();
//    return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Cont (uint8_t addr, const uint16_t* bytes, uint32_t size,
			  uint8_t mem_addr)
{
//    uint8_t *array = new (std::nothrow)uint8_t[size*2];
//    if (array==0)//TODO Test for fail in real hardware
//      {
//        return I2C::ErrorCode::GENERAL_ERROR;
//      }
//    for(uint32_t i=0,j=0;i<size;i++)
//      {
//        array[j++]=(bytes[i]>>8);
//      array[j++]=(bytes[i]&0xff);
//      }
//  return Send_Bytes(addr,bytes,size,&mem_addr,1);
//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//  Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//  //uint32_t t;
//  //procedure required by hardware to clear Addr bit.
//  Get_Status1_Reg();
//  Get_Status2_Reg();
//
//
//  Send_Byte (mem_addr);
//  error=Check_Errors_After_Data();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//
//  uint8_t *array = new (std::nothrow)uint8_t[size*2];
//  if (array==0)//TODO Test for fail in real hardware
//    {
//      return I2C::ErrorCode::GENERAL_ERROR;
//    }
//  for(uint32_t i=0,j=0;i<size;i++)
//    {
//      array[j++]=(bytes[i]>>8);
//    array[j++]=(bytes[i]&0xff);
//    }
//  for (uint32_t i=0;i<size*2;i++)
//    {
//      Send_Byte (array[i]);
//      error=Check_Errors_After_Data();
//      if(error!=ErrorCode::OK)
//        {
//          return error;
//        }
//    }
//
//  delete array;
//  Generate_Stop();
//  return I2C::ErrorCode::OK;
}


I2C::ErrorCode I2C::Send_Data_Circular (uint8_t addr, const uint8_t* byte, uint32_t size)
{
  if(dma==0)
    {
      return I2C::ErrorCode::DMA_DISABLED;
    }
  Allocate_Bytes_DMA(byte,size,true);//TODO fix name


  i=0;
  Generate_Start();
  while (Get_Status_Start_Bit () == 0)
    {
      i++;
      if (i > timeout * 1000)
	{
	  return I2C::ErrorCode::TIMEOUT;
	}
      I2C::delay (1);
    }
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }

  Get_Status1_Reg();
  Get_Status2_Reg();


  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size)
{
  if(dma==0)
    {
      return I2C::ErrorCode::DMA_DISABLED;
    }

  //Arrange data into 8bit array.
  uint8_t *array = new (std::nothrow)uint8_t[size*2];
    if (array==0)//TODO Test for fail in real hardware
      {
        return I2C::ErrorCode::GENERAL_ERROR;
      }
    for(uint32_t i=0,j=0;i<size;i++)
      {
        array[j++]=(byte[i]>>8);
      array[j++]=(byte[i]&0xff);
      }
  Allocate_Bytes_DMA(array,size*2,true);//TODO fix name
  //delete array;//If deleted DMA can't send


  i=0;
  Generate_Start();
  while (Get_Status_Start_Bit () == 0)
    {
      i++;
      if (i > timeout * 1000)
	{
	  return I2C::ErrorCode::TIMEOUT;
	}
      I2C::delay (1);
    }



  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }

  Get_Status1_Reg();
  Get_Status2_Reg();

  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Circular (uint8_t addr, const uint8_t* byte, uint32_t size,uint8_t mem_addr)
{
  if(dma==0)
    {
      return I2C::ErrorCode::DMA_DISABLED;
    }


  Allocate_Bytes_DMA(byte,size,true);//TODO fix name

  i=0;
  Generate_Start();
  while (Get_Status_Start_Bit () == 0)
    {
      i++;
      if (i > timeout * 1000)
	{
	  return I2C::ErrorCode::TIMEOUT;
	}
      I2C::delay (1);
    }

  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }

  Get_Status1_Reg();
  Get_Status2_Reg();
  Send_Byte (mem_addr);//not sure

  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Circular(uint8_t addr, const uint16_t *byte,uint32_t size,uint8_t mem_addr)
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  uint8_t *array = new (std::nothrow)uint8_t[size*2];//TODO assign pointer to this so it can be deleted outside scope
  if (array==0)//TODO Test for fail in real hardware
    {
      return I2C::ErrorCode::GENERAL_ERROR;
    }
  for(uint32_t i=0,j=0;i<size;i++)
    {
      array[j++]=(bytes[i]>>8);
    array[j++]=(bytes[i]&0xff);
    }
return Send_Bytes(addr,bytes,size,&mem_addr,1);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  if(dma==0)
//    {
//      return I2C::ErrorCode::DMA_DISABLED;
//    }
//
//  uint8_t *array = new (std::nothrow)uint8_t[size*2+1];//1 because of memory adress
//    if (array==0)//TODO Test for fail in real hardware
//      {
//        return I2C::ErrorCode::GENERAL_ERROR;
//      }
//    array[0]=mem_addr;
//    for(uint32_t i=0,j=1;i<size;i++)
//      {
//        array[j++]=(byte[i]>>8);
//      array[j++]=(byte[i]&0xff);
//      }
//
//  Allocate_Bytes_DMA(array,size*2+1,true);
//  //delete array;//it cannot be deleted!! what with dma- source ummm?
////TODO write this^^^ in docs. allocating data for >8bit
//
//  i=0;
//  Generate_Start();
//  while (Get_Status_Start_Bit () == 0)
//    {
//      i++;
//      if (i > timeout * 1000)
//	{
//	  return I2C::ErrorCode::TIMEOUT;
//	}
//      I2C::delay (1);
//    }
//
//  Send_Address (addr);
//  error=Check_Errors_After_Addr();
//  if(error!=ErrorCode::OK)
//    {
//      return error;
//    }
//
//  Get_Status1_Reg();
//  Get_Status2_Reg();
//
//  return I2C::ErrorCode::OK;


}


inline void I2C::Check_Errors_ISR (I2C& i2c)
{
//TODO maybe write some more solutions for errors to handle them without external code?
  if (i2c.Get_Status_Bus_Busy_Bit () != 0)
    {
      i2c.lasterror=ErrorCode::BUS_BUSY;
    }
  if (i2c.Get_Status_Bus_Error_Bit () != 0)
    {
      i2c.lasterror=ErrorCode::BUS_ERROR;
    }
  if (i2c.Get_Status_Arbitration_Lost_Bit () != 0)
    {
      i2c.lasterror=ErrorCode::ARBITION_LOST;
    }
  if (i2c.Get_Status_NACK_Bit () != 0)
    {
      i2c.lasterror=ErrorCode::NACK;
    }
//  if (i2c.circular !=0)
//    {
//
//    }
}


