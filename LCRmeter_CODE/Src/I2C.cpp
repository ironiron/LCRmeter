/*
 * I2C.cpp
 *
 *  Created on: 12.05.2019
 *      Author: Rafa³
 */

#include <I2C.hpp>
#include "I2C_hardware.hpp"
#include <vector>
#include "fakes/DMA.hpp"

#include <iostream>
using namespace std;

I2C::ErrorCode I2C::Send_Data (uint8_t addr, uint8_t byte)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
    }
  Generate_Start();
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  //uint32_t t;
  //procedure required by hardware to clear Addr bit.
  Get_Status1_Reg();
  Get_Status2_Reg();

 Send_Byte (byte);
 error=Check_Errors_After_Data();
 if(error!=ErrorCode::OK)
   {
     return error;
   }

  Generate_Stop();
  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data (uint8_t addr, uint16_t byte)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
    }
  Generate_Start();
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  //uint32_t t;
  //procedure required by hardware to clear Addr bit.
  Get_Status1_Reg();
  Get_Status2_Reg();

  Send_Byte (byte>>8);
  error=Check_Errors_After_Data();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  Send_Byte (byte & 0xff);
  error=Check_Errors_After_Data();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  Generate_Stop();
  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data (uint8_t addr, uint8_t byte, uint8_t mem_addr)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
    }
  Generate_Start();
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  //uint32_t t;
  //procedure required by hardware to clear Addr bit.
  Get_Status1_Reg();
  Get_Status2_Reg();

  Send_Byte (mem_addr);
  error=Check_Errors_After_Data();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
 Send_Byte (byte);
 error=Check_Errors_After_Data();
 if(error!=ErrorCode::OK)
   {
     return error;
   }

  Generate_Stop();
  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data(uint8_t addr, uint16_t byte,uint8_t mem_addr)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
    }
  Generate_Start();
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  //uint32_t t;
  //procedure required by hardware to clear Addr bit.
  Get_Status1_Reg();
  Get_Status2_Reg();

  Send_Byte (mem_addr);
  error=Check_Errors_After_Data();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  Send_Byte (byte >> 8);
  error=Check_Errors_After_Data();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  Send_Byte (byte & 0xff);
  error=Check_Errors_After_Data();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  Generate_Stop();
  return I2C::ErrorCode::OK;
}


