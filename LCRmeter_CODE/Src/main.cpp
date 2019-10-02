/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/**@mainpage This project contains source code for simple and cheap LCRmeter
 * It is capable of measuring resistance, inductance, capacitance, as well as
 * outputs 8MHz clock from oscillator, and PWM. For more hardware related stuff go to
 * LCRmeter_PCB directory outside this folder.
 * \ref md_LICENSE.md
 */

/* USER CODE END Header */
/*TODO list
Buttons
osciloscope
menu
lcr calculations
dac sine generation
dac latch/HW pins
display waweforms
*/
/* Includes ------------------------------------------------------------------*/

#include <MCP4725.hpp>
#include <MCP47FEB.hpp>
#include "main.h"
#include "usb_device.h"
#include "Pwm.hpp"
#include "stm32f1xx.h"
#include "DMA.hpp"
#include "I2C.hpp"
#include "delay.h"
#include <stdio.h>
#include "sine.hpp"
#include "SSD1306.hpp"
#include <string>
//#include <adc.h>
//#include <dma.h>
//#include "i2c.h"
/* Private function prototypes -----------------------------------------------*/

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);

extern "C" {
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

  /* USER CODE END DMA1_Channel4_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}
}
//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);

ADC_HandleTypeDef adc;

#if 0
template<typename port_type,
typename bval_type,
const port_type port,
const bval_type bval>
  class led_template
  {
  public:
    led_template ()
    {
// Set the port pin value to low.
     // *reinterpret_cast<volatile bval_type*> (port) &=
	  //static_cast<bval_type> (~bval);
// Set the port pin direction to output.
      //*reinterpret_cast<volatile bval_type*> (pdir) |= bval;
    }
    static void
    toggle ()
    {
// Toggle the LED.
      *reinterpret_cast<volatile bval_type*> (port) ^= bval;
    }
  private:
    static constexpr port_type pdir = port - 1U;
  };



namespace
{
  const led_template<uint32_t, uint32_t, &uint32_t(GPIOC->ODR), (1<<13)> led_c13;
}

#endif

const static uint8_t sandals[1024] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x30, 0x10, 0x08, 0x08, 0x04, 0x04, 0x06, 0x02,
    0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x07, 0x09, 0x31, 0x21, 0x41, 0x81, 0x41, 0x41,
    0x21, 0x11, 0x19, 0x09, 0x05, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x87, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xC0, 0x30, 0x0C, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
    0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x06, 0x02, 0x01, 0x01, 0x00, 0x01, 0x02,
    0x04, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xC0, 0x40, 0x20, 0x20, 0x30, 0x10, 0x18, 0x08, 0x08, 0x0C, 0x04, 0x06,
    0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7F, 0x40, 0x40, 0x40, 0x40, 0x40, 0x60, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x30,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x12, 0x12, 0x11, 0x11, 0x10, 0x30,
    0x60, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06, 0x0C, 0x10, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x18,
    0x0C, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
    0xC0, 0x40, 0x20, 0xA0, 0xF0, 0xFC, 0xCE, 0x48, 0xFC, 0x06, 0x82, 0xC2, 0x21, 0x31, 0x0D, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x40, 0x40, 0x40, 0xC0, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x81, 0x81, 0x01, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x08, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x70, 0x10, 0x30, 0x20, 0x40, 0xC0, 0xC0, 0xC0, 0x60,
    0xD0, 0xD0, 0xF8, 0xE8, 0xF8, 0x54, 0xAC, 0xBC, 0x5A, 0xAE, 0x6A, 0x56, 0xB3, 0xEB, 0xFD, 0xF5,
    0xFC, 0x7E, 0xBF, 0xBF, 0x8F, 0xCB, 0xA7, 0xE5, 0x52, 0x31, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xA0, 0x90,
    0x88, 0x84, 0x84, 0x86, 0x82, 0x82, 0x82, 0x83, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81,
    0x83, 0x82, 0x83, 0x81, 0x80, 0x81, 0x83, 0x82, 0x86, 0x84, 0x84, 0x88, 0x88, 0x90, 0x90, 0x90,
    0x90, 0x98, 0x8C, 0x86, 0x82, 0x82, 0x82, 0x82, 0x81, 0x83, 0x86, 0x8C, 0x98, 0xB0, 0xE0, 0xC0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xE0, 0x3C, 0x03, 0x80, 0xC0, 0x20, 0x90, 0xD9, 0xAD, 0x64, 0xDA, 0xC9,
    0x67, 0xD6, 0xD6, 0xEB, 0xE9, 0xDD, 0x7A, 0x6D, 0xFD, 0x7E, 0x7E, 0x3F, 0x3F, 0x3F, 0x3F, 0x2F,
    0x2F, 0x23, 0x23, 0x21, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 0xC0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0x7C, 0x40, 0x38, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x03, 0x06, 0x04, 0x08, 0x18, 0x30, 0x60, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x03,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x82, 0x03, 0x03, 0x0F, 0x8B, 0xCE, 0x66, 0x36, 0x2F, 0x1F, 0x8D,
    0xC7, 0x27, 0x25, 0x17, 0x0D, 0x0F, 0x09, 0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xC0, 0x60, 0x30, 0x18, 0x0C, 0x07, 0x1E, 0x12, 0x11, 0x08, 0x08, 0x04, 0x04, 0x02, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int adc_read(uint32_t channel)
{
 ADC_ChannelConfTypeDef adc_ch;
 adc_ch.Channel = channel;
 adc_ch.Rank = ADC_REGULAR_RANK_1;
 adc_ch.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
 HAL_ADC_ConfigChannel(&adc, &adc_ch);

    HAL_ADC_Start(&adc);
 HAL_ADC_PollForConversion(&adc, 1000);
    return HAL_ADC_GetValue(&adc);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  char buf[30];


  /* Configure the system clock */
  SystemClock_Config();
  MX_GPIO_Init();

  __HAL_RCC_TIM1_CLK_ENABLE();
  printf("asdczx");
GPIOC->ODR^=(1<<13);
HAL_Delay(40);
GPIOC->ODR^=(1<<13);

// Pwm<TIM_TypeDef, uint16_t, 2> pwm (TIM1, 100);
// pwm.Initialise();//one timer-two channels->thus only one initialization TODO fix typo
// pwm.Set_Frequency(5000);
// pwm.Set_Duty(0);
// pwm.Enable();

 ///////////////////////////////////////////////////////////////////////////
///	I2C
//////////////////////////////////////////////////////////////////////////

 __HAL_RCC_I2C1_CLK_ENABLE();
 __HAL_RCC_I2C2_CLK_ENABLE();

 uint8_t data[]={ 0xAE,0xd5,0x80,0xA8,0x1F};
 volatile I2C::ErrorCode er;

// I2C i2c1(I2C1);
// i2c1.Initialise();
// i2c1.Disable_DMA();
// i2c1.Enable();

 I2C i2c2(I2C2);
// i2c2.Initialise();
// i2c2.Disable_DMA();
// i2c2.Enable();

// I2C_InitTypeDef hi2c2;
//
// hi2c2.ClockSpeed=300000;
// hi2c2.
 I2C_HandleTypeDef i2c;

  i2c.Instance             = I2C2;
  i2c.Init.ClockSpeed      = 240000;
  i2c.Init.DutyCycle       = I2C_DUTYCYCLE_2;
  i2c.Init.OwnAddress1     = 0xff;
  i2c.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  i2c.Init.OwnAddress2     = 0xff;
  i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  i2c.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

  HAL_I2C_Init(&i2c);
// HAL_I2C_Master_Transmit(&hi2c1,0x44,&u,1,200);

// i2c2.Send_Data(uint8_t(0x78),uint8_t(0x33),uint8_t(0x00));
// i2c.Send_Data(uint8_t(0x78),uint8_t(0x33),uint8_t(0x00));
// i2c.Send_Data(uint8_t(0x78),uint8_t(0x56));
// i2c.Send_Data(uint8_t(0x78),uint16_t(0x3449));
/////////////////////////////////////////////////////////////
  volatile int err;

 SSD1306 oled(&i2c2,64);
 //SSD1306 led(i2c1,32,SSD1306::HardwareConf::SEQ_NOREMAP);

 //printf("asdczx");
 oled.Initialize();
 err=oled.Get_Last_Error();
 if(err)
   {
     er=I2C::ErrorCode::BUS_BUSY;
   }

// led.Initialize();
 //err=led.Get_Last_Error();

 //oled.Set_Font_size(Fonts::Font_16x26);
 err=oled.IsInitialized();
// err=led.IsInitialized();
// printf("asdczx");
 oled.Fill(SSD1306::WHITE);
 oled.Update_Screen();
 //led.Fill(SSD1306::WHITE);
// led.Update_Screen();
 err=oled.Get_Last_Error();
// err=led.Get_Last_Error();
 delay_ms(200);
 oled.Fill(SSD1306::BLACK);
 oled.Update_Screen();

 oled.Update_Screen();
 delay_ms(500);
 oled.Set_Cursor(0,0);
 oled.Write_String("lorem");
 oled.Update_Screen();
 delay_ms(200);
 oled.Set_Cursor(50,16);
 oled.Write_String_Inverted("ipsum");
 oled.Set_Cursor(0,40);
 oled.Write_String_Inverted("l");
 oled.Update_Screen();

 oled.Draw_Image(sandals);

 oled.Update_Screen();



 //////////////////////////////////////////////////////////////////////////////////////////////
 /////DAC
  DMA dma(DMA1_Channel6);

  dma.Set_Size_Memory(DMA::Size::HALF_WORD);
  dma.Set_Minc(0);

  I2C i2c_dma(I2C1,&dma);
  i2c_dma.Initialise();
  i2c_dma.Enable_DMA();
  i2c_dma.Enable();


 MCP4725 dac_dma(i2c_dma);
 MCP47FEB dac(i2c_dma);
// uint16_t data2[]={ 0xAE,0xd5,0x80,0xA8,0x1F};
 //dac_dma.Set_Continuous(sine_table,sine_table_lenght);
 ////////////////////////////////////////////////////////////////////////////////////////////
 //uint32_t h=dac_dma.Set_Output(2000);
 uint32_t h=dac.Set_Output(50);
 sprintf(buf,"E = %d", h);
              oled.Set_Cursor(0,0),
              oled.Write_String(buf);
              oled.Update_Screen();
              HAL_Delay(600);
              //dac_dma.Reset();
              //dac_dma.Set_Power_mode(MCP4725::PowerMode::GND_1K);
              //dac_dma.Set_Output(1000);

              //latch pin
              GPIOB->ODR |=(1<<5);
              HAL_Delay(50);
              GPIOB->ODR &=~(1<<5);
 ///////////////////////////////////////////////////////////////////////////////////
 //ADC
 //////////////////////////////////////////////////////
              volatile uint32_t adc_value[10];
              int retval=0;
             // char buf[30];

              MX_GPIO_Init();
              MX_DMA_Init();
              MX_ADC1_Init();
              MX_ADC2_Init();
              //MX_I2C1_Init();
             // MX_I2C2_Init();
              /* USER CODE BEGIN 2 */
             retval= HAL_ADC_Start(&hadc1);
             if(retval!=0)
               {
                 asm("bkpt 255");
               }
             retval=HAL_ADC_Start(&hadc2);
             if(retval!=0)
               {
                 asm("bkpt 255");
               }
             retval=HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)adc_value, 10);
             if(retval!=0)
               {
                 asm("bkpt 255");
               }

             while (1)
             {
               /* USER CODE END WHILE */
                       sprintf(buf,"ADC1=%2d  ;	%2d  ", ((adc_value[0]>>16)& 0xffff), (adc_value[0]&0xffff));
                       oled.Set_Cursor(0,0),
                       oled.Write_String(buf);
                       sprintf(buf,"ADC1=%d;	%d", ((adc_value[1]>>16)& 0xffff), (adc_value[1]&0xffff));
                       oled.Set_Cursor(0,20),
                       oled.Write_String(buf);
                       oled.Update_Screen();
               /* USER CODE BEGIN 3 */
             }
