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
#include "Menu01.hpp"

//TODO make those XY coordinates on GUI a little bit more concise, maybe make some #define?


/**
 * @brief in this file most of the logic is present
 *
 * ADC
 */

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern ADC_HandleTypeDef hadc5;
extern DMA_HandleTypeDef hdma_adc1;
extern CORDIC_HandleTypeDef hcordic;
extern DMA_HandleTypeDef hdma_dac1_ch1;
extern I2C_HandleTypeDef hi2c1; // for oled display


volatile unsigned int button_up=0;
volatile unsigned int button_down=0;
volatile unsigned int button_ok=0;
volatile unsigned int button_back=0;

volatile bool osc_complete;
volatile bool vref_complete;

constexpr unsigned int max_average = 20;
MovingAverager<uint32_t, max_average, false> vref_avg;
MovingAverager<decltype(LCR_math::capacitance), max_average, true> cap;
MovingAverager<decltype(LCR_math::inductance), max_average, true> in;
MovingAverager<decltype(LCR_math::resistance), max_average, true> res;
MovingAverager<decltype(LCR_math::loss_angle), max_average, true> los;
MovingAverager<decltype(Waveform_arythmetics::alfa), max_average, true> ang;

 enum {
    LCR,
    OSCILLOSCOPE,
    V_TEMP,

} state = LCR;

enum {
   NONE,
   FREQ,
   R_SER,

} settings_state = NONE;

SSD1306 oled(&hi2c1, 64);

static       char buf[30];

auto menu_main=Menu01_base::menu_list("",{"LCR","OSC","BAT","PWM"});
auto menu_lcr=Menu01_base::menu_list("LCR",{"freq","R_ser"},&menu_main,0);
auto menu_osc=Menu01_base::menu_list("Oscilloscope",{"Sample period"},&menu_main,1);
auto menu_pwm=Menu01_base::menu_list("PWM",{"DUTY","FREQUENCY"},&menu_main,3);
//
auto menu_lcr_freq=Menu01_base::menu_list("",{""},&menu_lcr,0);
auto menu_lcr_r_ser=Menu01_base::menu_list("",{""},&menu_lcr,1);
Menu01 menu=Menu01(&menu_main);

void error_informer(int error,int line)
{
    oled.Set_Font_size(Fonts::font_16x26);
    oled.Set_Cursor(0, 0);
    oled.Write_String("Error:");
    oled.Set_Cursor(0, 30);
    snprintf(buf,20,"%d_%d",line,error);
    oled.Write_String(buf);
    oled.Update_Screen();
    HAL_Delay(3000);

    //now do a reset, we don't have recoverable solution for now
    NVIC_SystemReset();
}

#define LOG(x) error_informer(static_cast<int>((x)),__LINE__);

/*
 * @brief Handles cleaning after ADC mode has been changed
 */
void DeInit(void)
{
    HAL_ADC_Stop_DMA(&hadc5);
    HAL_ADCEx_MultiModeStop_DMA(&hadc1);
    HAL_ADC_Stop(&hadc2);
    HAL_Delay(20); //let's put some artificial delay to finish any ongoing transaction
    hadc1.State = HAL_ADC_STATE_READY;//overwrite any lingering error in HAL state machine
    hadc2.State = HAL_ADC_STATE_READY;//overwrite any lingering error in HAL state machine
}


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

void Do_Oscilloscope(void)
{
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
}

/*
 * @brief Callback invoked when ADC completed
 * @param hadc based on this will evaluate results
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
//    printf("dd\n");
    if(hadc == &hadc1) // LCR
    {
        Waveform_arythmetics::Calc_Moving_Average((uint32_t*) Adc::adc_buffer,
                Adc::size_of_adc_buffer, 1);

        Waveform_arythmetics::Find_Peaks();
        Waveform_arythmetics::Calc_Frequency();
        Waveform_arythmetics::Calc_Alfa();
        Waveform_arythmetics::Calc_Amplitude();

        if (Adc::Start_LCR())
        {
            LOG(1);
        }
    }
    if(hadc == &hadc5) // VBAT
    {
        vref_avg.insert(Adc::Get_Vref());
        //if averager is filled let;s update the batterry voltage
        if(vref_avg.get(0) != 0)
        {
            Adc::Update_Vref(vref_avg.average());
        }

       auto  ret = HAL_ADC_Start_DMA(&hadc5, (uint32_t*) Adc::volt_temp, 2);
        if (ret != 0)
        {
            LOG(ret)
        }
    }
}

/*
 * @brief Initializes LCR measurment with previously used settings
 * see \ref HAL_ADC_ConvCpltCallback for how the results are evaluated
 * @param i unused
 */
