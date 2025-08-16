/*
 * Maintask.cpp
 *
 *  Created on: Jun 21, 2025
 *      Author: xDDD
 */

#include <stdio.h>
#include <string>
#include <vector>
#include "Maintask.h"
#include "LLutils.h"
#include "usb_device.h"
#include "Pwm.hpp"
#include "delay.h"
#include "Waveformarythmetics.hpp"
#include "SSD1306.hpp"
#include "adc.hpp"
#include "LCRmath.hpp"
#include "sine.hpp"
#include "MovingAverager.hpp"
#include "Maintask.h"
#include "common_defines.hpp"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern ADC_HandleTypeDef hadc5;
extern DMA_HandleTypeDef hdma_adc1;

extern CORDIC_HandleTypeDef hcordic;

extern DAC_HandleTypeDef hdac1;
extern DAC_HandleTypeDef hdac2;
extern DMA_HandleTypeDef hdma_dac1_ch1;

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;

extern volatile int xD;

volatile unsigned int b1=0;
volatile unsigned int b2=0;
volatile unsigned int b3=0;
volatile unsigned int b4=0;

volatile bool osc_complete;
volatile bool vref_complete;

/*
 * @brief for debugging
 */
void print_result(void)
{

    printf("a1=%ld\n", Waveform_arythmetics::amplitude1);
    printf("a2=%ld\n", Waveform_arythmetics::amplitude2);
    printf("f=%ld\n", Waveform_arythmetics::frequency);
    printf("a=%f\n", Waveform_arythmetics::alfa);
    printf("a1=%ld volts\n",
            Adc::Adc_To_Milivolts(Waveform_arythmetics::amplitude1));
    printf("a2=%ld volts\n",
            Adc::Adc_To_Milivolts(Waveform_arythmetics::amplitude2));

    printf("nbr_of_minimas[0]=%ld\n", Waveform_arythmetics::nbr_of_minimas[0]);
    printf("nbr_of_peaks[0]=%ld\n", Waveform_arythmetics::nbr_of_peaks[0]);
    printf("nbr_of_minimas[1]=%ld\n", Waveform_arythmetics::nbr_of_minimas[1]);
    printf("nbr_of_peaks[1]=%ld\n", Waveform_arythmetics::nbr_of_peaks[1]);
    printf("nbr_of_peaks[1]=%ld\n", Waveform_arythmetics::nbr_of_peaks[1]);
    printf("peaks[0][0]=%ld\n", Waveform_arythmetics::peaks[0][0]);
    printf("peaks[0][1]=%ld\n", Waveform_arythmetics::peaks[0][1]);
    printf("peaks[1][0]=%ld\n", Waveform_arythmetics::peaks[1][0]);
    printf("peaks[1][1]=%ld\n", Waveform_arythmetics::peaks[1][1]);
    printf("peaks[0][2]=%ld\n", Waveform_arythmetics::peaks[0][2]);
    printf("peaks[1][2]=%ld\n", Waveform_arythmetics::peaks[1][2]);

    printf("minimas=%ld\n", Waveform_arythmetics::minimas[0][0]);

    printf("cap=%1.9f F\n", LCR_math::capacitance);
    printf("ind=%1.9f H\n", LCR_math::inductance);
    printf("res=%1.9f R\n", LCR_math::resistance);
    printf("loss angle=%1.9f degs\n", LCR_math::loss_angle);
    for (unsigned int i = 0; i < Waveform_arythmetics::buffer_size; i++)
    {

        printf("%d, %d\n", Waveform_arythmetics::filtered_buffer[0][i],
                Waveform_arythmetics::filtered_buffer[1][i]);
        if (Waveform_arythmetics::filtered_buffer[0][i] == 0)
        {
            printf("i= %d (from back = %ld\n", i, Adc::size_of_adc_buffer - i);
            break;
        }
    }
    printf("hadc1---%ld\n", hadc1.ErrorCode);
    printf("hadc2---%ld\n", hadc2.ErrorCode);
    HAL_Delay(2000);
}