// asm("bkpt 255");

// DMA_HandleTypeDef dma_init;
// //ADC_HandleTypeDef adc;
//
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     __HAL_RCC_GPIOB_CLK_ENABLE();
//     __HAL_RCC_USART2_CLK_ENABLE();
//     __HAL_RCC_DMA1_CLK_ENABLE();
//     __HAL_RCC_ADC1_CLK_ENABLE();
//
//     GPIO_InitTypeDef gpio;
//
//     gpio.Mode = GPIO_MODE_ANALOG;
//     gpio.Pin =  GPIO_PIN_0|GPIO_PIN_1;
//     HAL_GPIO_Init(GPIOB, &gpio);
//
//     gpio.Mode = GPIO_MODE_ANALOG;
//     gpio.Pin =  GPIO_PIN_7;
//     HAL_GPIO_Init(GPIOA, &gpio);
//
//     gpio.Mode = GPIO_MODE_OUTPUT_PP;
//     gpio.Pin =  GPIO_PIN_5;
//     HAL_GPIO_Init(GPIOB, &gpio);
//     GPIOD->ODR|=(1<<5);
//
//
//     RCC_PeriphCLKInitTypeDef adc_clk;
//     adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
//     adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV6;
//     HAL_RCCEx_PeriphCLKConfig(&adc_clk);
//
//     adc.Instance = ADC1;
//     adc.Init.ContinuousConvMode = DISABLE;
//     adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//     adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//     adc.Init.ScanConvMode = ADC_SCAN_DISABLE;
//     adc.Init.NbrOfConversion = 1;
//     adc.Init.DiscontinuousConvMode = DISABLE;
//     adc.Init.NbrOfDiscConversion = 1;
//     HAL_ADC_Init(&adc);
//
//     HAL_ADCEx_Calibration_Start(&adc);


