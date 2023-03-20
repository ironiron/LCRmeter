#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f1xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

void HAL_Delay(uint32_t);
void delay_ms(int32_t);
void delay_us(int32_t);

#ifdef __cplusplus
 }
#endif

#endif /* DELAY_H_ */
