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

#define WELCOME_MSG "Welcome to the Nucleo management console\r\n"
#define MAIN_MENU                                                                             \
  "Select the option you are interested in:\r\n\t1. Toggle LD2 LED\r\n\t2. Read USER BUTTON " \
  "status\r\n\t3. Clear screen and print this message "
#define PROMPT "\r\n> "

UART_HandleTypeDef huart2;

static void GpioInit() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIOInitStruct(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);  // LED
  GPIOInitStruct(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_NOPULL);     // button
}

static void UartInit() {
  __HAL_RCC_USART2_CLK_ENABLE();
  GPIOInitStruct(GPIOA, GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, 3);  // TX
  GPIOInitStruct(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL);     // RX

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
}

void printWelcomeMessage() {
  HAL_UART_Transmit(&huart2, (uint8_t*)"\033[0;0H", strlen("\033[0;0H"), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)"\033[2J", strlen("\033[2J"), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)WELCOME_MSG, strlen(WELCOME_MSG), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t*)MAIN_MENU, strlen(MAIN_MENU), HAL_MAX_DELAY);
}

uint8_t processUserInput(uint8_t opt) {
  char msg[30];
  if (!opt || opt > 3) return 0;
  sprintf(msg, "%d", opt);
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
  switch (opt) {
    case 1:
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      break;
    case 2:
      sprintf(msg, "\r\nUSER BUTTON status: %s",
              HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET ? "PRESSED" : "RELEASED");
      HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
      break;
    case 3:
      return 2;
  };
  return 1;
}

uint8_t readUserInput(void) {
  char readBuf[1];
  HAL_UART_Transmit(&huart2, (uint8_t*)PROMPT, strlen(PROMPT), HAL_MAX_DELAY);
  HAL_UART_Receive(&huart2, (uint8_t*)readBuf, 1, HAL_MAX_DELAY);
  return atoi(readBuf);
}

int main(void) {
  uint8_t opt = 0;

  HAL_Init();

  SystemClockInit();
  GpioInit();
  UartInit();

printMessage:
  printWelcomeMessage();
  while (1) {
    opt = readUserInput();
    processUserInput(opt);
    if (opt == 3) {
      goto printMessage;
    }
  }
}