//     RCC_PeriphCLKInitTypeDef adc_clk;
//     adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
//     adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV6;
//     HAL_RCCEx_PeriphCLKConfig(&adc_clk);
//
//     adc.Instance = ADC1;
//     adc.Init.ContinuousConvMode = ENABLE;
//     adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//     adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//     //adc.Init.ScanConvMode = ADC_SCAN_ENABLE;
//     adc.Init.NbrOfConversion = 1;
//     adc.Init.DiscontinuousConvMode = DISABLE;
//     adc.Init.NbrOfDiscConversion = 0;
//     HAL_ADC_Init(&adc);
//
//
//
//     ADC_ChannelConfTypeDef adc_ch;
//     adc_ch.Channel = ADC_CHANNEL_7;
//     adc_ch.Rank = ADC_REGULAR_RANK_1;
//     adc_ch.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
//     HAL_ADC_ConfigChannel(&adc, &adc_ch);
//
//     HAL_ADCEx_Calibration_Start(&adc);
//
////     adc_ch.Channel = ADC_CHANNEL_1;
////     adc_ch.Rank = ADC_REGULAR_RANK_2;
////     HAL_ADC_ConfigChannel(&adc, &adc_ch);
////     HAL_ADCEx_Calibration_Start(&adc);
//
//     dma_init.Instance = DMA1_Channel1;
//     dma_init.Init.Direction = DMA_PERIPH_TO_MEMORY;
//     dma_init.Init.PeriphInc = DMA_PINC_DISABLE;
//     dma_init.Init.MemInc = DMA_MINC_ENABLE;
//     dma_init.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//     dma_init.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//     dma_init.Init.Mode = DMA_NORMAL;
//     dma_init.Init.Priority = DMA_PRIORITY_HIGH;
//     HAL_DMA_Init(&dma_init);
//     __HAL_LINKDMA(&adc, DMA_Handle, dma_init);
//
//     HAL_ADC_Start_DMA(&adc, (uint32_t*)adc_value, 2);

     uint16_t value = HAL_ADC_GetValue(&adc);
     float v ;


     while (1)
       {
	  value = adc_read(ADC_CHANNEL_8);
	  v = (float)value * 3.3f / 4096.0f;
	  printf("ADC0 = %d (%.3fV) ", value, v);
	  HAL_Delay(20);
	 oled.Clean();
	 //value = adc_value[0];
	 value = adc_read(ADC_CHANNEL_0);
	 v = (float)value * 3.3f / 4096.0f;
             sprintf(buf,"ADC%d = %d", 0, value);
             oled.Set_Cursor(0,0),
             oled.Write_String(buf);
             value = adc_read(ADC_CHANNEL_9);
             sprintf(buf,"ADC%d = %d", 1, adc_value[1]);
             oled.Set_Cursor(0,20),
             oled.Write_String(buf);
             sprintf(buf,"ADC = %f", v);
             oled.Set_Cursor(0,40),
             oled.Write_String(buf);
             oled.Update_Screen();
     }

 while(1);

