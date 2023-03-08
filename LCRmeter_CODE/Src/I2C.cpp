/*
 * I2C.cpp
 *
 *  Created on: 12.05.2019
 *      Author: Rafał Mazurkiewicz
 */

#include <I2C.hpp>
#include <array>

I2C::ErrorCode I2C::Send_Data(uint8_t addr, uint8_t byte)
{
	return Send_Bytes(addr, &byte, 1);
}

I2C::ErrorCode I2C::Send_Data(uint8_t addr, uint16_t byte)
{
	uint8_t arr[2] =
	{ uint8_t(byte >> 8), uint8_t(byte & 0xff) };
	return Send_Bytes(addr, &arr[0], 2);
}

I2C::ErrorCode I2C::Send_Data(uint8_t addr, uint8_t byte, uint8_t mem_addr)
{
	return Send_Bytes(addr, &byte, 1, &mem_addr, 1);
}

I2C::ErrorCode I2C::Send_Data(uint8_t addr, uint16_t byte, uint8_t mem_addr)
{
	uint8_t arr[2] =
	{ uint8_t(byte >> 8), uint8_t(byte & 0xff) };
	return Send_Bytes(addr, &arr[0], 2, &mem_addr, 1);
}

I2C::ErrorCode I2C::Send_Data_Cont(uint8_t addr, const uint8_t* bytes,
		uint32_t size)
{
	return Send_Bytes(addr, bytes, size);
}

I2C::ErrorCode I2C::Send_Data_Cont(uint8_t addr, const uint8_t* bytes,
		uint32_t size, uint8_t mem_addr)
{
	return Send_Bytes(addr, bytes, size, &mem_addr, 1);
}

///@note not recommended - uses memory allocation under the hood (16 bit)
I2C::ErrorCode I2C::Send_Data_Cont(uint8_t addr, const uint16_t* bytes,
		uint32_t size)
{
	uint8_t *array = new (std::nothrow) uint8_t[size * 2];
	if (array == 0)
	{
		return I2C::ErrorCode::GENERAL_ERROR;
	}
	for (uint32_t i = 0, j = 0; i < size; i++)
	{
		array[j++] = (bytes[i] >> 8);
		array[j++] = (bytes[i] & 0xff);
	}
	ErrorCode error = Send_Bytes(addr, array, size * 2);
	delete array;

	return error;
}

///@note not recommended - uses memory allocation under the hood (16 bit)
I2C::ErrorCode I2C::Send_Data_Cont(uint8_t addr, const uint16_t* bytes,
		uint32_t size, uint8_t mem_addr)
{
	uint8_t *array = new (std::nothrow) uint8_t[size * 2];
	if (array == 0)
	{
		return I2C::ErrorCode::GENERAL_ERROR;
	}
	for (uint32_t i = 0, j = 0; i < size; i++)
	{
		array[j++] = (bytes[i] >> 8);
		array[j++] = (bytes[i] & 0xff);
	}
	ErrorCode error = Send_Bytes(addr, array, size * 2, &mem_addr, 1);
	delete array;

	return error;
}

I2C::ErrorCode I2C::Send_Data_Circular(uint8_t addr, const uint8_t* byte,
		uint32_t size)
{
	if (dma == 0)
	{
		return I2C::ErrorCode::DMA_DISABLED;
	}
	return Send_Bytes_DMA(addr, byte, size);
}

I2C::ErrorCode I2C::Send_Data_Circular(uint8_t addr, const uint8_t* byte,
		uint32_t size, uint8_t mem_addr)
{
	if (dma == 0)
	{
		return I2C::ErrorCode::DMA_DISABLED;
	}
	return Send_Bytes_DMA(addr, byte, size, &mem_addr, 1);
}

///@note not recommended - uses memory allocation under the hood (16 bit)
I2C::ErrorCode I2C::Send_Data_Circular(uint8_t addr, const uint16_t *byte,
		uint32_t size)
{
	if (dma == 0)
	{
		return I2C::ErrorCode::DMA_DISABLED;
	}
	uint8_t *array = new (std::nothrow) uint8_t[size * 2]; ///TODO memory leak array is nowhere freed
	if (array == 0)
	{
		return I2C::ErrorCode::GENERAL_ERROR;
	}
	ptr_to_bytes = array;
	size_of_data = size * 2;
	for (uint32_t i = 0, j = 0; i < size; i++)
	{
		array[j++] = (byte[i] >> 8);
		array[j++] = (byte[i] & 0xff);
	}
	return Send_Bytes_DMA(addr, array, size * 2);

}

///@note not recommended - uses memory allocation under the hood (16 bit)
I2C::ErrorCode I2C::Send_Data_Circular(uint8_t addr, const uint16_t *byte,
		uint32_t size, uint8_t mem_addr)
{
	if (dma == 0)
	{
		return I2C::ErrorCode::DMA_DISABLED;
	}
	uint8_t *array = new (std::nothrow) uint8_t[size * 2];
	if (array == 0)
	{
		return I2C::ErrorCode::GENERAL_ERROR;
	}
	ptr_to_bytes = array;
	size_of_data = size * 2;
	for (uint32_t i = 0, j = 0; i < size; i++)
	{
		array[j++] = (byte[i] >> 8);
		array[j++] = (byte[i] & 0xff);
	}
	return Send_Bytes_DMA(addr, array, size * 2, &mem_addr, 1);
}

