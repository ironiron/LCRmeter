/*
 * Pwm.cpp
 *
 *  Created on: 19.03.2019
 *      Author: Rafa³ Mazurkiewicz
 */

#include <Pwm.hpp>

#ifndef deb
template <typename T>
Pwm<T>::Pwm (T tim):_timer(tim)
{
  // TODO Auto-generated constructor stub

}
template <typename T>
Pwm<T>::~Pwm ()
{
  // TODO Auto-generated destructor stub
}

/**@brief This function computes optimal prescaler and ARR registers
 * for desired frequency. Ensuring at least more than 1% of duty resolution.
 *
 * @param frequency : desired frequency in intiger.
 *
 */
template <typename T>
void Pwm<T>::Set_Frequency (uint32_t frequency)
{

  _timer->PSC=0;
}
template <typename T>
uint32_t Pwm<T>::Get_Clock (void)
{
  return 72000000;
}

template void Set_Frequency<int>(uint32_t);
#endif
