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



/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "Pwm.hpp"
#include "stm32f1xx.h"
#include "DMA.hpp"
#include "MCP47.hpp"
#include "I2C.hpp"
#include "delay.h"
#include <stdio.h>
#include "sine.hpp"
#include "SSD1306.hpp"
///aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

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
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{


  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

 // MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
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
 __HAL_RCC_I2C1_CLK_ENABLE();
 __HAL_RCC_I2C2_CLK_ENABLE();
 __HAL_RCC_DMA1_CLK_ENABLE();

 uint8_t data[]={ 0xAE,0xd5,0x80,0xA8,0x1F};
 volatile I2C::ErrorCode er;

 I2C i2c(I2C1);
 i2c.Initialise();
 i2c.Disable_DMA();
 i2c.Enable();

 I2C i2c2(I2C2);
 i2c2.Initialise();
 i2c2.Disable_DMA();
 i2c2.Enable();
 I2C_HandleTypeDef hi2c1;
// HAL_I2C_Master_Transmit(&hi2c1,0x44,&u,1,200);

// i2c2.Send_Data(uint8_t(0x78),uint8_t(0x33),uint8_t(0x00));
// i2c.Send_Data(uint8_t(0x78),uint8_t(0x33),uint8_t(0x00));
// i2c.Send_Data(uint8_t(0x78),uint8_t(0x56));
// i2c.Send_Data(uint8_t(0x78),uint16_t(0x3449));
/////////////////////////////////////////////////////////////
int err;

 SSD1306 oled(64,i2c2);
 SSD1306 led(32,i2c,0x78,SSD1306::HardwareConf::SEQ_NOREMAP);

 //printf("asdczx");
 oled.Initialize();
 err=oled.last_error;
 led.Initialize();
 err=led.last_error;

 //oled.Set_Font_size(Fonts::Font_16x26);
 err=oled.IsInitialized();
 err=led.IsInitialized();
// printf("asdczx");
 oled.Fill(SSD1306::WHITE);
 oled.Update_Screen();
 led.Fill(SSD1306::WHITE);
 led.Update_Screen();
 err=oled.last_error;
 err=led.last_error;
 delay_ms(200);
 oled.Fill(SSD1306::BLACK);
 oled.Update_Screen();
 led.Fill(SSD1306::BLACK);
 led.Update_Screen();
 delay_ms(50);
// printf("asdczx");
 oled.Draw_Pixel(0,0,SSD1306::WHITE);
 oled.Draw_Pixel(10,0,SSD1306::WHITE);
 oled.Draw_Pixel(20,20,SSD1306::WHITE);
 oled.Draw_Pixel(10,1,SSD1306::WHITE);
 oled.Draw_Pixel(10,7,SSD1306::WHITE);
 oled.Draw_Pixel(10,8,SSD1306::WHITE);
 oled.Draw_Pixel(10,9,SSD1306::WHITE);
 oled.Draw_Pixel(10,10,SSD1306::WHITE);
 oled.Draw_Pixel(10,11,SSD1306::WHITE);
 oled.Draw_Pixel(127,62,SSD1306::WHITE);
 oled.Draw_Pixel(127,63,SSD1306::WHITE);
 oled.Draw_Pixel(126,63,SSD1306::WHITE);

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

 led.Draw_Pixel(0,0,SSD1306::WHITE);
 led.Draw_Pixel(10,0,SSD1306::WHITE);
 led.Draw_Pixel(20,20,SSD1306::WHITE);
 led.Draw_Pixel(10,1,SSD1306::WHITE);
 led.Update_Screen();
 delay_ms(800);
 led.Set_Cursor(0,0);
 led.Write_String("lorem");
 led.Update_Screen();
 delay_ms(200);
 led.Set_Cursor(50,16);
 led.Write_String_Inverted("ipsum");
 led.Update_Screen();

 oled.Draw_Image(sandals);

 oled.Update_Screen();

while(1)
 {
// oled.Clean();
// oled.Set_Cursor(0,0);
// oled.Set_Brightness(0);
// oled.Write_String("b=0");
// oled.Update_Screen();
// delay_ms(500);
// oled.Set_Cursor(0,0);
// oled.Set_Brightness(50);
// oled.Write_String("b=50");
// oled.Update_Screen();
// delay_ms(500);
// oled.Set_Cursor(0,0);
// oled.Set_Brightness(100);
// oled.Write_String("b=100");
// oled.Update_Screen();
// delay_ms(500);
// oled.Set_Cursor(0,0);
// oled.Set_Brightness(150);
// oled.Write_String("b=150");
// oled.Update_Screen();
// delay_ms(500);
// oled.Set_Cursor(0,0);
// oled.Set_Brightness(200);
// oled.Write_String("b=200");
// oled.Update_Screen();
// delay_ms(500);
// oled.Set_Cursor(0,0);
// oled.Set_Brightness(0xff);
// oled.Write_String("b=0xff");
// oled.Update_Screen();
// delay_ms(500);
 }
 //oled.Mirror_Screen(false);
 //oled.Update_Screen();
 //oled.Flip_Screen(true);


// DMA dma(DMA1_Channel6);
//
// dma.Set_Size_Memory(DMA::Size::HALF_WORD);
// delay_ms(100);
// dma.Set_Minc(1);
//
// dma.Set_Minc(0);
//
// I2C i2c_dma(I2C1,&dma);
// i2c_dma.Initialise();
// i2c_dma.Enable();
// i2c_dma.Enable_DMA();
//
// MCP47<1> dac_dma(i2c_dma);
//// uint16_t data[]={ 0xAE,0xd5,0x80,0xA8,0x1F};
// dac_dma.Set_Continuous(sine_table,sine_table_lenght);

 while(1);

#if 0
 I2C i2c(I2C1);
 i2c.Initialise();
 i2c.Enable();
 //volatile I2C::ErrorCode jkl=i2c.Send_Data((uint8_t)0xc0,(uint8_t)0x34);
 printf("asdczx");

 //while(1);
delay_ms(13);
 MCP47<1> dac(i2c);
 dac.Set_Output(200);
 delay_ms(1000);
 dac.Set_Output(2000);

 while(1)
   {
     printf("asdczx");
     delay_ms(500);
   }
#endif

#if 0
 uint8_t data[]={ 0xAE,0xd5,0x80,0xA8,0x1F};

 DMA dd(DMA1_Channel6);
 dd.Set_Direction(1);
 dd.Set_Minc(1);
 dd.Set_Data_Count(5);
 dd.Set_Peripheral_Addr((uint32_t)(&I2C1->DR));
 dd.Set_Memory_Addr((uint32_t)(&data));

 //DMA1_Channel6->CCR|=DMA_CCR_MINC|DMA_CCR_DIR;
 //    DMA1_Channel6->CNDTR=5;
 ///DMA1_Channel6->CPAR=(uint32_t)(&I2C1->DR);
 //DMA1_Channel6->CMAR=(uint32_t)(&data);


 I2C1->CR1|= I2C_CR1_SWRST;
 HAL_Delay(50);
 I2C1->CR1&=~ I2C_CR1_SWRST;
I2C1->CR2|=36|I2C_CR2_DMAEN;
//CCR = 50ns
I2C1->CCR|=I2C_CCR_FS| I2C_CCR_DUTY | 6;
I2C1->TRISE|=9;
I2C1->CR1|= I2C_CR1_PE;
I2C1->CR1|=I2C_CR1_START;

//DMA1_Channel6->CCR|=DMA_CCR_EN;
dd.Enable();

volatile uint32_t jk=I2C1->SR1;
printf("\n\r sds kjhui%d",jk);

I2C1->DR=0x78;

while((I2C1->SR1 & I2C_SR1_ADDR)==0);


jk=I2C1->SR1;
 printf("\n\rsds%d",jk);
 jk=I2C1->SR2;
  printf("\n\rsds%d",jk);

  //DMA1_Channel6->CNDTR=5;



#else
 /*Initialize in full speed 400kHz
  * freq=36MHz=>TPCLK1=28ns
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
if (1)
{
    I2C1->CR1|= I2C_CR1_SWRST;
    HAL_Delay(50);
    I2C1->CR1&=~ I2C_CR1_SWRST;
  I2C1->CR2|=36;
  //CCR = 50ns
  I2C1->CCR|=I2C_CCR_FS| I2C_CCR_DUTY | 6;
  I2C1->TRISE|=9;
  I2C1->CR1|= I2C_CR1_PE;
  I2C1->CR1|=I2C_CR1_START;
}

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
#endif

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
