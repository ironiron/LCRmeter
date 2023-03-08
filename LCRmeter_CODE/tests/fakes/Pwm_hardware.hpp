/*
 * Pwm_hardware.hpp
 *
 *  Created on: 18.04.2019
 *      Author: Rafa³ Mazurkiewicz
 */

#ifndef FAKES_PWM_HARDWARE_HPP_
#define FAKES_PWM_HARDWARE_HPP_

#include <Pwm.hpp>
#include "testing.hpp"

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Set_Prescaler (width psc)
{
  testing::pwm::prescaler=psc;
}

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Set_Compare (width comp)
{
  testing::pwm::compare=comp;
}

template <class T, typename width,uint8_t chn>
void Pwm<T,width,chn>::Set_Counter (width cnt)
{
  testing::pwm::counter=cnt;
}

template <class T, typename width,uint8_t chn>
uint32_t Pwm<T,width,chn>::Get_Clock (void)
{
  return testing::pwm::clockfreq;
}

#endif /* FAKES_PWM_HARDWARE_HPP_ */
