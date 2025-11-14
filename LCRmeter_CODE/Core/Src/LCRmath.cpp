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
#include <cmath>
#include <stdio.h>

double LCR_math::loss_angle = 0;
double LCR_math::inductance = 0;
double LCR_math::resistance = 0;
double LCR_math::capacitance = 0;
double LCR_math::series_resistance = 100;

//todo return type should also be pure resistance
LCR_math::load_type_t LCR_math::Calculate (double Vg, double Vx, double angle,
			  uint32_t frequency)
{
  double voltage_rs = 0;
  double current = 0;
  double voltage_lcr = 0; //across measuring leads
  double reactance =0;
  load_type_t retval;

  double radians = Deg_to_Rad (angle);
  // Calculate voltage across series resistor:
  voltage_rs = sqrt (
      pow (Vg - (Vx * cos (radians)), 2)
	  + pow (Vx * sin (radians), 2));
  if(voltage_rs == 0)
  {
      inductance = NAN;
      capacitance = NAN;
      resistance = NAN;
      retval = load_type_t::ERROR;
      return retval;
  }
  capacitance = 0;
  inductance = 0;

  //current
  current = voltage_rs / series_resistance;

  if (angle == 0)
  {
      resistance = Vx/current;
      capacitance = 0;
      inductance = 0;
      retval = load_type_t::RESISTIVE;
      return retval;
  }

  if (angle < 0)
    {
      radians = - radians;
      loss_angle = pi / 2 - (radians + asin (Vx * sin (radians) / voltage_rs));

      voltage_lcr = Vx * cos (loss_angle);
      reactance = voltage_lcr / current;
      inductance = reactance / 2 / frequency / pi;
      resistance = Vx * sin (loss_angle) / current;//ESR
      retval = load_type_t::INDUCTIVE;
    }
  else
    {
      loss_angle = (pi / 2) -  (radians +  acos ((Vg - Vx * cos (radians)) / voltage_rs)) ;

      voltage_lcr = Vx * cos (loss_angle);
      reactance = voltage_lcr / current;
      capacitance = 1/reactance / 2 / frequency / pi;
      resistance = Vx * sin (loss_angle) / current;//ESR

      if(loss_angle<0)
      {
          resistance = -NAN; // too small accuracy - discard measurment
      }
      retval = load_type_t::CAPACITIVE;
    }

  loss_angle = Rad_to_Deg (loss_angle);
  return retval;
}
