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

#include "main.h"
#include "usb_device.h"
#include "Pwm.hpp"
#include "stm32g4xx_hal.h"
#include "delay.h"
#include <stdio.h>
#include <Waveformarythmetics.hpp>
//#include "sine.hpp"
#include "SSD1306.hpp"
#include <string>
#include <vector>
#include "adc.hpp"
#include "LCRmath.hpp"


ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
ADC_HandleTypeDef hadc4;
ADC_HandleTypeDef hadc5;
DMA_HandleTypeDef hdma_adc1;

CORDIC_HandleTypeDef hcordic;

DAC_HandleTypeDef hdac1;
DAC_HandleTypeDef hdac2;
DMA_HandleTypeDef hdma_dac1_ch1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;



//void Fatal_Error(void);

volatile int yolo = 0;
volatile int xD = 0;
volatile uint32_t error_flag = 0;

extern "C"
{
#include "printf_redirection.h"


//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC1_Init(void);
static void MX_DAC2_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
static void MX_ADC4_Init(void);
static void MX_ADC5_Init(void);
static void MX_CORDIC_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);


ADC_HandleTypeDef adc;

//#if 1
//template<typename port_type,
//typename bval_type,
//const port_type port,
//const bval_type bval>
//class led_template
//{
//public:
//	led_template (){}
//	static void
//	Toggle ()
//	{
//		*reinterpret_cast<volatile bval_type*> (port) ^= bval;
//	}
//    static void
//    Reset ()
//    {
//        *reinterpret_cast<volatile bval_type*> (port) &=static_cast<bval_type> (~bval);
//    }
//};
//
//namespace
//{
////	const led_template<uint32_t, uint32_t, reinterpret_cast<volatile uint32_t *>(&(GPIOC->ODR)), (1<<13)> led_c13;
//	const led_template<uint32_t, uint32_t, ((GPIOC->ODR)), (1<<13)> led_c13;
//}
//
//#endif

////////////
//template<typename port_type,
//typename bval_type,
//const port_type port,
//const bval_type bval>
//class button_template
//{
//public:
//    button_template (){}
////    static void
////    Toggle ()
////    {
////        *reinterpret_cast<volatile bval_type*> (port) ^= bval;
////    }
//    static bool
//    State ()
//    {
//        return (*reinterpret_cast<bval_type*> (port) & static_cast<bval_type> (bval));
//    }
//};
//
////namespace
////{
//    const button_template<uint32_t, uint32_t, &uint32_t(GPIOA->IDR), (1<<4)> button_OK;
//    const button_template<uint32_t, uint32_t, &uint32_t(GPIOA->IDR), (1<<3)> button_DOWN;
//    const button_template<uint32_t, uint32_t, &uint32_t(GPIOA->IDR), (1<<2)> button_UP;
//    const button_template<uint32_t, uint32_t, &uint32_t(GPIOA->IDR), (1<<5)> button_BACK;
////}///
///////////

//typedef reg_access<std::uint8_t, std::uint8_t, mcal::reg::portb, pdir> port_dir_type;

volatile unsigned int b1=0;
volatile unsigned int b2=0;
volatile unsigned int b3=0;
volatile unsigned int b4=0;

static void Init(void)
{
	HAL_Init();
	SystemClock_Config();
	  MX_GPIO_Init();
	  MX_DMA_Init();
	  MX_DAC1_Init();
	  MX_DAC2_Init();
	  MX_ADC1_Init();
	  MX_ADC2_Init();
	  MX_ADC3_Init();
	  MX_ADC4_Init();
	  MX_ADC5_Init();
	  MX_CORDIC_Init();
	  MX_USB_Device_Init();
	  MX_I2C1_Init();
	  MX_TIM1_Init();
	  MX_TIM2_Init();
}

