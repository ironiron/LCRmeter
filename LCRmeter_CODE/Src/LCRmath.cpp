/*
 * LCRmath.cpp
 *
 *  Created on: 03.11.2019
 *      Author: Rafał
 */

#include <LCRmath.hpp>
#include <cmath>

  float LCR_math::reactance=0;
  float LCR_math::loss_angle=0;
  float LCR_math::inductance=0;
  uint32_t LCR_math::resistance=0;
uint32_t LCR_math::capacity = 0;

bool LCR_math::Calculate (float amplitude1, float amplitude2,
			  float angle, uint32_t frequency)
{
  //radians * (180.0 / M_PI);
  float voltage_rs=0;
  float current=0;
  float voltage_lcr=0;//across measuring leads
  float radians=angle*3.14159/180.0;
  // Calculate voltage across series resistor:
  voltage_rs=sqrt(pow(amplitude1-(amplitude2*cos(radians)),2)+ pow(amplitude2*sin(radians),2));
  inductance=voltage_rs;
  current=voltage_rs/series_resistance;
  loss_angle=3.14159/2-(radians+asin(amplitude2*sin(radians)/voltage_rs));
//  loss_angle=180-loss_angle*180.0/3.14159;
  loss_angle=3.14159-loss_angle;
  voltage_lcr=amplitude2*cos(loss_angle);
  reactance=voltage_lcr/current;


  loss_angle=loss_angle*180.0/3.14159;
  return 0;
}
