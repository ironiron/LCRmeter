/*
 * I2C_test.cpp
 *
 *  Created on: 12.05.2019
 *      Author: Rafa�
 */



#include "../Inc/I2C.hpp"

#include "testing.hpp"

//#include "fakeit.hpp"
#include "catch.hpp"
#include <iostream>
using namespace std;

I2C::ErrorCode er;
DMA_TYPE_DEF *d=reinterpret_cast<DMA_TYPE_DEF*>(&er);//just make pointer point somewhere
I2C_TYPE_DEF i;
I2C i2c(&i);
I2C i2c_dma(&i,d);



//TODO send; error;dma+interrupt;dma+error;timeout
TEST_CASE( "sends data 8 bit")
{

  uint8_t addr=0x24;
  uint8_t data=0x45;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c.Send_Data(addr,data);

  REQUIRE(er==I2C::ErrorCode::OK);
  REQUIRE(testing::i2c::send_data.size()==2);
  REQUIRE(testing::i2c::send_data[0]==addr);
  REQUIRE(testing::i2c::send_data[1]==data);
}

TEST_CASE( "sends data 16 bit")
{
  uint8_t addr=0x24;
  uint16_t data=0x3256;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c.Send_Data(addr,data);

  REQUIRE(er==I2C::ErrorCode::OK);
  REQUIRE(testing::i2c::send_data.size()==3);
  REQUIRE(testing::i2c::send_data[0]==addr);
  REQUIRE(testing::i2c::send_data[1]==0x32);
  REQUIRE(testing::i2c::send_data[2]==0x56);
}

TEST_CASE( "sends data 16 bit + address 8 bit")
{
  uint8_t addr=0x24;
  uint16_t data=0x3256;
  uint8_t mem_addr=0x44;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c.Send_Data(addr,data,mem_addr);

  REQUIRE(er==I2C::ErrorCode::OK);
  REQUIRE(testing::i2c::send_data.size()==4);
  REQUIRE(testing::i2c::send_data[0]==addr);
  REQUIRE(testing::i2c::send_data[1]==mem_addr);
  REQUIRE(testing::i2c::send_data[2]==0x32);
  REQUIRE(testing::i2c::send_data[3]==0x56);
}

TEST_CASE( "sends data 16 bit + address 8 bit continuously")
{
  uint8_t addr=0x24;
  uint16_t data[3]={0x3256,0x0404,0x5589};
  uint8_t mem_addr=0x44;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c.Send_Data_Cont(addr,data,uint32_t(3),mem_addr);

  REQUIRE(er==I2C::ErrorCode::OK);
  REQUIRE(testing::i2c::send_data.size()==8);
  REQUIRE(testing::i2c::send_data[0]==addr);
  REQUIRE(testing::i2c::send_data[1]==mem_addr);
  REQUIRE(testing::i2c::send_data[2]==0x32);
  REQUIRE(testing::i2c::send_data[3]==0x56);
  REQUIRE(testing::i2c::send_data[4]==0x04);
  REQUIRE(testing::i2c::send_data[5]==0x04);
  REQUIRE(testing::i2c::send_data[6]==0x55);
  REQUIRE(testing::i2c::send_data[7]==0x89);
}


TEST_CASE( "return error when timeout occurs")
{
  testing::i2c::status_addr_bit=0;//set hardware bit which indicates that addres bit is not sent
  uint8_t addr=0x24;
  uint8_t data=0x45;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c.Send_Data(addr,data);
  REQUIRE(er==I2C::ErrorCode::TIMEOUT);
  testing::i2c::status_addr_bit=1;//clean
}

TEST_CASE( "return error when NACK occur")
{
  testing::i2c::status_nack_bit=1;//NACK occured, and no addr bit is set
  testing::i2c::status_addr_bit=0;
  uint8_t addr=0x24;
  uint8_t data=0x45;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c.Send_Data(addr,data);
  REQUIRE(er==I2C::ErrorCode::NACK);

  testing::i2c::status_addr_bit=1;//clean
  testing::i2c::status_nack_bit=0;
}

//no need anymore

//TEST_CASE( "return error when bus is busy")
//{
//  testing::i2c::status_TxE_bit=0;//bus_busy bit set while Transfer buffer not empty
//  testing::i2c::status_bus_busy_bit=1;
//  uint8_t addr=0x24;
//  uint8_t data=0x45;
//  testing::i2c::send_data.clear();
//
//  I2C::ErrorCode er=i2c.Send_Data(addr,data);
//  REQUIRE(er==I2C::ErrorCode::BUS_BUSY);
//
//  testing::i2c::status_TxE_bit=1;//clean
//  testing::i2c::status_bus_busy_bit=0;
//}

TEST_CASE( "abort sending circular data without DMA")
{
  uint8_t addr=0x24;
  uint16_t data[3]={0x3256,0x0404,0x5589};
  uint8_t mem_addr=0x44;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c.Send_Data_Circular(addr,data,uint32_t(3),mem_addr);

  REQUIRE(er==I2C::ErrorCode::DMA_DISABLED);
}

TEST_CASE( "Send data circular thru DMA")
{
  uint8_t addr=0x24;
  uint16_t data[3]={0x3256,0x0404,0x5589};
  uint8_t mem_addr=0x44;
  testing::i2c::send_data.clear();

  I2C::ErrorCode er=i2c_dma.Send_Data_Circular(addr,data,uint32_t(3),mem_addr);

  REQUIRE(er==I2C::ErrorCode::OK);
  REQUIRE(testing::i2c::send_data.size()==8);
  REQUIRE(testing::i2c::send_data[0]==addr);
  REQUIRE(testing::i2c::send_data[1]==mem_addr);
  REQUIRE(testing::i2c::send_data[2]==0x32);
  REQUIRE(testing::i2c::send_data[3]==0x56);
  REQUIRE(testing::i2c::send_data[4]==0x04);
  REQUIRE(testing::i2c::send_data[5]==0x04);
  REQUIRE(testing::i2c::send_data[6]==0x55);
  REQUIRE(testing::i2c::send_data[7]==0x89);
}

TEST_CASE( "Sets frequency of I2C")
{

}













//TEST_CASE( "handle errors while sending thru DMA")
//{
//  uint8_t addr=0x24;
//  uint16_t data[3]={0x3256,0x0404,0x5589};
//  uint8_t mem_addr=0x44;
//  testing::i2c::send_data.clear();
//  testing::i2c::status_nack_bit=1;
//
//  I2C::ErrorCode er=i2c_dma.Send_Data_Circular(addr,data,uint32_t(3),mem_addr);
//
//  REQUIRE(er==I2C::ErrorCode::OK);
//
//  I2C::Check_Errors_ISR(i2c_dma);
//
//  REQUIRE(i2c_dma.Get_Last_Error()==I2C::ErrorCode::NACK);
//
//}