int main(void)
{
	Init();
	HAL_Delay(100);
	char buf[30];
	uint8_t display_buffer[100];
	uint32_t error = 0;

//	UART_HandleTypeDef* u1=UART_printf_init();
	printf("hola!! \n");

//	Pwm<TIM_TypeDef, uint16_t, 2> pwm(TIM1, 100);
//	pwm.Initialise();
//	pwm.Set_Frequency(600);
//	pwm.Set_Duty(80);
//	pwm.Enable();
//
	SSD1306 oled(&hi2c1, 64);
	oled.Initialize();
	oled.Set_Brightness(0xff);

	oled.Fill(SSD1306::WHITE);
	oled.Update_Screen();
	delay_ms(1000);
	oled.Fill(SSD1306::BLACK);
	oled.Update_Screen();
	delay_ms(500);

	while(1)
	{
	    oled.Set_Cursor(0, 0);
	        sprintf (buf, "%d", ((GPIOA->IDR)>>2) & 0x01);
	        oled.Write_String (buf);
	    oled.Set_Cursor(0, 15);
	        sprintf (buf, "%d", ((GPIOA->IDR)>>3) & 0x01);
	        oled.Write_String (buf);
	    oled.Set_Cursor(0, 30);
	        sprintf (buf, "%d", ((GPIOA->IDR)>>4) & 0x01);
	        oled.Write_String (buf);
	    oled.Set_Cursor(0, 45);
	        sprintf (buf, "%d", ((GPIOA->IDR)>>5) & 0x01);
	        oled.Write_String (buf);
	        /////////////
//	        oled.Set_Cursor(20, 0);
//	            sprintf (buf, "|%d", button_UP.State());
//	            oled.Write_String (buf);
//	        oled.Set_Cursor(20, 15);
//	            sprintf (buf, "|%d", button_DOWN.State());
//	            oled.Write_String (buf);
//	        oled.Set_Cursor(20, 30);
//	            sprintf (buf, "|%d", button_OK.State());
//	            oled.Write_String (buf);
//	        oled.Set_Cursor(20, 45);
//	            sprintf (buf, "|%d", button_BACK.State() );
//	            oled.Write_String (buf);
	        /////////////
	        oled.Set_Cursor(60, 0);
	            sprintf (buf, "b1=%d", b1);
	            oled.Write_String (buf);
	        oled.Set_Cursor(60, 15);
	            sprintf (buf, "b2=%d", b2);
	            oled.Write_String (buf);
	        oled.Set_Cursor(60, 30);
	            sprintf (buf, "b3=%d",b3);
	            oled.Write_String (buf);
	        oled.Set_Cursor(60, 45);
	            sprintf (buf, "b4=%d",b4 );
	            oled.Write_String (buf);
	            oled.Update_Screen();
	            HAL_Delay(100);
	}
//
//  //////////////////////////////////////////////////////////////////////////////////////////////
//  /////DAC
//
////  HAL_NVIC_SetPriority (DMA1_Channel6_IRQn, 2, 1);
////  HAL_NVIC_EnableIRQ (DMA1_Channel6_IRQn);
////
////  __HAL_RCC_DMA1_CLK_ENABLE();
////  DMA dma (DMA1_Channel6);
////
////  //DMA1_Channel6->CCR|=14;
////  I2C i2c_dma (I2C1, &dma);
////  i2c_dma.Initialise ();
////  i2c_dma.Enable_DMA ();
////  i2c_dma.Enable ();
////
////  MCP47FEB dac(i2c_dma);
////  error = dac.Set_Vref (MCP47FEB::Vref::VREF_BUFFERED);
////	oled.Set_Cursor(0, 0);
////	sprintf (buf, "dac.error1=%lu", error);
////	oled.Write_String (buf);
////  error = dac.Set_Continuous (sine_table, sine_table_lenght);
////	oled.Set_Cursor(0, 20);
////	sprintf (buf, "dac.error2=%lu", error);
////	oled.Write_String (buf);
////  dac.Enable_Output ();
////  oled.Update_Screen();
//
//
////////////////////////////////////////////////////
////OSCILLOSCOPE
///////////////////////////////////////////////////////////////////////////////
////	int eh = 0;
////	eh = Adc::Set_LCR();
////	oled.Clean();
////	oled.Set_Cursor(0, 0);
////	sprintf(buf, "dac.error1=%d", eh);
////	oled.Write_String(buf);
////	HAL_Delay(100);
////	eh = Adc::Set_Oscilloscope();
////	double lala = 0;
////
////	lala = Adc::Set_Sampling_time(Adc::SamplingTimeClocks::ADCCLK_239CYCLES5);
////
////	oled.Set_Cursor(0, 20);
////	sprintf(buf, "dac.error2=%d", eh);
////	oled.Write_String(buf);
////	oled.Set_Cursor(0, 30);
////	sprintf(buf, "dac.error3=%f", lala);
////	oled.Write_String(buf);
////	oled.Update_Screen();
////	HAL_Delay(1300);
////	while (1)
////	{
////		while (xD == 0)
////		{
////
////		}
////
////		xD = 0;
////		HAL_Delay(100);
////		oled.Clean();
////		Waveform_arythmetics::Calc_Moving_Average((uint32_t*) Adc::adc_buffer,
////				1024, 1);
////		uint32_t edge = Waveform_arythmetics::Get_Edge_index(1000, false);
////		if (edge >900)
////		{
////			Adc::Resume_DMA();
////			continue;
////		}
////		for (int i = 0; i < 100; i++)
////		{
////			display_buffer[i] = Waveform_arythmetics::filtered_buffer[0][i
////					+ edge] / 64;
////		}
////		sprintf(buf, "yol=%1.3f", lala);
////		oled.Set_Cursor(0, 0), oled.Write_String(buf);
////		sprintf(buf, "edge=%ld", edge);
////		oled.Set_Cursor(63, 0), oled.Write_String(buf);
////		oled.Draw_Waveform(10, 60, display_buffer, 100, SSD1306::WHITE);
////		oled.Update_Screen();
////
////		Adc::Resume_DMA();
////	}
//////
//////	HAL_Delay(1000);
////	////////////////////////////////////////////
////
////	//TEMP
////	/////////////////
//  Adc::Set_Voltage_Temperature ();
//  uint32_t Vmax=0;
//  uint32_t Vmin=99999;
//  uint32_t Vref=0;
//  int co=0;
//  while(1)
//  {
//      while (xD == 0)
//	{
//
//	}
//      xD = 0;
//      oled.Clean ();
//      Vref=Adc::Update_Vref ();
//      if(Vref<Vmin)
//      {
//    	  Vmin=Vref;
//      }
//      if(Vref>Vmax)
//      {
//    	  Vmax=Vref;
//      }
//      sprintf (buf, "Vref=%ld",Vref);
//      oled.Set_Cursor (0, 0), oled.Write_String (buf);
//      sprintf (buf, "temp=%d", Adc::Get_Temperature ());
//      oled.Set_Cursor (0, 10), oled.Write_String (buf);
//      sprintf (buf, "Vmax=%d", Vmax);
//      oled.Set_Cursor (0, 20), oled.Write_String (buf);
//      sprintf (buf, "Vmin=%d", Vmin);
//      oled.Set_Cursor (0, 30), oled.Write_String (buf);
//      oled.Update_Screen ();
//      HAL_Delay (100);
//      co++;
//      if(co>2)
//      {
//    	  break;
//      }
//    }
//// ///////////////////////////////////////////////////////////////////////////////////
//// //ADC
//// //////////////////////////////////////////////////////
//
//  oled.Clean ();
//  double lala = 0;
//  error = Adc::Set_LCR ();
//  lala = Adc::Set_Sampling_time (Adc::SamplingTimeClocks::ADCCLK_239CYCLES5);
//
//  Waveform_arythmetics::mid_voltage = 2000;
//  Waveform_arythmetics::hysteresis_samples=10;
//  Waveform_arythmetics::user_point_time = 21; //TODO get the Set_Sampling_time in uint
//
//  while (1)
//    {
//      while (xD == 0)
//	{
//
//	}
//      xD = 0;
//      HAL_Delay (500);
//      oled.Clean ();
//
//      Waveform_arythmetics::Calc_Moving_Average ((uint32_t*) Adc::adc_buffer,
//						 Adc::size_of_adc_buffer, 7); //TODO with 1 it's to chaotic can proccessing be improved?
//      Waveform_arythmetics::Find_Peaks ();
//      Waveform_arythmetics::Calc_Frequency();
//      Waveform_arythmetics::Calc_Alfa ();
//      Waveform_arythmetics::Calc_Amplitude ();
//
//      bool ind=LCR_math::Calculate (
//	  Adc::Adc_To_Milivolts (Waveform_arythmetics::amplitude1),
//	  Adc::Adc_To_Milivolts (Waveform_arythmetics::amplitude2),
//	  double(Waveform_arythmetics::alfa/1000), Waveform_arythmetics::frequency);
//
//      sprintf (buf, "cap=%1.9f", LCR_math::capacitance);
//      oled.Set_Cursor (0, 0), oled.Write_String (buf);
//      sprintf (buf, "ind=%1.9f", LCR_math::inductance);
//      oled.Set_Cursor (0, 10), oled.Write_String (buf);
//      sprintf (buf, "res=%1.9f", LCR_math::resistance);
//      oled.Set_Cursor (0, 20), oled.Write_String (buf);
////
//      sprintf (buf, "a1=%ld", Waveform_arythmetics::amplitude1);
//      oled.Set_Cursor (0, 30), oled.Write_String (buf);
//      sprintf (buf, "a2=%ld", Waveform_arythmetics::amplitude2);
//      oled.Set_Cursor (60, 30), oled.Write_String (buf);
//      sprintf (buf, "f=%ld", Waveform_arythmetics::frequency);
//      oled.Set_Cursor (0, 40), oled.Write_String (buf);
//      sprintf (buf, "a=%ld", Waveform_arythmetics::alfa);
//      oled.Set_Cursor (60, 40), oled.Write_String (buf);
//
//            sprintf (buf, "min=%d", Waveform_arythmetics::nbr_of_minimas[0]);
//            oled.Set_Cursor (00, 50), oled.Write_String (buf);
//            sprintf (buf, "max=%d",  Waveform_arythmetics::nbr_of_peaks[0]);
//            oled.Set_Cursor (40, 50), oled.Write_String (buf);
//            sprintf (buf, "ind=%d", Waveform_arythmetics::minimas[0][0]);
//            oled.Set_Cursor (80, 50), oled.Write_String (buf);
//
//        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
//        {
//            uint8_t aaa123[] =
//            { 0xff, 0x11, 0xab, 0xaa };     //start indication or sth
//            for (uint32_t i = 0; i < Waveform_arythmetics::buffer_size; i++)
//            {
//                HAL_UART_Transmit(u1, (uint8_t*) (aaa123), 4, 1000);
//                uint8_t x[4];
//                x[0] = Waveform_arythmetics::filtered_buffer[0][i] >> 8;
//                x[1] = Waveform_arythmetics::filtered_buffer[0][i] & 0xff;
//                x[2] = Waveform_arythmetics::filtered_buffer[1][i] >> 8;
//                x[3] = Waveform_arythmetics::filtered_buffer[1][i] & 0xff;
//                if (i == Waveform_arythmetics::peaks[0][0] && i != 0)
//                {
//                    x[0] = 0x0f;
//                    x[1] = 0x8f;
//                }
//                if (i == Waveform_arythmetics::minimas[0][0] && i != 0)
//                {
//                    x[0] = 0;
//                    x[1] = 5;
//                }
//                if (i == Waveform_arythmetics::peaks[1][0] && i != 0)
//                {
//                    x[2] = 0x0f;
//                    x[3] = 0x8f;
//                }
//                if (i == Waveform_arythmetics::minimas[1][0] && i != 0)
//                {
//                    x[2] = 0;
//                    x[3] = 5;
//                }
//
//                if (i == Waveform_arythmetics::peaks[0][1] && i != 0)
//                {
//                    x[0] = 0x0f;
//                    x[1] = 0x8f;
//                }
//                if (i == Waveform_arythmetics::minimas[0][1] && i != 0)
//                {
//                    x[0] = 0;
//                    x[1] = 5;
//                }
//                if (i == Waveform_arythmetics::peaks[1][1] && i != 0)
//                {
//                    x[2] = 0x0f;
//                    x[3] = 0x8f;
//                }
//                if (i == Waveform_arythmetics::minimas[1][1] && i != 0)
//                {
//                    x[2] = 0;
//                    x[3] = 5;
//                }
//                HAL_UART_Transmit(u1, &x[0], 4, 1000);
//
//            }
//
//            HAL_Delay(3000);
//        }
//        oled.Update_Screen();
//        Adc::Resume_DMA();
//    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_DUALMODE_REGSIMULT;
  multimode.DMAAccessMode = ADC_DMAACCESSMODE_12_10_BITS;
  multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
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

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.GainCompensation = 0;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.GainCompensation = 0;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief ADC4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC4_Init(void)
{

  /* USER CODE BEGIN ADC4_Init 0 */

  /* USER CODE END ADC4_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC4_Init 1 */

  /* USER CODE END ADC4_Init 1 */

  /** Common config
  */
  hadc4.Instance = ADC4;
  hadc4.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc4.Init.Resolution = ADC_RESOLUTION_12B;
  hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc4.Init.GainCompensation = 0;
  hadc4.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc4.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc4.Init.LowPowerAutoWait = DISABLE;
  hadc4.Init.ContinuousConvMode = DISABLE;
  hadc4.Init.NbrOfConversion = 1;
  hadc4.Init.DiscontinuousConvMode = DISABLE;
  hadc4.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc4.Init.DMAContinuousRequests = DISABLE;
  hadc4.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc4.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC4_Init 2 */

  /* USER CODE END ADC4_Init 2 */

}

/**
  * @brief ADC5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC5_Init(void)
{

  /* USER CODE BEGIN ADC5_Init 0 */

  /* USER CODE END ADC5_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC5_Init 1 */

  /* USER CODE END ADC5_Init 1 */

  /** Common config
  */
  hadc5.Instance = ADC5;
  hadc5.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc5.Init.Resolution = ADC_RESOLUTION_12B;
  hadc5.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc5.Init.GainCompensation = 0;
  hadc5.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc5.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc5.Init.LowPowerAutoWait = DISABLE;
  hadc5.Init.ContinuousConvMode = DISABLE;
  hadc5.Init.NbrOfConversion = 1;
  hadc5.Init.DiscontinuousConvMode = DISABLE;
  hadc5.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc5.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc5.Init.DMAContinuousRequests = DISABLE;
  hadc5.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc5.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc5) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc5, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC5_Init 2 */

  /* USER CODE END ADC5_Init 2 */

}

