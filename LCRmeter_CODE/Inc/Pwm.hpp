/*
 * Pwm.h
 *
 *  Created on: 19.03.2019
 *      Author: Rafa� Mazurkiewicz
 */

#ifndef PWM_HPP_
#define PWM_HPP_
#include <stdint.h>


#define deb

template < class  T, typename width,uint8_t chn>
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
  Pwm ( T* tim,width period=100):
    _timer(tim),
    cnt_initialvalue(period)
  {
    //static_assert(channel<=4,"edsdd");
    //Initialise();
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
  }

 void Set_Duty (uint8_t duty )
  {
    if (duty>100)
      {
	duty=100;
      }
    if(cnt!=cnt_initialvalue)
      {
	Set_Compare(duty*cnt/cnt_initialvalue,channel);
      }
    else
      {
	if(cnt!=0)
	  {
	    Set_Compare(duty*cnt/100,channel);
	  }
	else
	{
	    Set_Compare(duty,channel);
	}
      }
    this->duty=duty;
  }


   void Enable (void);
   void Disable (void);
  void Initialise (void);
   void Set_Prescaler (width);
   void Set_Counter (width);
   void Set_Compare  (width,uint8_t) ;
   uint32_t Get_Clock (void);

private:
  T* _timer;
  const uint8_t channel=chn;
  uint8_t duty;
  const int cnt_initialvalue;
  int cnt=cnt_initialvalue;

#endif
};
#include <Pwm_hardware.hpp>
#endif /* PWM_HPP_ */