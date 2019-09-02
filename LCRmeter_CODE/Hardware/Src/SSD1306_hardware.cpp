/*
 * SSD1306_hardware.cpp
 *
 *  Created on: 25.08.2019
 *      Author: Rafa³
 */

#include <SSD1306.hpp>
#include "delay.h"
#include "stdio.h"

//TODO handle return values

void SSD1306::delay (uint32_t miliseconds)
{
  delay_ms(miliseconds);
}

void SSD1306::Write_Command (uint8_t com)
{
  temp=conn.Send_Data(address,com,control_b_command);
//  if(temp !=0)
//    {
//      last_error=temp;
//      GPIOC->ODR^=(1<<13);
//      delay(1);
//      GPIOC->ODR^=(1<<13);
//      delay(1);
//      GPIOC->ODR^=(1<<13);
//      delay(1);
//      printf("error=%d",last_error);
//    }
}

void SSD1306::Write_Data (std::array<uint8_t, SSD1306::buffer_size>  &data)
{
  temp=conn.Send_Data_Cont(address,data.begin(),buffer_size,control_b_data);
//  if(temp !=0)
//    {
//      last_error=temp;
//      GPIOC->ODR^=(1<<13);
//      delay(1);
//      GPIOC->ODR^=(1<<13);
//      delay(1);
//      GPIOC->ODR^=(1<<13);
//      delay(1);
//      printf("error=%d",last_error);
//    }
}

void SSD1306::Reset (void)
{
  //in I2C-empty instruction.
  //in SPI use:
  //TODO finish it
}