void Loop(void)
{
    constexpr unsigned int max_average = 20;
    MovingAverager<decltype(LCR_math::capacitance), max_average, true> cap;
    MovingAverager<decltype(LCR_math::inductance), max_average, true> in;
    MovingAverager<decltype(LCR_math::resistance), max_average, true> res;
    MovingAverager<decltype(LCR_math::loss_angle), max_average, true> los;
    MovingAverager<decltype(Waveform_arythmetics::alfa), max_average, true> ang;
    MovingAverager<uint32_t, max_average, false> vref_avg;
    unsigned int counter=0;

    char buf[30];



    SSD1306 oled(&hi2c1, 64);
    oled.Initialize();
    oled.Set_Brightness(0xff);

    oled.Fill(SSD1306::WHITE);
    oled.Update_Screen();
    delay_ms(1000);
    oled.Fill(SSD1306::BLACK);
    oled.Update_Screen();
    delay_ms(500);

    Set_DAC_Frequency(sine_samples_t::SINE_400_SAMPELS);
    Set_Rseries(Rseries_t::R_100);

    auto retval = HAL_ADC_Start(&hadc2);
    if (retval != 0)
    {
        printf("AAAA111\n");
        printf("retval = %d\n", retval);
    }
    Adc::Start_LCR();

//    while (1)
//    {
//        while (xD == 0)
//        {
//
//        }
//        xD = 0;
//
//
////        if(b1)
////        {
////            b1=0;
////            HAL_GPIO_WritePin(EN_PATH_1_GPIO_Port, EN_PATH_1_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_2_GPIO_Port, EN_PATH_2_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_3_GPIO_Port, EN_PATH_3_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(RES_CALIBRATION_GPIO_Port, RES_CALIBRATION_Pin, GPIO_PIN_RESET);
////
////            HAL_GPIO_WritePin(EN_PATH_1_GPIO_Port, EN_PATH_1_Pin, GPIO_PIN_SET);
////        }
////        if(b2)
////        {
////            b2=0;
////            HAL_GPIO_WritePin(EN_PATH_1_GPIO_Port, EN_PATH_1_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_2_GPIO_Port, EN_PATH_2_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_3_GPIO_Port, EN_PATH_3_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(RES_CALIBRATION_GPIO_Port, RES_CALIBRATION_Pin, GPIO_PIN_RESET);
////
////            HAL_GPIO_WritePin(EN_PATH_2_GPIO_Port, EN_PATH_2_Pin, GPIO_PIN_SET);
////        }
////        if(b3)
////        {
////            b3=0;
////            HAL_GPIO_WritePin(EN_PATH_1_GPIO_Port, EN_PATH_1_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_2_GPIO_Port, EN_PATH_2_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_3_GPIO_Port, EN_PATH_3_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(RES_CALIBRATION_GPIO_Port, RES_CALIBRATION_Pin, GPIO_PIN_RESET);
////
////            HAL_GPIO_WritePin(EN_PATH_3_GPIO_Port, EN_PATH_3_Pin, GPIO_PIN_SET);
////        }
////        if(b4)
////        {
////            b4=0;
////
////            HAL_GPIO_WritePin(EN_PATH_1_GPIO_Port, EN_PATH_1_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_2_GPIO_Port, EN_PATH_2_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(EN_PATH_3_GPIO_Port, EN_PATH_3_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(RES_CALIBRATION_GPIO_Port, RES_CALIBRATION_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(RES_CALIBRATION_GPIO_Port, RES_CALIBRATION_Pin, GPIO_PIN_SET);
////        }
//
//        Waveform_arythmetics::Calc_Moving_Average((uint32_t*) Adc::adc_buffer,
//                Adc::size_of_adc_buffer, 1);
//
//        Waveform_arythmetics::Find_Peaks();
//        Waveform_arythmetics::Calc_Frequency();
//        Waveform_arythmetics::Calc_Alfa();
//        Waveform_arythmetics::Calc_Amplitude();
//
//        if (Waveform_arythmetics::nbr_of_peaks[1] < 2
//                || Waveform_arythmetics::nbr_of_peaks[0] < 2)
//        {
//            printf("WARNING!!!!!!! number of peaks too small!!\n");
//            printf("WARNING!!!!!!! number of peaks too small!!\n");
//            printf("WARNING!!!!!!! number of peaks too small!!\n");
//        }
//        else
//        {
//            bool ind = LCR_math::Calculate(
//                    Adc::Adc_To_Milivolts(Waveform_arythmetics::amplitude1),
//                    Adc::Adc_To_Milivolts(Waveform_arythmetics::amplitude2),
//                    double(Waveform_arythmetics::alfa),
//                    Waveform_arythmetics::frequency);
//
//            cap.insert(LCR_math::capacitance);
//            in.insert(LCR_math::inductance);
//            res.insert(LCR_math::resistance);
//            los.insert(LCR_math::loss_angle);
//            ang.insert(Waveform_arythmetics::alfa);
//        }
//        counter++;
//        if(counter > max_average)
//        {
//            counter = 0;
//            printf("*********************************************************\n");
//            printf("avg_cap=%1.9f F\n", cap.average());
//            printf("avg_ind=%1.9f H\n", in.average());
//            printf("avg_res=%1.9f R\n", res.average());
//            printf("avg_los=%1.9f deg\n", los.average());
//            printf("avg_ang=%1.9f deg\n", ang.average());
//            printf("res = %d freq = %d\n",test2,test1);
//            cap.reset();
//            in.reset();
//            res.reset();
//            los.reset();
//            ang.reset();
//
//            test2++;
//            if (test2 > 2)
//            {
//                test2 = 0;
//                test1++;
//            }
//            if (test1 > 3)
//            {
//                test1 = 0;
//            }
//            Set_DAC_Frequency(sine_samples_t(test1));
//            Set_Rseries(Rseries_t(test2));
//            if(test2 == 0)
//            {
//                LCR_math::series_resistance = 100;
//            }
//            if(test2 == 1)
//            {
//                LCR_math::series_resistance = 820;
//            }
//            if(test2 == 2)
//            {
//                LCR_math::series_resistance = 6;
//            }
//        }
//
////        print_result();
//
//        HAL_Delay(100);
//        Adc::Start_LCR();
//
//    }
    //////////////////////////////////////////////////
    //OSCILLOSCOPE
    /////////////////////////////////////////////////////////////////////////////
      uint8_t display_buffer[100];

//        lala = Adc::Set_Sampling_time(Adc::SamplingTimeClocks::ADCCLK_239CYCLES5);
      //todo implement varying pulse lengtht

//        for (unsigned int i = 0; i < Waveform_arythmetics::buffer_size; i++)
//        {
//
//            printf("%d, %d\n", Waveform_arythmetics::filtered_buffer[0][i],
//                    Waveform_arythmetics::filtered_buffer[1][i]);
//        }
//        Adc::Start_Oscilloscope();
//        HAL_Delay(300);
//        while (1)
//        {
//            while (osc_complete == false)
//            {
//
//            }
//            printf("================================================================\n");
//            HAL_Delay(300);
//            osc_complete = false;
//            oled.Clean();
//            Waveform_arythmetics::Calc_Moving_Average((uint32_t*) Adc::adc_buffer,
//                    1000, 1);
//            uint32_t edge = 0;
//            // todo implement edge detection
////            uint32_t edge = Waveform_arythmetics::Get_Edge_index(1000, false);
////            if (edge >900)
////            {
////                ret = HAL_ADC_Start_DMA(&hadc3, Adc::adc_buffer, Adc::size_of_adc_buffer);
////                printf("hadc3 ret = %d\n",ret);
////                continue;
////            }
//
//            for (int i = 0; i < 100; i++)
//            {
//                display_buffer[i] = Waveform_arythmetics::filtered_buffer[0][i
//                        + edge] / 64;//4096/64 is 64 (max oled height resolution)
//            }
//            sprintf(buf, "edge=%ld", edge);
//            oled.Set_Cursor(63, 0), oled.Write_String(buf);
//            oled.Draw_Waveform(10, 63, display_buffer, 100, SSD1306::WHITE);
//            oled.Update_Screen();
//
////            for (unsigned int i = 0; i < Waveform_arythmetics::buffer_size; i++)
////            {
////
////                printf("%d, %d\n", Waveform_arythmetics::filtered_buffer[0][i],
////                        Waveform_arythmetics::filtered_buffer[1][i]);
////            }
//            Adc::Start_Oscilloscope();
//        }

    ////    ////////////////////////////////////////////
    ////
    ////    //TEMP
    ////    /////////////////

      uint32_t Vmax=0;
      uint32_t Vmin=99999;
      uint32_t Vref=0;
      printf("================================================================\n");
      printf("Internal Vref calibration value:\n");


      printf("%d\n",(*(volatile uint16_t *)0x1FFF75AA));

//      auto ret = HAL_ADC_Start_DMA(&hadc5, Adc::adc_buffer, Adc::size_of_adc_buffer);
//      HAL_ADC_PollForConversion
//      auto ret = HAL_ADC_PollForConversion(&hadc5,2000);
      auto ret = HAL_ADC_Start_DMA(&hadc5, (uint32_t*) Adc::volt_temp,
                 2);
     if (ret != 0)
     {
         printf("hadc5 ret = %d\n",ret);
     }

     vref_avg.reset();

      while(1)
      {

          while (vref_complete==false)
      {

      }
          vref_complete = false;

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
          vref_avg.insert(Vref);

          sprintf (buf, "Vref=%04ld",Vref);
          oled.Set_Cursor (0, 10), oled.Write_String (buf);
          sprintf (buf, " avg =  %04ld",vref_avg.average());
          oled.Set_Cursor (0, 40), oled.Write_String (buf);
          sprintf (buf, "temp=%d", Adc::Get_Temperature ());
          oled.Set_Cursor (0, 30), oled.Write_String (buf);
          sprintf (buf, "Vmax=%04ld", Vmax);
          oled.Set_Cursor (0, 0), oled.Write_String (buf);
          sprintf (buf, "Vmin=%04ld", Vmin);
          oled.Set_Cursor (0, 20), oled.Write_String (buf);

          printf("%d, %d\n", Adc::volt_temp[0],Adc::volt_temp[1]);

          oled.Update_Screen ();
          HAL_Delay (100);

          ret = HAL_ADC_Start_DMA(&hadc5, (uint32_t*) Adc::volt_temp,
                           2);
         if (ret != 0)
         {
             printf("hadc5 ret = %d\n",ret);
         }
        }
}