I2C::ErrorCode I2C::Send_Data_Cont (uint8_t addr, const uint8_t* bytes, uint32_t size)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
    }
  Generate_Start();
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  //uint32_t t;
  //procedure required by hardware to clear Addr bit.
  Get_Status1_Reg();
  Get_Status2_Reg();

  for (uint32_t i=0;i<size*2;i++)
    {
      Send_Byte (bytes[i]);
      error=Check_Errors_After_Data();
      if(error!=ErrorCode::OK)
        {
          return error;
        }
    }

  Generate_Stop();
  return I2C::ErrorCode::OK;
}
//not recommended - uses memory allocation under the hood (16 bit)
I2C::ErrorCode I2C::Send_Data_Cont(uint8_t addr, const uint16_t* bytes,uint32_t size)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
    }
  Generate_Start();
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  //uint32_t t;
  //procedure required by hardware to clear Addr bit.
  Get_Status1_Reg();
  Get_Status2_Reg();

  uint8_t *array = new (std::nothrow)uint8_t[size*2];
  if (array==NULL)//TODO Test for fail in real hardware
    {
      return I2C::ErrorCode::GENERAL_ERROR;
    }
  for(uint32_t i=0,j=0;i<size;i++)
    {
      array[j++]=(bytes[i]>>8);
    array[j++]=(bytes[i]&0xff);
    }
  for (uint32_t i=0;i<size*2;i++)
    {
      Send_Byte (array[i]);
      error=Check_Errors_After_Data();
      if(error!=ErrorCode::OK)
        {
          return error;
        }
    }

  delete array;
  Generate_Stop();
  return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Cont (uint8_t addr, const uint8_t* bytes, uint32_t size,
			  uint8_t mem_addr)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
      {
        reset ();
      }
    Generate_Start();
    Send_Address (addr);
    error=Check_Errors_After_Addr();
    if(error!=ErrorCode::OK)
      {
        return error;
      }
    //uint32_t t;
    //procedure required by hardware to clear Addr bit.
    Get_Status1_Reg();
    Get_Status2_Reg();


    Send_Byte (mem_addr);
    error=Check_Errors_After_Data();
    if(error!=ErrorCode::OK)
      {
        return error;
      }

    for (uint32_t i=0;i<size*2;i++)
      {
        Send_Byte (bytes[i]);
        error=Check_Errors_After_Data();
        if(error!=ErrorCode::OK)
          {
            return error;
          }
      }
    return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Data_Cont (uint8_t addr, const uint16_t* bytes, uint32_t size,
			  uint8_t mem_addr)
{
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
    }
  Generate_Start();
  Send_Address (addr);
  error=Check_Errors_After_Addr();
  if(error!=ErrorCode::OK)
    {
      return error;
    }
  //uint32_t t;
  //procedure required by hardware to clear Addr bit.
  Get_Status1_Reg();
  Get_Status2_Reg();


  Send_Byte (mem_addr);
  error=Check_Errors_After_Data();
  if(error!=ErrorCode::OK)
    {
      return error;
    }

  uint8_t *array = new (std::nothrow)uint8_t[size*2];
  if (array==NULL)//TODO Test for fail in real hardware
    {
      return I2C::ErrorCode::GENERAL_ERROR;
    }
  for(uint32_t i=0,j=0;i<size;i++)
    {
      array[j++]=(bytes[i]>>8);
    array[j++]=(bytes[i]&0xff);
    }
  for (uint32_t i=0;i<size*2;i++)
    {
      Send_Byte (array[i]);
      error=Check_Errors_After_Data();
      if(error!=ErrorCode::OK)
        {
          return error;
        }
    }

  delete array;
  return I2C::ErrorCode::OK;
}


I2C::ErrorCode I2C::Send_Data_Circular (uint8_t addr, const uint8_t* byte, uint32_t size)
{
  if(dma==0)
    {
      return I2C::ErrorCode::DMA_DISABLED;
    }
  Send_Bytes_DMA(byte,size,true);//TODO fix name


  Generate_Start();
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
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

  uint8_t *array = new (std::nothrow)uint8_t[size*2];
    if (array==NULL)//TODO Test for fail in real hardware
      {
        return I2C::ErrorCode::GENERAL_ERROR;
      }
    for(uint32_t i=0,j=0;i<size;i++)
      {
        array[j++]=(byte[i]>>8);
      array[j++]=(byte[i]&0xff);
      }
  Send_Bytes_DMA(array,size*2,true);//TODO fix name
  delete array;


  Generate_Start();
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
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


  Send_Bytes_DMA(byte,size,true);//TODO fix name

  Generate_Start();
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
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
  if(dma==0)
    {
      return I2C::ErrorCode::DMA_DISABLED;
    }

  uint8_t *array = new (std::nothrow)uint8_t[size*2+1];
    if (array==NULL)//TODO Test for fail in real hardware
      {
        return I2C::ErrorCode::GENERAL_ERROR;
      }
    array[0]=mem_addr;
    for(uint32_t i=0,j=1;i<size;i++)
      {
        array[j++]=(byte[i]>>8);
      array[j++]=(byte[i]&0xff);
      }
  Send_Bytes_DMA(array,size*2+1,true);//TODO fix name
  delete array;


  Generate_Start();
  if (Get_Status1_Reg() != 0)//Reset bus in case of errors.
    {
      reset ();
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


//inline void I2C::delay_us (uint32_t unsignedInt)
//{
//}