void Init_LCR_Measurment([[maybe_unused]] unsigned int i)
{
       DeInit();
       Set_DAC_Frequency(Get_DAC_Frequency());
       Set_Rseries(Get_Rseries());
       state = LCR;
       settings_state = NONE;

       auto retval = HAL_ADC_Start(&hadc2);
       if (retval != 0)
       {
           LOG(retval)
       }
       if (Adc::Start_LCR())
       {
           LOG(1);
       }
}

/*
 * @brief Starts Vbat and temperature measurment
 *  see \ref HAL_ADC_ConvCpltCallback for how the results are evaluated
 * @param i unused
 */
void Init_V_Temp([[maybe_unused]] unsigned int i)
{
    DeInit();
    vref_avg.reset();
    state = V_TEMP;

    auto ret = HAL_ADC_Start_DMA(&hadc5, (uint32_t*) Adc::volt_temp, 2);
    if (ret != 0)
    {
        LOG(ret)
    }
}

void Print_LCR(void)
{
  static  int counter=0;

  //TODO move logic to clb, here only printing!

    if (Waveform_arythmetics::nbr_of_peaks[1] < 2
            || Waveform_arythmetics::nbr_of_peaks[0] < 2)
    {
        printf("WARNING!!!!!!! number of peaks too small!!\n");
        printf("WARNING!!!!!!! number of peaks too small!!\n");
        printf("WARNING!!!!!!! number of peaks too small!!\n");
    }
    else
    {
        LCR_math::Calculate(
                Adc::Adc_To_Milivolts(Waveform_arythmetics::amplitude1),
                Adc::Adc_To_Milivolts(Waveform_arythmetics::amplitude2),
                double(Waveform_arythmetics::alfa),
                Waveform_arythmetics::frequency);

        cap.insert(LCR_math::capacitance);
        in.insert(LCR_math::inductance);
        res.insert(LCR_math::resistance);
        los.insert(LCR_math::loss_angle);
        ang.insert(Waveform_arythmetics::alfa);
    }
        counter = 0;
        printf("*********************************************************\n");
        sprintf(buf, "C=%5.9f F\n", cap.average());
        oled.Set_Cursor(35, 25);
        oled.Write_String(buf);
        sprintf(buf, "L=%5.9f H\n", in.average());
        oled.Set_Cursor(35, 35);
        oled.Write_String(buf);
        sprintf(buf, "R=%5.9f R\n", res.average());
        oled.Set_Cursor(35,45);
        oled.Write_String(buf);
        sprintf(buf, "al=%2.3f\n", los.average());
        oled.Set_Cursor(35, 55);
        oled.Write_String(buf);
        sprintf(buf, "ang=%2.3f\n", ang.average());
        oled.Set_Cursor(80, 55);
        oled.Write_String(buf);
//
//        test2++;
//        if (test2 > 2)
//        {
//            test2 = 0;
//            test1++;
//        }
//        if (test1 > 3)
//        {
//            test1 = 0;
//        }
//        Set_DAC_Frequency(sine_samples_t(test1));
//        Set_Rseries(Rseries_t(test2));
//        if (test2 == 0)
//        {
//            LCR_math::series_resistance = 100;
//        }
//        if (test2 == 1)
//        {
//            LCR_math::series_resistance = 820;
//        }
//        if (test2 == 2)
//        {
//            LCR_math::series_resistance = 6;
//        }

    //        print_result();
}

void adjust_LCR_rser_settings_clb([[maybe_unused]] unsigned int index)
{
        settings_state = R_SER;
}

void adjust_LCR_freq_settings_clb([[maybe_unused]] unsigned int index)
{
        settings_state = FREQ;
}

