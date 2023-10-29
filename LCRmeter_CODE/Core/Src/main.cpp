/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : Main program body
 ******************************************************************************
* @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
extern volatile uint32_t Adc::adc_buffer[size_of_adc_buffer];

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
DMA_HandleTypeDef hdma_i2c1_tx;



/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
//void Fatal_Error(void);

volatile int yolo = 0;
volatile int xD = 0;
volatile uint32_t error_flag = 0;

extern "C"
{
#include "printf_redirection.h"

void DMA1_Channel1_IRQHandler(void)
{
	if(DMA1->ISR & DMA_ISR_TEIF1)
	{
		//TODO add global error
		return;
	}
//  DMA1->IFCR |=DMA_IFCR_CHTIF1;
	DMA1->IFCR = DMA_IFCR_CGIF1;
	xD = 1;
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

/**
 * @brief This function handles I2C1 error interrupt.
 */
void I2C1_ER_IRQHandler(void)
{
	//  __disable_irq();
	HAL_NVIC_DisableIRQ (I2C1_ER_IRQn);
	error_flag = 1;
}

void EXTI3_IRQHandler(void)
{
	/* USER CODE BEGIN EXTI2_IRQn 0 */
	yolo++;
	/* USER CODE END EXTI2_IRQn 0 */
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
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

static void Init(void)
{
	HAL_Init();
	SystemClock_Config();
	__HAL_RCC_ADC2_CLK_ENABLE();
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();
	HAL_Delay(10);

	MX_DMA_Init();
	MX_GPIO_Init();
	HAL_Delay(10);


	//MX_I2C1_Init();


	/* Because of messed cubemx configuration and this file
	 * I2C2 is here instead of MX_I2Cx_Init() like I2C1
	 * It didn't work with MX so I left it here if you have time you can fix it.
	*/
	MX_I2C2_Init();


//	  hi2c2.Instance = I2C2;
//	  hi2c2.Init.ClockSpeed = 400000;
//	  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
//	  hi2c2.Init.OwnAddress1 = 0xff;
//	  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//	  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	  hi2c2.Init.OwnAddress2 = 0xff;
//	  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//	  auto tret=HAL_I2C_Init (&hi2c2);
//	  printf("tret=%d",tret);
	  printf("1111hi2c2->State=%d\n",hi2c2.State);
}

int main(void)
{
	Init();
	HAL_Delay(100);
	char buf[30];
	uint8_t display_buffer[100];
	uint32_t error = 0;

	UART_HandleTypeDef* u1=UART_printf_init();
	printf("hola!! \n");

//	Pwm<TIM_TypeDef, uint16_t, 2> pwm(TIM1, 100);
//	pwm.Initialise();
//	pwm.Set_Frequency(600);
//	pwm.Set_Duty(80);
//	pwm.Enable();
//
	SSD1306 oled(&hi2c2, 64);
	oled.Initialize();
	oled.Set_Brightness(0xff);

	oled.Fill(SSD1306::WHITE);
	oled.Update_Screen();
	delay_ms(1000);
	oled.Fill(SSD1306::BLACK);
	oled.Update_Screen();
	delay_ms(500);

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

  MCP47FEB dac(i2c_dma);
  error = dac.Set_Vref (MCP47FEB::Vref::VREF_BUFFERED);
	oled.Set_Cursor(0, 0);
	sprintf (buf, "dac.error1=%lu", error);
	oled.Write_String (buf);
  error = dac.Set_Continuous (sine_table, sine_table_lenght);
	oled.Set_Cursor(0, 20);
	sprintf (buf, "dac.error2=%lu", error);
	oled.Write_String (buf);
  dac.Enable_Output ();
  oled.Update_Screen();


//////////////////////////////////////////////////
//OSCILLOSCOPE
/////////////////////////////////////////////////////////////////////////////
//	int eh = 0;
//	eh = Adc::Set_LCR();
//	oled.Clean();
//	oled.Set_Cursor(0, 0);
//	sprintf(buf, "dac.error1=%d", eh);
//	oled.Write_String(buf);
//	HAL_Delay(100);
//	eh = Adc::Set_Oscilloscope();
//	double lala = 0;
//
//	lala = Adc::Set_Sampling_time(Adc::SamplingTimeClocks::ADCCLK_239CYCLES5);
//
//	oled.Set_Cursor(0, 20);
//	sprintf(buf, "dac.error2=%d", eh);
//	oled.Write_String(buf);
//	oled.Set_Cursor(0, 30);
//	sprintf(buf, "dac.error3=%f", lala);
//	oled.Write_String(buf);
//	oled.Update_Screen();
//	HAL_Delay(1300);
//	while (1)
//	{
//		while (xD == 0)
//		{
//
//		}
//
//		xD = 0;
//		HAL_Delay(100);
//		oled.Clean();
//		Waveform_arythmetics::Calc_Moving_Average((uint32_t*) Adc::adc_buffer,
//				1024, 1);
//		uint32_t edge = Waveform_arythmetics::Get_Edge_index(1000, false);
//		if (edge >900)
//		{
//			Adc::Resume_DMA();
//			continue;
//		}
//		for (int i = 0; i < 100; i++)
//		{
//			display_buffer[i] = Waveform_arythmetics::filtered_buffer[0][i
//					+ edge] / 64;
//		}
//		sprintf(buf, "yol=%1.3f", lala);
//		oled.Set_Cursor(0, 0), oled.Write_String(buf);
//		sprintf(buf, "edge=%ld", edge);
//		oled.Set_Cursor(63, 0), oled.Write_String(buf);
//		oled.Draw_Waveform(10, 60, display_buffer, 100, SSD1306::WHITE);
//		oled.Update_Screen();
//
//		Adc::Resume_DMA();
//	}
////
////	HAL_Delay(1000);
//	////////////////////////////////////////////
//
//	//TEMP
//	/////////////////
  Adc::Set_Voltage_Temperature ();
  uint32_t Vmax=0;
  uint32_t Vmin=99999;
  uint32_t Vref=0;
  int co=0;
  while(1)
  {
      while (xD == 0)
	{

	}
      xD = 0;
      oled.Clean ();
      Vref=Adc::Update_Vref ();
      if(Vref<Vmin)
      {
    	  Vmin=Vref;
      }
      if(Vref>Vmax)
      {
    	  Vmax=Vref;
      }
      sprintf (buf, "Vref=%ld",Vref);
      oled.Set_Cursor (0, 0), oled.Write_String (buf);
      sprintf (buf, "temp=%d", Adc::Get_Temperature ());
      oled.Set_Cursor (0, 10), oled.Write_String (buf);
      sprintf (buf, "Vmax=%d", Vmax);
      oled.Set_Cursor (0, 20), oled.Write_String (buf);
      sprintf (buf, "Vmin=%d", Vmin);
      oled.Set_Cursor (0, 30), oled.Write_String (buf);
      oled.Update_Screen ();
      HAL_Delay (100);
      co++;
      if(co>2)
      {
    	  break;
      }
    }
// ///////////////////////////////////////////////////////////////////////////////////
// //ADC
// //////////////////////////////////////////////////////

  oled.Clean ();
  double lala = 0;
  error = Adc::Set_LCR ();
  lala = Adc::Set_Sampling_time (Adc::SamplingTimeClocks::ADCCLK_239CYCLES5);

  Waveform_arythmetics::mid_voltage = 2000;
  Waveform_arythmetics::hysteresis_samples=10;
  Waveform_arythmetics::user_point_time = 21; //TODO get the Set_Sampling_time in uint

  while (1)
    {
      while (xD == 0)
	{

	}
      xD = 0;
      HAL_Delay (500);
      oled.Clean ();

      Waveform_arythmetics::Calc_Moving_Average ((uint32_t*) Adc::adc_buffer,
						 Adc::size_of_adc_buffer, 7); //TODO with 1 it's to chaotic can proccessing be improved?
      Waveform_arythmetics::Find_Peaks ();
      Waveform_arythmetics::Calc_Frequency();
      Waveform_arythmetics::Calc_Alfa ();
      Waveform_arythmetics::Calc_Amplitude ();

      bool ind=LCR_math::Calculate (
	  Adc::Adc_To_Milivolts (Waveform_arythmetics::amplitude1),
	  Adc::Adc_To_Milivolts (Waveform_arythmetics::amplitude2),
	  double(Waveform_arythmetics::alfa/1000), Waveform_arythmetics::frequency);

      sprintf (buf, "cap=%1.9f", LCR_math::capacitance);
      oled.Set_Cursor (0, 0), oled.Write_String (buf);
      sprintf (buf, "ind=%1.9f", LCR_math::inductance);
      oled.Set_Cursor (0, 10), oled.Write_String (buf);
      sprintf (buf, "res=%1.9f", LCR_math::resistance);
      oled.Set_Cursor (0, 20), oled.Write_String (buf);
//
      sprintf (buf, "a1=%ld", Waveform_arythmetics::amplitude1);
      oled.Set_Cursor (0, 30), oled.Write_String (buf);
      sprintf (buf, "a2=%ld", Waveform_arythmetics::amplitude2);
      oled.Set_Cursor (60, 30), oled.Write_String (buf);
      sprintf (buf, "f=%ld", Waveform_arythmetics::frequency);
      oled.Set_Cursor (0, 40), oled.Write_String (buf);
      sprintf (buf, "a=%ld", Waveform_arythmetics::alfa);
      oled.Set_Cursor (60, 40), oled.Write_String (buf);

            sprintf (buf, "min=%d", Waveform_arythmetics::nbr_of_minimas[0]);
            oled.Set_Cursor (00, 50), oled.Write_String (buf);
            sprintf (buf, "max=%d",  Waveform_arythmetics::nbr_of_peaks[0]);
            oled.Set_Cursor (40, 50), oled.Write_String (buf);
            sprintf (buf, "ind=%d", Waveform_arythmetics::minimas[0][0]);
            oled.Set_Cursor (80, 50), oled.Write_String (buf);

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
        {
            uint8_t aaa123[] =
            { 0xff, 0x11, 0xab, 0xaa };     //start indication or sth
            for (uint32_t i = 0; i < Waveform_arythmetics::buffer_size; i++)
            {
                HAL_UART_Transmit(u1, (uint8_t*) (aaa123), 4, 1000);
                uint8_t x[4];
                x[0] = Waveform_arythmetics::filtered_buffer[0][i] >> 8;
                x[1] = Waveform_arythmetics::filtered_buffer[0][i] & 0xff;
                x[2] = Waveform_arythmetics::filtered_buffer[1][i] >> 8;
                x[3] = Waveform_arythmetics::filtered_buffer[1][i] & 0xff;
                if (i == Waveform_arythmetics::peaks[0][0] && i != 0)
                {
                    x[0] = 0x0f;
                    x[1] = 0x8f;
                }
                if (i == Waveform_arythmetics::minimas[0][0] && i != 0)
                {
                    x[0] = 0;
                    x[1] = 5;
                }
                if (i == Waveform_arythmetics::peaks[1][0] && i != 0)
                {
                    x[2] = 0x0f;
                    x[3] = 0x8f;
                }
                if (i == Waveform_arythmetics::minimas[1][0] && i != 0)
                {
                    x[2] = 0;
                    x[3] = 5;
                }

                if (i == Waveform_arythmetics::peaks[0][1] && i != 0)
                {
                    x[0] = 0x0f;
                    x[1] = 0x8f;
                }
                if (i == Waveform_arythmetics::minimas[0][1] && i != 0)
                {
                    x[0] = 0;
                    x[1] = 5;
                }
                if (i == Waveform_arythmetics::peaks[1][1] && i != 0)
                {
                    x[2] = 0x0f;
                    x[3] = 0x8f;
                }
                if (i == Waveform_arythmetics::minimas[1][1] && i != 0)
                {
                    x[2] = 0;
                    x[3] = 5;
                }
                HAL_UART_Transmit(u1, &x[0], 4, 1000);

            }

            HAL_Delay(3000);
        }
        oled.Update_Screen();
        Adc::Resume_DMA();
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
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
	// RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6; //12Mhz at 72Mhz core clock
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
	/**Enables the Clock Security System
	 */
	HAL_RCC_EnableCSS();

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
static void MX_GPIO_Init(void)
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
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5; //latch
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitTypeDef gpio;

	gpio.Mode = GPIO_MODE_ANALOG; //LCR
	gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &gpio);

	gpio.Mode = GPIO_MODE_ANALOG; //OSCILLOSCOPE
	gpio.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &gpio);

	//I2C

	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/////
	/*Configure GPIO pins : BUTTON_OK_Pin BUTTON_BACK_Pin BUTTON_OKA4_Pin BUTTON_UP_Pin */
	GPIO_InitStruct.Pin = BUTTON_OK_Pin | BUTTON_BACK_Pin | BUTTON_OKA4_Pin
			| BUTTON_UP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	////
	//USB
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ (EXTI3_IRQn);

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

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE()
	;

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
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
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    printf("ERROR_HANDLER \n");


    while (1)
    {
    }
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
