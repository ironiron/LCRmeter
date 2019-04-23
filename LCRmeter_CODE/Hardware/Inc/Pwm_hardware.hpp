/*
 * Pwm_hardware.hpp
 *
 *  Created on: 18.04.2019
 *      Author: Rafa³ Mazurkiewicz
 */

#ifndef FAKES_PWM_HARDWARE_HPP_
#define FAKES_PWM_HARDWARE_HPP_

#include <Pwm.hpp>
#include "stm32f1xx_hal.h"


template <class T, typename width>
void Pwm<T,width>::Set_Prescaler (width psc)
{
  _timer->PSC=psc;
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width>
void Pwm<T,width>::Set_Compare (width comp)
{
  _timer->CCR2=comp;
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width>
void Pwm<T,width>::Set_Counter (width cnt)
{
  _timer->ARR=cnt;
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width>
uint32_t Pwm<T,width>::Get_Clock (void)
{
  return 72000000;
}

template <class T, typename width>
void Pwm<T,width>::Initialise (void)
{
  _timer->SR=0;
  _timer->CR1|=TIM_CR1_ARPE;
  _timer->BDTR |= TIM_BDTR_MOE;
  _timer->CCMR1|=TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1| TIM_CCMR1_OC2PE| TIM_CCMR1_OC2FE;
  _timer->CCER=TIM_CCER_CC2E;
  _timer->EGR|=TIM_EGR_UG;
}

template <class T, typename width>
void Pwm<T,width>::Enable (void)
{
  _timer->CR1|=TIM_CR1_CEN;
}

template <class T, typename width>
void Pwm<T,width>::Disable (void)
{
  _timer->CR1&=~TIM_CR1_CEN;
}

#endif /* FAKES_PWM_HARDWARE_HPP_ */