// HAL_I2C_Master_Transmit(&hi2c1,0x44,&u,1,200);

     /* I2C1 interrupt Init */
  //   HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

 volatile uint32_t jk=I2C1->SR1;
 printf("\n\r sds kjhui%d",jk);

 I2C1->DR=0xc0;//bylo 78


 while((I2C1->SR1 & I2C_SR1_ADDR)==0);

 jk=I2C1->SR1;
  //printf("\n\rsds%d",jk);
  jk=I2C1->SR2;
  // printf("\n\rsds%d",jk);

 I2C1->DR=0x07;//ae
 while((I2C1->SR1 & I2C_SR1_TXE)==0);
 I2C1->DR=0xd0;//d5
 while((I2C1->SR1 & I2C_SR1_TXE)==0);
// I2C1->DR=0x80;
// while((I2C1->SR1 & I2C_SR1_TXE)==0);
// I2C1->DR=0xA8;
// while((I2C1->SR1 & I2C_SR1_TXE)==0);
// I2C1->DR=0x1F;
// while((I2C1->SR1 & I2C_SR1_TXE)==0);

 I2C1->CR1|=I2C_CR1_STOP;

 ///////////////////////////////////////////////////////////////////////////
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      delay_ms(200);
    /* USER CODE EN
     * D WHILE */
      GPIOC->ODR^=(1<<13);
      printf("sdshm  mj");
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
  /**Enables the Clock Security System
  */
 HAL_RCC_EnableCSS();



}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  //I2C

  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /**Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure the ADC multi-mode
  */
  multimode.Mode = ADC_DUALMODE_REGSIMULT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /**Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  /**I2C2 GPIO Configuration
  PB10   ------> I2C2_SCL
  PB11   ------> I2C2_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

  /* I2C2 DMA Init */

  /* I2C2_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MDATAALIGN_BYTE);

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  /**I2C Initialization
  */
  LL_I2C_DisableOwnAddress2(I2C2);
  LL_I2C_DisableGeneralCall(I2C2);
  LL_I2C_EnableClockStretching(I2C2);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 400000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C2, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C2, 0);
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
