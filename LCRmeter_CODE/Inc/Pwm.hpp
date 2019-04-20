/*
 * Pwm.h
 *
 *  Created on: 19.03.2019
 *      Author: Rafa³ Mazurkiewicz
 */

#ifndef PWM_HPP_
#define PWM_HPP_
#include <stdint.h>


#define deb

template <class T, typename width>
class Pwm
{
#ifndef deb
public:
  Pwm (T);
  void Set_Frequency(uint32_t );
private:
  const T _timer;
  uint32_t Get_Clock(void);
  void Set_Prescaler (uint32_t);
public:
  virtual  ~Pwm ();
#else
public:
  Pwm (T *tim):_timer(tim)
  {
    // TODO Auto-generated constructor stub
    //CR1|=(1<<ARPE)|	;//(1<<CEN);
   // CCMR1|=(1<<OC1M2-1);
    //EGR|=(1<<UG);

  }
virtual  ~Pwm ()
  {
    // TODO Auto-generated destructor stub
  }

  /**@brief This function computes optimal prescaler and ARR registers
   * for desired frequency. Ensuring at least 1% of duty resolution.
   *
   * @param frequency : desired frequency in intiger.
   *
   */

  void Set_Frequency (uint32_t frequency)
  {
    int psc=0;
    cnt=cnt_initialvalue;//- set initial value
    psc=Get_Clock()/cnt/frequency;
    if (psc>0xffff)
      {
	 psc=0xffff;
	 cnt=Get_Clock()/psc/frequency;
      }
    if (psc==0)
      {
	psc++;
      }
    Set_Prescaler(psc-1);
    Set_Counter(cnt-1);

    Set_Duty(duty);// Update after frequency change.


   // _timer->PSC=0;
  }

  void Set_Duty (uint8_t duty)
  {
    if (duty>100)
      {
	duty=100;
      }
    if(cnt!=cnt_initialvalue)
      {
	Set_Compare(duty*cnt/cnt_initialvalue);
      }
    else
      {
	Set_Compare(duty);
      }
    this->duty=duty;
  }

private:
  void Set_Prescaler (width);
  void Set_Counter (width);
  void Set_Compare (width);
  uint32_t Get_Clock (void);

  const T* _timer;
  uint8_t duty=0;
  const int cnt_initialvalue=100;
  int cnt=cnt_initialvalue;
#endif
};
#include "Pwm_hardware.hpp"
#endif /* PWM_HPP_ */
