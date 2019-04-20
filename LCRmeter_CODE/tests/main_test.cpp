/*
 * main_test.cpp
 *
 *  Created on: 19.03.2019
 *      Author: Rafa³ Mazurkiewicz
 */




using namespace std;
#define CATCH_CONFIG_NO_CPP11_TO_STRING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include<iostream>
#include <Pwm.hpp>

int Factorial( int number ) {
   //return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
 return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}
#if 0
TEST_CASE( "Factorial of 0 is 1 (fail)", "[single-file]" ) {
    REQUIRE( Factorial(0) == 1 );
}

TEST_CASE( "Factorials of 1 and higher are computed (pass)", "[single-file]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
#endif
#if 0
int main(void){

	std::cout << "Hello World!" << std::endl;

	return 0;
}
#endif

#if 0

int main( int argc, char* argv[] ) {
  // global setup...

  int result = Catch::Session().run( argc, argv );

  // global clean-up...

  return result;
}
#endif
