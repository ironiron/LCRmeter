/**
  ******************************************************************************
  * @file    Pwm.hpp
  * @author  Rafał Mazurkiewicz
  * @date    14.05.2019
  * @brief   Class file header for PWM signal generation
  ******************************************************************************
  * @attention
  * <h2><center>&copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz</center></h2>
  ******************************************************************************
  */

#ifndef PWM_HPP_
#define PWM_HPP_
#include <stdint.h>

/*! @class Pwm
 *  @brief This class is intended to generate waveform on desired Timer channel of STM32 microcontrollers.
 *  Example of use:
 *    Pwm<TIM_TypeDef, uint16_t, 2> pwm (TIM1, 100);
 *    pwm.Initialise ();
 * 	  pwm.Set_Frequency (5000);
 * 	  pwm.Set_Duty (30);
 * 	  pwm.Enable ();
 *  @tparam T timer structure
 *  @tparam Specifies resolution of timer (uint16_t uint32_t etc.)
 *  @tparam chn timer's channel on which will be generated signal (if chn=0 all 4 channels will be used)
 */
template < class  T, typename width,uint8_t chn>
 class Pwm
{
public:
  /**@brief Constructor. Only initializes variables
   * @param tim: pointer to timer structure
   * @param period: max counter value
   */
  Pwm ( T* tim,width period=100):
    _timer(tim),
    cnt_initialvalue(period)//TODO rename period to something more meaningful
  {
    static_assert(chn<=4,"error, channel can be of value 0-4");
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
  * @note If parameter \a duty is over 100% it will be implicitly reduced to 100%
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
    * @param width : counter value to set
    */
   void Set_Counter (width);

   /**@brief Sets timer's compare register
    * @param width : set desired compare register value
    */
   void Set_Compare  (width);

  private:
   /**@brief Gets base frequency of timer clock
    * @return  clock value in integer
    * @warning Currently The value is hardcoded to 72 MHz
    */
   uint32_t Get_Clock (void);
    T* _timer; 			///< pointer to template timer struct
    const uint8_t channel = chn; ///<channel of timer on which PWM will be generated
    uint8_t duty; 		///<duty of the PWM
    const int cnt_initialvalue; ///<initial value of counter register used for calculating frequency
    int cnt = cnt_initialvalue; ///<shadow variable of timers counter register
};
#include <Pwm_hardware.hpp>
#endif /* PWM_HPP_ */
