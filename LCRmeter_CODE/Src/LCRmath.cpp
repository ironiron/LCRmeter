/**
 ******************************************************************************
 * @file    LCRmath.cpp
 * @author  Rafał Mazurkiewicz
 * @date    03.11.2019
 * @brief   Calculates LCR values across device leads
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#include <LCRmath.hpp>
#include <cmath>

double LCR_math::reactance = 0;
double LCR_math::loss_angle = 0;
double LCR_math::inductance = 0;
double LCR_math::resistance = 0;
double LCR_math::capacitance = 0;
double LCR_math::series_resistance = 150;

bool LCR_math::Calculate (double amplitude1, double amplitude2, double angle,
			  uint32_t frequency)
{
  double voltage_rs = 0;
  double current = 0;
  double voltage_lcr = 0; //across measuring leads
  bool isinductive;

  double radians = Deg_to_Rad (angle);
  // Calculate voltage across series resistor:
  voltage_rs = sqrt (
      pow (amplitude1 - (amplitude2 * cos (radians)), 2)
	  + pow (amplitude2 * sin (radians), 2));
  //current
  current = voltage_rs / series_resistance;

  if (angle < 0)
    {
      //loss angle in radians
      loss_angle = pi / 2
	  - (radians + asin (amplitude2 * sin (radians) / voltage_rs));
      loss_angle = pi - loss_angle; //do not why
      voltage_lcr = amplitude2 * cos (loss_angle);
      reactance = voltage_lcr / current;
      inductance = reactance / 2 / frequency / 3.14159;
      //ESR
      isinductive = true;
    }
  else
    {
      //loss angle in radians
      loss_angle = pi / 2
	  - (radians
	      + acos ((amplitude1 - amplitude2 * cos (radians)) / voltage_rs));
      //loss_angle=pi-loss_angle;//do not why
      voltage_lcr = amplitude2 * cos (loss_angle);
      reactance = voltage_lcr / current;
      capacitance = 1 / reactance / 2 / frequency / 3.14159;
      isinductive = false;
    }
  resistance = amplitude2 * sin (loss_angle) / current;
  loss_angle = Rad_to_Deg (loss_angle);
  return isinductive;
}
