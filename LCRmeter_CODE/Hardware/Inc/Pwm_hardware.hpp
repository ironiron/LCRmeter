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
void Pwm<T,width,chn>::Set_Compare (width comp,uint8_t channel)
{
  //static_assert(channel<=4,"error, channel can be of value 0-4");
  switch(channel)
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
  return 84000000;
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