/**
  * @brief CORDIC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CORDIC_Init(void)
{

  /* USER CODE BEGIN CORDIC_Init 0 */

  /* USER CODE END CORDIC_Init 0 */

  /* USER CODE BEGIN CORDIC_Init 1 */

  /* USER CODE END CORDIC_Init 1 */
  hcordic.Instance = CORDIC;
  if (HAL_CORDIC_Init(&hcordic) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CORDIC_Init 2 */

  /* USER CODE END CORDIC_Init 2 */

}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_ABOVE_160MHZ;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief DAC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC2_Init(void)
{

  /* USER CODE BEGIN DAC2_Init 0 */

  /* USER CODE END DAC2_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC2_Init 1 */

  /* USER CODE END DAC2_Init 1 */

  /** DAC Initialization
  */
  hdac2.Instance = DAC2;
  if (HAL_DAC_Init(&hdac2) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  sConfig.DAC_DMADoubleDataMode = DISABLE;
  sConfig.DAC_SignedFormat = DISABLE;
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_Trigger2 = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_EXTERNAL;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac2, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Triangle wave generation on DAC OUT1
  */
  if (HAL_DACEx_TriangleWaveGenerate(&hdac2, DAC_CHANNEL_1, DAC_TRIANGLEAMPLITUDE_4095) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC2_Init 2 */

  /* USER CODE END DAC2_Init 2 */

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
  hi2c1.Init.Timing = 0x30A0A7FB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4.294967295E9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 53600;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EN_PATH_1_Pin|RES_CALIBRATION_Pin|EN_PATH_2_Pin|EN_PATH_3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF15_EVENTOUT;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON_DOWN_Pin BUTTON_UP_Pin V_USB_IN_Pin BUTTON_OK_Pin
                           BUTTON_BACK_Pin */
  GPIO_InitStruct.Pin = BUTTON_DOWN_Pin|BUTTON_UP_Pin|V_USB_IN_Pin|BUTTON_OK_Pin
                          |BUTTON_BACK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : EN_PATH_1_Pin RES_CALIBRATION_Pin EN_PATH_2_Pin EN_PATH_3_Pin */
  GPIO_InitStruct.Pin = EN_PATH_1_Pin|RES_CALIBRATION_Pin|EN_PATH_2_Pin|EN_PATH_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_EN_Pin */
  GPIO_InitStruct.Pin = USB_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_EN_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

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