I2C::ErrorCode I2C::Send_Bytes(uint8_t address, const uint8_t *data, int size)
{
	uint32_t i;
	i = 0;
	Generate_Start();
	while (Get_Status_Start_Bit() == 0)
	{
		i++;
		if (i > timeout)
		{
			Generate_Stop();
			return I2C::ErrorCode::TIMEOUT;
		}
		I2C::Delay(1);
	}

	Send_Address(address);
	error = Check_Errors_After_Addr();
	if (error != ErrorCode::OK)
	{
		Generate_Stop();
		return error;
	}

	Get_Status1_Reg();
	Get_Status2_Reg();

	for (i = 0; i < size; i++)
	{
		Send_Byte(data[i]);
		error = Check_Errors_After_Data();
		if (error != ErrorCode::OK)
		{
			Generate_Stop();
			return error;
		}
	}

	Generate_Stop();
	return I2C::ErrorCode::OK;
}

///@warning Not working - mem address is ignored. If you fix it please make sure upper layer (DAC) is also updated.
I2C::ErrorCode I2C::Send_Bytes_DMA(uint8_t address, const uint8_t *data,
		int size, uint8_t *mem_bytes, int mem_size)
{
	uint32_t i;
	i = 0;
	Generate_Start();
	while (Get_Status_Start_Bit() == 0)
	{
		i++;
		if (i > timeout)
		{
			Generate_Stop();
			return I2C::ErrorCode::TIMEOUT;
		}
		I2C::Delay(1);
	}
	Send_Address(address);
	error = Check_Errors_After_Addr();
	if (error != ErrorCode::OK)
	{
		Generate_Stop();
		return error;
	}
	Get_Status1_Reg();
	Get_Status2_Reg();

	Allocate_Bytes_DMA(data, size);

	return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Send_Bytes(uint8_t address, const uint8_t *data, int size,
		uint8_t *mem_bytes, int mem_size)
{
	uint32_t i;
	i = 0;
	Generate_Start();
	while (Get_Status_Start_Bit() == 0)
	{
		i++;
		if (i > timeout)
		{
			Generate_Stop();
			return I2C::ErrorCode::TIMEOUT;
		}
		I2C::Delay(1);
	}
	Send_Address(address);
	error = Check_Errors_After_Addr();
	if (error != ErrorCode::OK)
	{
		Generate_Stop();
		return error;
	}
	Get_Status1_Reg();
	Get_Status2_Reg();
	for (i = 0; i < mem_size; i++)
	{
		Send_Byte(mem_bytes[i]);
		error = Check_Errors_After_Data();
		if (error != ErrorCode::OK)
		{
			Generate_Stop();
			return error;
		}
	}
	for (i = 0; i < size; i++)
	{
		Send_Byte(data[i]);
		error = Check_Errors_After_Data();
		if (error != ErrorCode::OK)
		{
			Generate_Stop();
			return error;
		}
	}
	Generate_Stop();
	return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Check_Errors_After_Data(void)
{
	uint32_t i = 0;
	while (Get_Status_TxE_Bit() == 0)
	{
		if (Get_Status_Arbitration_Lost_Bit() != 0)
		{
			return I2C::ErrorCode::ARBITION_LOST;
		}
		if (Get_Status_NACK_Bit() != 0)
		{
			return I2C::ErrorCode::NACK;
		}
		i++;
		if (i > timeout)
		{
			return I2C::ErrorCode::TIMEOUT;
		}
		Delay(1);
	}
	return I2C::ErrorCode::OK;
}

I2C::ErrorCode I2C::Check_Errors_After_Addr(void)
{
	uint32_t i = 0;
	while (Get_Status_Addr_Bit() == 0) //Timeout routine
	{
		if (Get_Status_Arbitration_Lost_Bit() != 0)
		{
			return I2C::ErrorCode::ARBITION_LOST;
		}
		if (Get_Status_NACK_Bit() != 0)
		{
			return I2C::ErrorCode::NACK;
		}
		i++;
		if (i > timeout)
		{
			return I2C::ErrorCode::TIMEOUT;
		}
		Delay(1);
	}
	return I2C::ErrorCode::OK;
}

inline void I2C::Check_Errors_ISR(I2C& i2c)
{
	if (i2c.Get_Status_Bus_Busy_Bit() != 0)
	{
		i2c.lasterror = ErrorCode::BUS_BUSY;
	}
	if (i2c.Get_Status_Bus_Error_Bit() != 0)
	{
		i2c.lasterror = ErrorCode::BUS_ERROR;
	}
	if (i2c.Get_Status_Arbitration_Lost_Bit() != 0)
	{
		i2c.lasterror = ErrorCode::ARBITION_LOST;
	}
	if (i2c.Get_Status_NACK_Bit() != 0)
	{
		i2c.lasterror = ErrorCode::NACK;
	}
//  if (i2c.circular !=0)
//    {
//
//    }
}

void I2C::Stop_Transfer(void)
{
	Stop_DMA();
	if (ptr_to_bytes != 0)
	{
		delete[] ptr_to_bytes;
		Generate_Stop();
	}
}

//TODO implement this Set_Frequency
void I2C::Set_Frequency(const uint32_t frequency)
{

}
