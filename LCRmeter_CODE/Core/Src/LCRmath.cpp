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

#include "LCRmath.hpp"
#include "cstdio"
#include <cmath>

double LCR_math::reactance = 0;
double LCR_math::loss_angle = 0;
double LCR_math::inductance = 0;
double LCR_math::resistance = 0;
double LCR_math::capacitance = 0;
double LCR_math::series_resistance = 100;

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
  if(voltage_rs == 0)
  {
      inductance = NAN;
      capacitance = NAN;
      resistance = NAN;
      isinductive = false;
      return isinductive;
  }
  //current
  current = voltage_rs / series_resistance;//fixme add handling of voltage_rs equal 0

  if (angle < 0)
    {
      radians = - radians;
      loss_angle = pi / 2 - (radians + asin (amplitude2 * sin (radians) / voltage_rs));
      voltage_lcr = amplitude2 * cos (loss_angle);
      reactance = voltage_lcr / current;
      inductance = reactance / 2 / frequency / pi;
      resistance = amplitude2 * sin (loss_angle) / current;//ESR
      isinductive = true;
      capacitance = 0;
    }
  else
    {
//      loss_angle = (radians + asin (amplitude2 * sin (radians) / voltage_rs)) - pi / 2 ;
      loss_angle =pi /2 - (radians + asin (amplitude2 * sin (radians) / voltage_rs)) ;
      if(loss_angle<0)
      {
          loss_angle = -loss_angle;
      }
      voltage_lcr = amplitude2 * cos (loss_angle);
      reactance = voltage_lcr / current;
      capacitance = 1/reactance / 2 / frequency / pi;
      resistance = amplitude2 * sin (loss_angle) / current;//ESR
      isinductive = false;
      inductance = 0;
    }

  loss_angle = Rad_to_Deg (loss_angle);
  return isinductive;
}
