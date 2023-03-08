/**
  ******************************************************************************
  * @file    MCP47FEB_hardware.cpp
  * @author  Rafał Mazurkiewicz
  * @date    02.10.2019
  * @brief   Class hardware implementation file for DAC based on MCP47FEB chip
  ******************************************************************************
  * @attention
  * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
  ******************************************************************************
  */

#include <MCP47FEB.hpp>
#include "stm32f1xx_hal.h"

void MCP47FEB::Enable_Output(void)
{
  GPIOB->ODR |= (1 << 5);
  HAL_Delay(5);
  GPIOB->ODR &= ~(1 << 5);
}

void MCP47FEB::Disable_Output(void)
{
  GPIOB->ODR |= (1 << 5);
}

//wiper lock--> HVC pin high
