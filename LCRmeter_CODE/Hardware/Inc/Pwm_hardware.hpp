/**
  ******************************************************************************
  * @file    Pwm_hardware.hpp
  * @author  Rafał Mazurkiewicz
  * @date    14.05.2019
  * @brief   Class file header for PWM signal generation
  ******************************************************************************
  * @attention
  * <h2><center>&copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz</center></h2>
  ******************************************************************************
  */
//TODO add doxygen to PWM lib

#ifndef FAKES_PWM_HARDWARE_HPP_
#define FAKES_PWM_HARDWARE_HPP_

#include <Pwm.hpp>
#include "stm32g4xx_hal.h"

template < class  T, typename width,uint8_t chn>
  void Pwm<T,width,chn>::Initialise (void)
{
  _timer->SR=0;
  _timer->CR1|=TIM_CR1_ARPE;
  _timer->BDTR |= TIM_BDTR_MOE;
  _timer->CCMR1|=TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1| TIM_CCMR1_OC2PE| TIM_CCMR1_OC2FE;
  _timer->CCER=TIM_CCER_CC2E;
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Set_Prescaler (width psc)
{
  _timer->PSC=psc;
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Set_Compare (width comp)
{
  switch(chn)
  {
    case 1:
      _timer->CCR1=comp;
      break;
    case 2:
      _timer->CCR2=comp;
      break;
    case 3:
      _timer->CCR3=comp;
      break;
    case 4:
      _timer->CCR4=comp;
      break;
    default:
      _timer->CCR1=comp;
      _timer->CCR2=comp;
      _timer->CCR3=comp;
      _timer->CCR4=comp;
      break;
  }
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Set_Counter (width cnt)
{
  _timer->ARR=cnt;
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width,uint8_t chn>
uint32_t Pwm<T,width,chn>::Get_Clock (void)
{
  return 72000000;//TODO implement actual freq
}

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Enable (void)
{
  _timer->CR1|=TIM_CR1_CEN;
}

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Disable (void)
{
  _timer->CR1&=~TIM_CR1_CEN;
}

#endif /* FAKES_PWM_HARDWARE_HPP_ */
