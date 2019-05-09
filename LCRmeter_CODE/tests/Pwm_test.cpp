/*
 * Pwm_test.cpp
 *
 *  Created on: 19.03.2019
 *      Author: Rafa³ Mazurkiewicz
 */

#include <Pwm_hardware.hpp>
#include <Pwm.hpp>
#include "catch.hpp"
#include "fakeit.hpp"

//using namespace fakeit;

int* kak;
Pwm <int,uint16_t,2>PWM(kak,100);

TEST_CASE( "Set PWM frequency") {

    PWM.Set_Frequency(20000);
    int result=(testing::clockfreq)/((testing::prescaler+1)*(testing::counter+1));
    REQUIRE( result== 20000 );

    PWM.Set_Frequency(500);
    result=(testing::clockfreq)/((testing::prescaler+1)*(testing::counter+1));
    REQUIRE( result== 500 );

    PWM.Set_Frequency(1);
    result=(testing::clockfreq)/((testing::prescaler+1)*(testing::counter+1));
    REQUIRE( result== 1 );

    PWM.Set_Frequency(700000);
    result=(testing::clockfreq)/((testing::prescaler+1)*(testing::counter+1));
    REQUIRE( result== 700000 );
}

TEST_CASE( "Set PWM frequency out-of-bounds") {
    PWM.Set_Frequency(730000);
    REQUIRE( (testing::counter)== 99 );
    REQUIRE( (testing::prescaler)== 0 );
}

TEST_CASE( "Set PWM duty") {
    PWM.Set_Duty(40);
    REQUIRE( (testing::compare2)== 40 );
    PWM.Set_Duty(70);
    REQUIRE( (testing::compare2)== 70 );
}

TEST_CASE( "Set PWM duty out-of-bounds") {
    PWM.Set_Duty(101);
    REQUIRE( (testing::compare2)== 100 );
}

