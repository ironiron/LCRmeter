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

SSD1306 oled(&hi2c1, 64);

static       char buf[30];

void error_informer(int error,int line)
{
    oled.Set_Font_size(Fonts::font_16x26);
    oled.Set_Cursor(0, 0);
    oled.Write_String("Error:");
    oled.Set_Cursor(0, 30);
    char buf [20];
    snprintf(buf,20,"%d_%d",line,error);
    oled.Write_String(buf);
    oled.Update_Screen();
    HAL_Delay(3000);

    //now do a reset, we don't have recoverable solution for now
    NVIC_SystemReset();
}

#define LOG(x) error_informer(static_cast<int>((x)),__LINE__);

/*
 * @brief Handles cleaning after mode has been changed
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


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    printf("dd\n");
    if(hadc == &hadc1)
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
    if(hadc == &hadc5)
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

void Init_LCR_Measurment([[maybe_unused]] unsigned int i)
{
       DeInit();
       Set_DAC_Frequency(sine_samples_t::SINE_400_SAMPELS);
       Set_Rseries(Rseries_t::R_100);
       state = LCR;

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

    if (Waveform_arythmetics::nbr_of_peaks[1] < 2
            || Waveform_arythmetics::nbr_of_peaks[0] < 2)
    {
        printf("WARNING!!!!!!! number of peaks too small!!\n");
        printf("WARNING!!!!!!! number of peaks too small!!\n");
        printf("WARNING!!!!!!! number of peaks too small!!\n");
    }
    else
    {
        bool ind = LCR_math::Calculate(
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
        sprintf(buf, "avg_cap=%5.9f F\n", cap.average());
        oled.Set_Cursor(40, 0);
        oled.Write_String(buf);
        sprintf(buf, "avg_ind=%5.9f H\n", in.average());
        oled.Set_Cursor(40, 10);
        oled.Write_String(buf);
        sprintf(buf, "avg_res=%5.9f R\n", res.average());
        oled.Set_Cursor(40,20);
        oled.Write_String(buf);
        sprintf(buf, "avg_los=%5.9f deg\n", los.average());
        oled.Set_Cursor(40, 30);
        oled.Write_String(buf);
        sprintf(buf, "avg_ang=%5.9f deg\n", ang.average());
        oled.Set_Cursor(40, 40);
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

void adjust_LCR_settings_clb(unsigned int index)
{

}

auto menu_main=Menu01_base::menu_list("",{"LCR","OSC","BAT","PWM"});
auto menu_lcr=Menu01_base::menu_list("LCR",{"Frequency","R series"},&menu_main,0);
auto menu_osc=Menu01_base::menu_list("Oscilloscope",{"Sample period"},&menu_main,1);
auto menu_pwm=Menu01_base::menu_list("PWM",{"DUTY","FREQUENCY"},&menu_main,2);
Menu01 menu=Menu01(&menu_main);

void Loop(void)
{
   //First create menu

//   auto menu_bat=Menu01_base::menu_list("battery",{"Sample period"},&main_page,2);

   menu_main.Register_callback(0, Init_LCR_Measurment);
   menu_main.Register_callback(2, Init_V_Temp);


   oled.Initialize();
   oled.Set_Brightness(0xff);

   oled.Fill(SSD1306::WHITE);
   oled.Update_Screen();
   delay_ms(500);
   oled.Fill(SSD1306::BLACK);
   oled.Update_Screen();
   Init_LCR_Measurment(0);

   while(1)
   {
//    let's check if buttons are pressed
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

    oled.Fill(SSD1306::BLACK);
    oled.Set_Cursor(50, 0);
    oled.Write_String(menu.PageName().data());

    for (int i = 0; i < 6; ++i) {
        oled.Set_Cursor(0, (uint8_t)i*10+3);
        oled.Write_String(menu_main.Get_item(i).data());
    }
    oled.Set_Cursor(0, (uint8_t)menu.CurrentItemInd()*10+3);
    oled.Write_String_Inverted(menu.CurrentItemStr().data());

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

    oled.Update_Screen();

   }

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
}


