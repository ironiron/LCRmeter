/*
 * I2C_test.cpp
 *
 *  Created on: 22.05.2019
 *      Author: Rafa³ Mazurkiewicz
 */
#include "I2C_hardware.hpp"
#include "I2C.hpp"

#include <vector>


void I2C::Send_Address (uint8_t addr, bool rw)
{

  i2c->DR=addr;
}

void I2C::Send_Byte (uint8_t byte)
{
  i2c->DR=byte;
}

I2C::ErrorCode I2C::Send_Bytes (uint8_t address,const uint8_t *data,int size)
{
  int i;
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

     Send_Address (address);
    error=Check_Errors_After_Addr();
    if(error!=ErrorCode::OK)
      {
        return error;
      }
    Get_Status1_Reg();
    Get_Status2_Reg();

    for (i=0;i<size;i++)
      {
	   Send_Byte (data[i]);
	   error=Check_Errors_After_Data();
	   if(error!=ErrorCode::OK)
	     {
	       return error;
	     }
      }

    Generate_Stop();
    return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Bytes (uint8_t address,const uint8_t *data,int size,uint8_t *mem_bytes,int mem_size)
{
  int i;
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
     Send_Address (address);
    error=Check_Errors_After_Addr();
    if(error!=ErrorCode::OK)
      {
        return error;
      }
    Get_Status1_Reg();
    Get_Status2_Reg();
    for (i=0;i<mem_size;i++)
      {
	   Send_Byte (mem_bytes[i]);
	   error=Check_Errors_After_Data();
	   if(error!=ErrorCode::OK)
	     {
	       return error;
	     }
      }
    for (i=0;i<size;i++)
      {
	   Send_Byte (data[i]);
	   error=Check_Errors_After_Data();
	   if(error!=ErrorCode::OK)
	     {
	       return error;
	     }
      }
    Generate_Stop();
    return I2C::ErrorCode::OK;
}

void I2C::Generate_Start (void)
{
  i2c->CR1|=I2C_CR1_START;
}

void I2C::Allocate_Bytes_DMA(const uint8_t* bytes,uint32_t size,bool circular)
{
  dma->Set_Direction(true);
  dma->Set_Minc(true);
  dma->Set_Data_Count(size);
  dma->Set_Size_Memory(DMA::Size::BYTE);
  if(circular==true)
    {
      dma->Circular_Enable(true);
    }
  else
  {
      dma->Circular_Enable(false);
  }

  dma->Set_Peripheral_Addr((uint32_t)(&i2c->DR));
  dma->Set_Memory_Addr((uint32_t)(bytes));
  dma->Enable();
}

void I2C::Generate_Stop (void)
{
  i2c->CR1|=I2C_CR1_STOP;
}

void I2C::Enable (void)
{
  i2c->CR1|= I2C_CR1_PE;
}

void I2C::Disable (void)
{
  i2c->CR1&=~ I2C_CR1_PE;
}

void I2C::Initialise (void)
{

  i2c->CR1|= I2C_CR1_SWRST;
  HAL_Delay(50);
  i2c->CR1&=~ I2C_CR1_SWRST;
  i2c->CR2|=36;//TODO implement method for this<< dma.
//CCR = 50ns
  i2c->CCR|=I2C_CCR_FS| I2C_CCR_DUTY | 6;
  i2c->TRISE|=9;
//TODO clean the mess

  /*Initialise in full speed 400kHz
   * freq=20MHz=>TPCLK1=50ns
   * Thigh = 9 * CCR * TPCLK1
   * Tlow = 16 * CCR * TPCLK1
   * tr(SCL)=300ns
   * tw(SCLH)=1,3us
   * thigh = tr(SCL) + tw(SCLH)=1,6us
   * tf(SCL)=300ns
   * tw(SCLL)=0,6us
   * tlow = tr(SCL) + tw(SCLH)=0,9us
   * CCR~4
   */

  /*  i2c->CR2|=36;
   //CCR = 50ns
   i2c->CCR|=I2C_CCR_F/S| I2C_CCR_DUTY | 4;
   i2c->TRISE|=7;
   i2c->CR1|=I2C_CR1_ACK |I2C_CR1_START| I2C_CR1_PE;*/
}
//TODO finish this function \/
void I2C::Set_Frequency (const uint32_t constUnsignedInt)
{

}

void I2C::Enable_DMA(void)
{
  i2c->CR2 |=I2C_CR2_DMAEN;
}

void I2C::Disable_DMA(void)
{
  i2c->CR2 &=~ I2C_CR2_DMAEN;
}

