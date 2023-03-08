#include "delay.h"

volatile int32_t _timer_delay=0;

void SysTick_Handler(void)
{
	_timer_delay--;
}

void delay_ms(int32_t t)
{
	SysTick_Config(SystemCoreClock/1000);
	_timer_delay=t;
	while(_timer_delay>0)
	{
	}
}

void delay_us(int32_t t)
{
	_timer_delay=t;
	SysTick_Config(SystemCoreClock/1000000);
	while(_timer_delay>0)
	{
	}
}

void HAL_Delay(uint32_t Delay)
{
	delay_ms(Delay);
}
