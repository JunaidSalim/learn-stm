/*
  STM32F103RBT6
  Flash: 128 KB
  RAM:   20  KB

  A0:   ADC1 CH0
  A1:   ADC1 CH1 @ DMA1 CH1   (Up   strip reading)
  A2:   ADC1 CH2 @ DMA1 CH1   (Left strip reading)
  A3:   FM17580 RST
  A4:   SPI1 CS (Used by FM17580)
  A5:   LED

*/

#include "main.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;
volatile bool g_pressed = false;
volatile bool g_buttonChanged = false;

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_13) {
    g_pressed = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, g_pressed ? GPIO_PIN_SET : GPIO_PIN_RESET);
    g_buttonChanged = true;
  }
}

int main(void) {
  HAL_Init();
  SystemClockInit();

  // 1. Button: PC13 as input ( the blue button on STM32 )
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIOInitStruct(GPIOC, GPIO_PIN_13, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL);

  // 2. UART: USART2 for sending output
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();

  GPIOInitStruct(GPIOA, GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, 3);
  GPIOInitStruct(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL);
  GPIOInitStruct(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);  // LED

  huart2.Instance = USART2;                     // which hardware block
  huart2.Init.BaudRate = 115200;                // speed: bits per second
  huart2.Init.WordLength = UART_WORDLENGTH_8B;  // 8 data bits per byte
  huart2.Init.StopBits = UART_STOPBITS_1;       // 1 stop bit
  huart2.Init.Parity = UART_PARITY_NONE;        // no error-check bit
  huart2.Init.Mode = UART_MODE_TX_RX;           // enable transmitter and receiver
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // no RTS/CTS extra wires
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
  // HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  while (1) {
    if (g_buttonChanged) {
      g_buttonChanged = false;
      const char* msg = g_pressed ? "Pressed - LED on\r\n" : "Released - LED off\r\n";
      HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
    }
  }
}
