/**
 ******************************************************************************
 * @file    printf_redirection.c
 * @author  Rafał Mazurkiewicz
 * @date    01.04.2023
 * @brief   Some utils for printf redirection
 ******************************************************************************
 * @attention
 * &copy; standard MIT License COPYRIGHT(c) 2019 Rafał Mazurkiewicz
 ******************************************************************************
 */

#include "printf_redirection.h"

#ifdef UART_PRINTF
UART_HandleTypeDef huart1;
#endif

int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
#ifdef ITM_PRINTF
      ITM_SendChar((*ptr));
#endif
#ifdef UART_PRINTF
      HAL_UART_Transmit(&huart1,(uint8_t*)ptr,1,100);
#endif
      ptr++;
  }
  return len;
}

#ifdef UART_PRINTF
void UART_printf_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

      __HAL_RCC_USART1_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();

      GPIO_InitStruct.Pin = GPIO_PIN_9;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_HalfDuplex_Init(&huart1);
}
#endif
