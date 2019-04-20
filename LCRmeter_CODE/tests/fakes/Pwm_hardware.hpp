/*
 * Pwm_hardware.hpp
 *
 *  Created on: 18.04.2019
 *      Author: Rafa³ Mazurkiewicz
 */

#ifndef FAKES_PWM_HARDWARE_HPP_
#define FAKES_PWM_HARDWARE_HPP_

#include <Pwm.hpp>
namespace testing
{
  extern int prescaler;
  extern int counter;
  extern int compare;
  extern int clockfreq;
}

template <class T, typename width>
void Pwm<T,width>::Set_Prescaler (width psc)
{
  testing::prescaler=psc;
}

template <class T, typename width>
void Pwm<T,width>::Set_Compare (width comp)
{
  testing::compare=comp;
}

template <class T, typename width>
void Pwm<T,width>::Set_Counter (width cnt)
{
  testing::counter=cnt;
}

template <class T, typename width>
uint32_t Pwm<T,width>::Get_Clock (void)
{
  return testing::clockfreq;
}

#endif /* FAKES_PWM_HARDWARE_HPP_ */
