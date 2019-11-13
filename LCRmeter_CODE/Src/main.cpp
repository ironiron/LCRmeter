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

/* Private function prototypes -----------------------------------------------*/

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern volatile uint32_t Adc::adc_value[SIZE_OF_ADC_BUFFER];

I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);

extern "C" {
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
//  DMA1->IFCR |=DMA_IFCR_CHTIF1;
  DMA1->IFCR =DMA_IFCR_CGIF1;
  /* USER CODE END DMA1_Channel1_IRQn 0 */
//  __HAL_DMA_CLEAR_FLAG(hdma_adc1, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_adc1));
 // HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
  DMA1_Channel1->CCR &=~DMA_CCR_EN;
  DMA1_Channel1->CNDTR=1024;
  DMA1_Channel1->CCR |=DMA_CCR_EN;// Check corectness for dual adc
 // HAL_ADC_Start_DMA (&hadc1, (uint32_t*) Adc::adc_value, SIZE_OF_ADC_BUFFER);
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


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  char buf[30];
  MX_DMA_Init();//TODO check MSP file

  /* Configure the system clock */
  SystemClock_Config();
  MX_GPIO_Init();


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



 //////////////////////////////////////////////////////////////////////////////////////////////
 /////DAC

 __HAL_RCC_DMA1_CLK_ENABLE();
  DMA dma(DMA1_Channel6);

  dma.Set_Size_Memory(DMA::Size::HALF_WORD);
  dma.Set_Minc(0);
  dma.Set_Priority(DMA::Priority::HIGH);


  I2C i2c_dma(I2C1,&dma);
  i2c_dma.Initialise();
  i2c_dma.Enable_DMA();
  i2c_dma.Enable();

  uint32_t h=0;


 MCP4725 dac_dma(i2c_dma);
 MCP47FEB dac(i2c_dma);

 // std::vector<int>arr(990000,4);
 // sprintf (buf, "vv=%d", arr[60]);
   // oled.Set_Cursor (70, 20); oled.Write_String (buf);

 h = dac.Set_Vref (MCP47FEB::Vref::VREF);
 sprintf (buf, "vref = %d", h);
 oled.Set_Cursor (0, 20), oled.Write_String (buf);
 oled.Update_Screen ();

  //h=dac.Set_Output(100);
   h = dac.Set_Continuous(sine_table,sine_table_lenght);
   sprintf (buf, "E = %d", h);
   oled.Set_Cursor (0, 0), oled.Write_String (buf);
   oled.Update_Screen ();
   HAL_Delay (600);

dac.Enable_Output();
////////////////////////////////////////////////////
//OSCILLOSCOPE
///////////////////////////////////////////////////////////////////////////////
  Adc::Set_Oscilloscope ();

  Adc::Set_Sampling_time(ADC_SAMPLETIME_71CYCLES_5);

// uint16_t value = HAL_ADC_GetValue(&adc);
  float v;
uint8_t xx[1000];
  while (1)
    {

      HAL_Delay (80);
      oled.Clean ();
//	 //value = adc_value[0];
//	 value = adc_read(ADC_CHANNEL_7);
//	 v = (float)value * 3.3f / 4096.0f;
//             sprintf(buf,"ADC%d = %d", 0, value);
//             oled.Set_Cursor(0,0),
//             oled.Write_String(buf);

      Waveform_arythmetics::Calc_Moving_Average((uint32_t*)Adc::adc_value,1024,1);
      for (int i=0;i<1000;i++)
	{
	  xx[i]=Waveform_arythmetics::filtered_buffer[0][i]/64;
	}
     	 oled.Draw_Waveform(10,60,xx,100,SSD1306::WHITE);
     	oled.Update_Screen ();

//      sprintf (buf, "ADC%ld = %ld", 0, Adc::adc_value[0]);
//      oled.Set_Cursor (0, 20), oled.Write_String (buf);
//      sprintf (buf, "ADC%d = %d", 1, Adc::adc_value[1]);
//      oled.Set_Cursor (0, 0), oled.Write_String (buf);
//      v=Adc::adc_value[2]*3.3*2.0/4095.0;
//      sprintf (buf, "ADC = %1.3f", v);
//      oled.Set_Cursor (0, 40), oled.Write_String (buf);
//      oled.Set_Cursor (100, 55), oled.Write_String ("xD");
//      oled.Update_Screen ();
    }
////////////////////////////////////////////////
//WAVEFORM
///////////////////////////////////////////////////////////////////////////
oled.Clean();
for(int i=0;i<120;i++)
  {
    oled.Draw_Pixel(i,(sine_table[i]/5),SSD1306::Color::WHITE);
  }
oled.Update_Screen();

Waveform_arythmetics::Calc_Moving_Average((uint32_t *)sine_table,10,3);
 ///////////////////////////////////////////////////////////////////////////////////
 //ADC
 //////////////////////////////////////////////////////
int t=0;
             // char buf[30];
Adc::Set_Oscilloscope();
HAL_Delay(2000);
Adc::Set_LCR();
//
             while (1)
             {
        	 t++;
               /* USER CODE END WHILE */
                       sprintf(buf,"ADC1=%2d  ;	%2d  ", ((Adc::adc_value[0]>>16)& 0xffff), (Adc::adc_value[0]&0xffff));
                       oled.Set_Cursor(0,0),
                       oled.Write_String(buf);
                       sprintf(buf,"ADC1=%d;	%d", ((Adc::adc_value[1]>>16)& 0xffff), (Adc::adc_value[1]&0xffff));
                       oled.Set_Cursor(0,20),
                       oled.Write_String(buf);
                       oled.Update_Screen();
                       HAL_Delay(10);
                       if(t>100)
                	 {
                	   break;
                	 }
//               /* USER CODE BEGIN 3 */
             }
// asm("bkpt 255");

   	  HAL_Delay(20);
   	 oled.Clean();
   	 oled.Set_Cursor(1,1);
   	 oled.Write_String("aaaaaaaaa");
   	 oled.Update_Screen();
   	 HAL_Delay(300);



 //ADC_HandleTypeDef adc;

// DMA dma_adc(DMA1_Channel1);
// dma_adc.Set_Data_Count(1);
// dma_adc.Set_Direction(false);
// dma_adc.Set_Peripheral_Addr(uint16_t(ADC1->DR));
// dma_adc.Set_Size_Memory(DMA::Size::HALF_WORD);
// dma_adc.Set_Size_Peripheral(DMA::Size::HALF_WORD);
// dma_adc.Set_Memory_Addr(&adc_value[0]);
// dma_adc.Enable();



              Adc::Set_Oscilloscope();


    // uint16_t value = HAL_ADC_GetValue(&adc);
//     float v ;


     while (1)
       {

	  HAL_Delay(20);
	 oled.Clean();
	// Waveform_arythmetics::Calc_Moving_Average(Adc::adc_value,1024,11);
	 //oled.Draw_Waveform(0,0,(uint8_t*)(Waveform_arythmetics::filtered_buffer[0]),1024/11,SSD1306::WHITE);



//             sprintf(buf,"ADC%d = %d", 0, Adc::adc_value[0]);
//             oled.Set_Cursor(0,20),
//             oled.Write_String(buf);
//             sprintf(buf,"ADC%d = %d", 1, Adc::adc_value[1]);
//             oled.Set_Cursor(0,0),
//             oled.Write_String(buf);
//             sprintf(buf,"ADC = %f", v);
//             oled.Set_Cursor(0,40),
//             oled.Write_String(buf);
//             oled.Set_Cursor(100,40),
//             oled.Write_String("xD");
//             oled.Update_Screen();
     }

 while(1);
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
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;//12Mhz at 72Mhz core clock
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
  /**Enables the Clock Security System
  */
 HAL_RCC_EnableCSS();

 __HAL_RCC_DMA1_CLK_ENABLE();
 __HAL_RCC_ADC1_CLK_ENABLE();
 __HAL_RCC_TIM1_CLK_ENABLE();

 RCC_PeriphCLKInitTypeDef adc_clk;
 adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
 adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV6;
 HAL_RCCEx_PeriphCLKConfig(&adc_clk);

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

  GPIO_InitStruct.Pin = GPIO_PIN_5;//latch
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  GPIO_InitTypeDef gpio;

  gpio.Mode = GPIO_MODE_ANALOG; //LCR
  gpio.Pin =  GPIO_PIN_0|GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &gpio);

  gpio.Mode = GPIO_MODE_ANALOG;//OSCILLOSCOPE
  gpio.Pin =  GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &gpio);

  //I2C

  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  //NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
 // NVIC_EnableIRQ(DMA1_Channel4_IRQn);

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
