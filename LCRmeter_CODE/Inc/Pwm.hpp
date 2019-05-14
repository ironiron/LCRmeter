/**
  ******************************************************************************
  * @file    Pwm.h
  * @author  Rafa³ Mazurkiewicz
  * @brief   Class file header for PWM signal generation
  ******************************************************************************
  * @attention
  * <h2><center>&copy; standard MIT License COPYRIGHT(c) 2019 Rafa³ Mazurkiewicz</center></h2>
  ******************************************************************************
  */

#ifndef PWM_HPP_
#define PWM_HPP_
#include <stdint.h>


#define deb

/*! @class Pwm
 *  @brief This class is intended to generate waveform on desired Timer channel.
 *  @tparam T timer structure
 *  @tparam width base resolution of timer(uint16_t etc.)
 *  @tparam chn timer's channel on which will be generated signal
 */
template < class  T, typename width,uint8_t chn>
 class Pwm
{
public:
  /**@brief Constructor. Only initialise variables
   * @param tim: pointer to timer structure
   * @param period: max counter value
   */
  Pwm ( T* tim,width period=100):
    _timer(tim),
    cnt_initialvalue(period)
  {
  }
  /**@brief This function computes optimal prescaler and ARR registers
   * for desired frequency. Ensuring at least 1% of duty resolution.
   * @param frequency : desired frequency in int.
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

 /**@brief This function calculates compare registers for desired duty.
  * @param duty : desired duty in range of 0-100%.
  */
 void Set_Duty (uint8_t duty )
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
	if(cnt!=100)
	  {
	    Set_Compare(duty*cnt/100);
	  }
	else
	{
	    Set_Compare(duty);
	}
      }
    this->duty=duty;
  }

 /**@brief Enables HW timer
  */
   void Enable (void);

   /**@brief Disables HW timer
    */
   void Disable (void);

   /**@brief Initialise HW
    */
   void Initialise (void);

   /**@brief Sets only timer's prescaler register
    * @param width : prescaler to set
    */
   void Set_Prescaler (width);

   /**@brief Sets timer's counter register
    * @param width : prescaler to set
    */
   void Set_Counter (width);

   /**@brief Sets timer's compare register
    * @param width : prescaler to set
    */
   void Set_Compare  (width) ;

   /**@brief Gets base frequency of timer clock
    * @return  clock value in integer
    */
   uint32_t Get_Clock (void);

private:
  T* _timer;///< pointer to template timer struct
  const uint8_t channel=chn;///<channel of timer on which PWM will be generated
  uint8_t duty;///<duty of the PWM
  const int cnt_initialvalue;///<initial value of counter register used for calculating frequency
  int cnt=cnt_initialvalue;///<shadow variable of timers counter register
};
#include <Pwm_hardware.hpp>
#endif /* PWM_HPP_ */