void Handle_GUI(void)
{
    //FIXME take into account parent's size, this if menu.HasParent shall be in external for loop!
    oled.Fill(SSD1306::BLACK);
    for (unsigned int i = 0; i < menu.GetMenuLength(); ++i)
    {
        oled.Set_Cursor(0, (uint8_t) i * 10 + 3);
        if (menu.HasParent())
        {
            oled.Write_String(menu.GetParent()->Get_item(i).data());
        }
        else
        {
            oled.Write_String(menu.GetListItem(i).data());
        }
    }
    if (menu.HasParent())
    {
        auto parent_index = menu.GetParent()->Get_parent_index();
        oled.Set_Cursor(0, (uint8_t) parent_index * 10 + 3);
        oled.Write_String_Inverted(
                menu.GetParent()->Get_item(parent_index).data());

        for (int i = 0; i < menu.GetMenuLength(); ++i)
        {
            oled.Set_Cursor(35, (uint8_t) i * 10 + 3);
            oled.Write_String(menu.GetListItem(i).data());
        }
        oled.Set_Cursor(35, (uint8_t) menu.CurrentItemInd() * 10 + 3);
        oled.Write_String_Inverted(menu.CurrentItemStr().data());
    }
    else
    {
        oled.Set_Cursor(0, (uint8_t) menu.CurrentItemInd() * 10 + 3);
        oled.Write_String_Inverted(menu.CurrentItemStr().data());
    }

    switch (state)
    {
        case LCR:
//            Do_LCR_Measurment();
            Print_LCR();
            break;

        case OSCILLOSCOPE:
//            Do_Oscilloscope();
            break;
        case V_TEMP:
            sprintf(buf, "%04lu mV", vref_avg.average());
            oled.Set_Cursor(40, 20);
            oled.Write_String(buf);
            sprintf(buf, "%d deg. C", Adc::Get_Temperature());
            oled.Set_Cursor(40, 40);
            oled.Write_String(buf);
            break;

        default:
            break;
    }

    if (settings_state == FREQ)
    {

        oled.Set_Cursor(35, 3);
        sprintf(buf, "%d", Get_DAC_Frequency());
        oled.Write_String_Inverted(buf);
    }
    if (settings_state == R_SER)
    {

        oled.Set_Cursor(35, 13);
        sprintf(buf, "%d", Get_Rseries());
        oled.Write_String_Inverted(buf);
    }

    oled.Update_Screen();
}

void Loop(void)
{
   //Update VBAT as the first step!
    Init_V_Temp(0);

    //First create menu's callbacks
   menu_main.Register_callback(0, Init_LCR_Measurment);
   menu_main.Register_callback(2, Init_V_Temp);
   //settings
   menu_lcr_freq.Register_callback(0, adjust_LCR_freq_settings_clb);
   menu_lcr_r_ser.Register_callback(0, adjust_LCR_rser_settings_clb);


   oled.Initialize();
   oled.Set_Brightness(0xff);
   oled.Fill(SSD1306::WHITE);
   oled.Update_Screen();

   delay_ms(500); // give some time for battery voltage update
   oled.Fill(SSD1306::BLACK);
   oled.Update_Screen();

   printf("Vbat is %ld\n",Adc::vref);

   //after this point VBAT should be ready so let's start normal operation
   Init_LCR_Measurment(0);

   while(1)
   {

//    let's check if buttons are pressed
       if(settings_state == NONE)
       {
           if(button_up)
           {
               button_up =0;
               menu.MoveUp();
           }
           if(button_down)
           {
               button_down = 0;
               menu.MoveDown();
           }
           if(button_ok)
           {
               button_ok = 0;
               menu.GoIn();
           }
           if(button_back)
           {
               button_back=0;
               menu.GoOut();
           }
       }
       else if(settings_state == FREQ)
       {
           if(button_up)
           {
               button_up =0;
               Increase_DAC_frequncy();
           }
           if(button_down)
           {
               button_down = 0;
               Decrease_DAC_frequncy();

           }
           if(button_back)
           {
               button_back=0;
               settings_state = NONE;
               menu.GoOut();
           }

       }
       else if(settings_state == R_SER)
       {
           if(button_up)
           {
               button_up =0;
               Increase_Rseries();
           }
           if(button_down)
           {
               button_down = 0;
               Decrease_Rseries();

           }
           if(button_back)
           {
               button_back=0;
               settings_state = NONE;
               menu.GoOut();
           }

       }

    Handle_GUI();

   }
}
