/**
 ******************************************************************************
 * @file    printf_redirection.h
 * @author  Rafał Mazurkiewicz
 * @date    01.04.2023
 * @brief   Some utils for printf redirection
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#ifndef PRINTF_REDIRECTION_H_
#define PRINTF_REDIRECTION_H_

#define ITM_PRINTF
#define UART_PRINTF

#include "stm32f1xx_hal.h"

int _write(int file, char *ptr, int len);
UART_HandleTypeDef* UART_printf_init(void);

#endif /* PRINTF_REDIRECTION_H_ */
