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
 do hardware test (for buttons) and do not delete after pass!!!
 USB- mass storage of data
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
#include <Waveformarythmetics.hpp>
#include "sine.hpp"
#include "SSD1306.hpp"
#include <string>
#include <vector>
#include "adc.hpp"
#include "LCRmath.hpp"

/* Private function prototypes -----------------------------------------------*/

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern volatile uint32_t Adc::adc_value[size_of_adc_buffer];

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
static void MX_GPIO_Init (void);
static void MX_DMA_Init (void);
static void MX_I2C1_Init (void);
static void MX_I2C2_Init (void);
//void Fatal_Error(void);

volatile int yolo = 0;
volatile int xD = 0;
volatile uint32_t error_flag = 0;

extern "C"
{
  void DMA1_Channel1_IRQHandler (void)
  {
    /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
//  DMA1->IFCR |=DMA_IFCR_CHTIF1;
    DMA1->IFCR = DMA_IFCR_CGIF1;
    /* USER CODE END DMA1_Channel1_IRQn 0 */
//  __HAL_DMA_CLEAR_FLAG(hdma_adc1, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_adc1));
    // HAL_DMA_IRQHandler(&hdma_adc1);
    xD = 1;
    // HAL_ADC_Start_DMA (&hadc1, (uint32_t*) Adc::adc_value, SIZE_OF_ADC_BUFFER);
    /* USER CODE END DMA1_Channel1_IRQn 1 */
  }

  int _write (int file, char *ptr, int len)
  {
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
      {
	ITM_SendChar ((*ptr++));
	//__io_putchar(*ptr++);
      }
    return len;
  }

  /**
   * @brief This function handles DMA1 channel4 global interrupt.
   */
  void DMA1_Channel4_IRQHandler (void)
  {
    /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

    /* USER CODE END DMA1_Channel4_IRQn 0 */

    /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

    /* USER CODE END DMA1_Channel4_IRQn 1 */
  }

  /**
   * @brief This function handles I2C1 error interrupt.
   */
  void I2C1_ER_IRQHandler (void)
  {
    //  __disable_irq();
    HAL_NVIC_DisableIRQ (I2C1_ER_IRQn);
    error_flag = 1;
  }

  void EXTI3_IRQHandler (void)
  {
    /* USER CODE BEGIN EXTI2_IRQn 0 */
    yolo++;
    /* USER CODE END EXTI2_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler (GPIO_PIN_3);
    /* USER CODE BEGIN EXTI2_IRQn 1 */

    /* USER CODE END EXTI2_IRQn 1 */
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

/**
 * @brief  The application entry point.
 * @retval int
 */
int main (void)
{
  HAL_Init ();
  MX_DMA_Init ();
  SystemClock_Config ();
  MX_GPIO_Init ();
  MX_I2C1_Init ();

  char buf[30];
  uint8_t display_buffer[100];
  uint32_t error = 0;

  Pwm<TIM_TypeDef, uint16_t, 2> pwm (TIM1, 100);
  pwm.Initialise ();
  pwm.Set_Frequency (5000);
  pwm.Set_Duty (30);
  pwm.Enable ();

  HAL_Delay (200);
  __HAL_RCC_ADC2_CLK_ENABLE();
  __HAL_RCC_ADC1_CLK_ENABLE();
  ///////////////////////////////////////////////////////////////////////////
///	I2C
//////////////////////////////////////////////////////////////////////////

  __HAL_RCC_I2C1_CLK_ENABLE();
  __HAL_RCC_I2C2_CLK_ENABLE();

  I2C i2c2 (I2C2);

  I2C_HandleTypeDef i2c;

  i2c.Instance = I2C2;
  i2c.Init.ClockSpeed = 400000;
  i2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
  i2c.Init.OwnAddress1 = 0xff;
  i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  i2c.Init.OwnAddress2 = 0xff;
  i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  HAL_I2C_Init (&i2c);

  delay_ms (200);

  SSD1306 oled (&i2c2, 64);

  oled.Initialize ();
  //oled.Get_Last_Error();
  //oled.IsInitialized();

  oled.Fill (SSD1306::WHITE);
  oled.Update_Screen ();
  delay_ms (500);
  oled.Fill (SSD1306::BLACK);
  oled.Update_Screen ();

  //////////////////////////////////////////////////////////////////////////////////////////////
  /////DAC

  HAL_NVIC_SetPriority (DMA1_Channel6_IRQn, 2, 1);
  HAL_NVIC_EnableIRQ (DMA1_Channel6_IRQn);

  __HAL_RCC_DMA1_CLK_ENABLE();
  DMA dma (DMA1_Channel6);

  //DMA1_Channel6->CCR|=14;
  I2C i2c_dma (I2C1, &dma);
  i2c_dma.Initialise ();
  i2c_dma.Enable_DMA ();
  i2c_dma.Enable ();

  MCP47FEB dac (i2c_dma);
  error = dac.Set_Vref (MCP47FEB::Vref::VREF_BUFFERED);
  error = dac.Set_Continuous (sine_table, sine_table_lenght);
  dac.Enable_Output ();

////////////////////////////////////////////////////
//OSCILLOSCOPE
///////////////////////////////////////////////////////////////////////////////
//  Adc::Set_LCR();
//  HAL_Delay(100);
//  Adc::Set_Oscilloscope();
//  double lala=0;
//
//  lala=Adc::Set_Sampling_time(Adc::SamplingTimeClocks::ADCCLK_239CYCLES5);
//  while (1)
//    {
//
//      while(xD==0)
//	{
//
//	}
//      xD=0;
//      HAL_Delay (100);
//      oled.Clean ();
//      Waveform_arythmetics::Calc_Moving_Average((uint32_t*)Adc::adc_value,1024,1);
//      uint32_t edge=Waveform_arythmetics::Get_Edge_index(1000,false);
//      for (int i=0;i<100;i++)
//	{
//	  display_buffer[i]=Waveform_arythmetics::filtered_buffer[0][i+edge]/64;
//	}
//      sprintf (buf, "yol=%1.3f", lala);
//      oled.Set_Cursor (0, 0), oled.Write_String (buf);
//      sprintf (buf, "edge=%ld", edge);
//      oled.Set_Cursor (63, 0), oled.Write_String (buf);
//     	 oled.Draw_Waveform(10,60,display_buffer,100,SSD1306::WHITE);
//     	oled.Update_Screen ();
//
//        DMA1_Channel1->CCR &=~DMA_CCR_EN;
//        DMA1_Channel1->CNDTR=1024;
//        DMA1_Channel1->CCR |=DMA_CCR_EN;// Check corectness for dual adc
//    }
  //////////////////////////////////////////////
  //TEMP
  /////////////////
  Adc::Set_Voltage_Temperature ();
      while (xD == 0)
	{

	}
      xD = 0;
      oled.Clean ();

      sprintf (buf, "volt=%ld", Adc::Update_Vref ());
      oled.Set_Cursor (0, 0), oled.Write_String (buf);
      sprintf (buf, "temp=%d", Adc::Get_Temperature ());
      oled.Set_Cursor (0, 10), oled.Write_String (buf);
      oled.Update_Screen ();
      HAL_Delay (1000);
      oled.Clean ();

//      DMA1_Channel1->CCR &= ~DMA_CCR_EN;
//      DMA1_Channel1->CNDTR = 2;
//      DMA1_Channel1->CCR |= DMA_CCR_EN; // Check corectness for dual adc
//    }
// ///////////////////////////////////////////////////////////////////////////////////
// //ADC
// //////////////////////////////////////////////////////
//  oled.Clean ();
//	oled.Update_Screen ();
//             // char buf[30];
  double lala = 0;
  error = Adc::Set_LCR ();
//  lala = Adc::Set_Sampling_time (Adc::SamplingTimeClocks::ADCCLK_239CYCLES5);
//
  Waveform_arythmetics::mid_voltage = 2000;
  Waveform_arythmetics::user_point_time = 21;

  while (1)
    {
      while (xD == 0)
	{

	}
      xD = 0;
      if (error_flag == 1)
	{
	  oled.Set_Cursor (20, 20);
	  oled.Set_Font_size (Fonts::font_16x26);
	  oled.Write_String_Inverted ("FATAL");
	  oled.Update_Screen ();
	  break;
	}
      HAL_Delay (400);
      oled.Clean ();

//      Waveform_arythmetics::Calc_Moving_Average ((uint32_t*) Adc::adc_value,
//						 Adc::size_of_adc_buffer, 1);
//      uint32_t edge = Waveform_arythmetics::Get_Edge_index (1000, false);
//      for (int i = 0; i < 100; i++)
//	{
//	  display_buffer[i] = Waveform_arythmetics::filtered_buffer[0][i + edge]
//	      / 64;
//	}
//      oled.Draw_Waveform (10, 60, display_buffer, 100, SSD1306::WHITE);
//      oled.Update_Screen ();
      Waveform_arythmetics::Calc_Moving_Average ((uint32_t*) Adc::adc_value,
						 Adc::size_of_adc_buffer, 5);
      Waveform_arythmetics::Find_Peaks ();
      Waveform_arythmetics::Calc_Alfa ();
      Waveform_arythmetics::Calc_Amplitude ();

      bool ind=LCR_math::Calculate (
	  Adc::Adc_To_Milivolts (Waveform_arythmetics::amplitude1),
	  Adc::Adc_To_Milivolts (Waveform_arythmetics::amplitude2),
	  double(Waveform_arythmetics::alfa/1000), Waveform_arythmetics::frequency);

  sprintf (buf, "cap=%1.9f", LCR_math::capacitance);
  oled.Set_Cursor (0, 0), oled.Write_String (buf);
  sprintf (buf, "ind=%1.9f", LCR_math::inductance);
  oled.Set_Cursor (0, 20), oled.Write_String (buf);
  sprintf (buf, "res=%1.9f", LCR_math::resistance);
  oled.Set_Cursor (0, 40), oled.Write_String (buf);
//      sprintf (buf, "a1=%3ld", Waveform_arythmetics::amplitude1);
//      oled.Set_Cursor (60, 0), oled.Write_String (buf);
//      sprintf (buf, "a2=%ld", Waveform_arythmetics::amplitude2);
//      oled.Set_Cursor (60, 20), oled.Write_String (buf);
//      sprintf (buf, "f=%ld", Waveform_arythmetics::frequency);
//      oled.Set_Cursor (0, 20), oled.Write_String (buf);
//      sprintf (buf, "a=%ld", Waveform_arythmetics::alfa);
//      oled.Set_Cursor (60, 40), oled.Write_String (buf);
//      sprintf (buf, "e=%ld", error);
//      oled.Set_Cursor (60, 40), oled.Write_String (buf);
//      sprintf (buf, "mp1=%ld", Waveform_arythmetics::minor_peak1);
//      oled.Set_Cursor (0, 40), oled.Write_String (buf);

  if(ind==true)
    {
      oled.Draw_Line_H(10,60,20,SSD1306::Color::WHITE);
    }
  else
    {
      oled.Draw_Line_H(10,60,20,SSD1306::Color::BLACK);
    }

      oled.Update_Screen ();

      DMA1_Channel1->CCR &= ~DMA_CCR_EN;
      DMA1_Channel1->CNDTR = Adc::size_of_adc_buffer;
      DMA1_Channel1->CCR |= DMA_CCR_EN; // Check corectness for dual adc
    }

///////////////////////////////////////////////////////////////////////////
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
//    delay_ms (200);
//    /* USER CODE EN
//     * D WHILE */
//    GPIOC->ODR ^= (1 << 13);
//    printf ("sdshm  mj");
//    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config (void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct =
    { 0 };
  RCC_ClkInitTypeDef RCC_ClkInitStruct =
    { 0 };
  RCC_PeriphCLKInitTypeDef PeriphClkInit =
    { 0 };

  /**Initializes the CPU, AHB and APB busses clocks
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler ();
    }
  /**Initializes the CPU, AHB and APB busses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
      | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      Error_Handler ();
    }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInit) != HAL_OK)
    {
      Error_Handler ();
    }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6; //12Mhz at 72Mhz core clock
  if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInit) != HAL_OK)
    {
      Error_Handler ();
    }

  HAL_RCC_MCOConfig (RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
  /**Enables the Clock Security System
   */
  HAL_RCC_EnableCSS ();

  __HAL_RCC_DMA1_CLK_ENABLE()
  ;
  __HAL_RCC_ADC1_CLK_ENABLE()
  ;
  __HAL_RCC_TIM1_CLK_ENABLE()
  ;

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStruct =
    { 0 };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE()
  ;
  __HAL_RCC_GPIOA_CLK_ENABLE()
  ;
  __HAL_RCC_GPIOB_CLK_ENABLE()
  ;
  __HAL_RCC_GPIOC_CLK_ENABLE()
  ;

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init (GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_5; //latch
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init (GPIOB, &GPIO_InitStruct);

  GPIO_InitTypeDef gpio;

  gpio.Mode = GPIO_MODE_ANALOG; //LCR
  gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  HAL_GPIO_Init (GPIOB, &gpio);

  gpio.Mode = GPIO_MODE_ANALOG; //OSCILLOSCOPE
  gpio.Pin = GPIO_PIN_7;
  HAL_GPIO_Init (GPIOA, &gpio);

  //I2C

  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init (GPIOB, &GPIO_InitStruct);

  /////
  /*Configure GPIO pins : BUTTON_OK_Pin BUTTON_BACK_Pin BUTTON_OKA4_Pin BUTTON_UP_Pin */
  GPIO_InitStruct.Pin = BUTTON_OK_Pin | BUTTON_BACK_Pin | BUTTON_OKA4_Pin
      | BUTTON_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority (EXTI3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ (EXTI3_IRQn);

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init (void)
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
  if (HAL_I2C_Init (&hi2c1) != HAL_OK)
    {
      Error_Handler ();
    }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init (void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct =
    { 0 };

  LL_GPIO_InitTypeDef GPIO_InitStruct =
    { 0 };

  LL_APB2_GRP1_EnableClock (LL_APB2_GRP1_PERIPH_GPIOB);
  /**I2C2 GPIO Configuration
   PB10   ------> I2C2_SCL
   PB11   ------> I2C2_SDA
   */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init (GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_I2C2);

  /* I2C2 DMA Init */

  /* I2C2_TX Init */
  LL_DMA_SetDataTransferDirection (DMA1, LL_DMA_CHANNEL_4,
				   LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel (DMA1, LL_DMA_CHANNEL_4, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode (DMA1, LL_DMA_CHANNEL_4, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode (DMA1, LL_DMA_CHANNEL_4, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode (DMA1, LL_DMA_CHANNEL_4, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize (DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize (DMA1, LL_DMA_CHANNEL_4, LL_DMA_MDATAALIGN_BYTE);

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  /**I2C Initialization
   */
  LL_I2C_DisableOwnAddress2 (I2C2);
  LL_I2C_DisableGeneralCall (I2C2);
  LL_I2C_EnableClockStretching (I2C2);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.ClockSpeed = 400000;
  I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init (I2C2, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2 (I2C2, 0);
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init (void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE()
  ;

  HAL_NVIC_SetPriority (DMA1_Channel1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ (DMA1_Channel1_IRQn);

  /* DMA1_Channel4_IRQn interrupt configuration */
  //NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  // NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler (void)
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
