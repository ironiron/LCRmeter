///*
// * Pwm_test.cpp
// *
// *  Created on: 19.03.2019
// *      Author: Rafa� Mazurkiewicz
// */
//
//#include <Pwm_hardware.hpp>
//#include <Pwm.hpp>
//#include "catch.hpp"
//
//int* k;//just random pointer
//Pwm <int,uint16_t,2>PWM(k,100);
//
//TEST_CASE( "Set PWM frequency") {
//
//    PWM.Set_Frequency(20000);
//    int result=(testing::pwm::clockfreq)/((testing::pwm::prescaler+1)*(testing::pwm::counter+1));
//    REQUIRE( result== 20000 );
//
//    PWM.Set_Frequency(500);
//    result=(testing::pwm::clockfreq)/((testing::pwm::prescaler+1)*(testing::pwm::counter+1));
//    REQUIRE( result== 500 );
//
//    PWM.Set_Frequency(1);
//    result=(testing::pwm::clockfreq)/((testing::pwm::prescaler+1)*(testing::pwm::counter+1));
//    REQUIRE( result== 1 );
//
//    PWM.Set_Frequency(700000);
//    result=(testing::pwm::clockfreq)/((testing::pwm::prescaler+1)*(testing::pwm::counter+1));
//    REQUIRE( result== 700000 );
//}
//
//TEST_CASE( "Set PWM frequency out-of-bounds") {
//    PWM.Set_Frequency(730000);
//    REQUIRE( (testing::pwm::counter)== 99 );
//    REQUIRE( (testing::pwm::prescaler)== 0 );
//}
//
//TEST_CASE( "Set PWM duty") {
//    PWM.Set_Duty(40);
//    REQUIRE( (testing::pwm::compare)== 40 );
//    PWM.Set_Duty(70);
//    REQUIRE( (testing::pwm::compare)== 70 );
//}
//
//TEST_CASE( "Set PWM duty out-of-bounds") {
//    PWM.Set_Duty(101);
//    REQUIRE( (testing::pwm::compare)== 100 );
//}
//
